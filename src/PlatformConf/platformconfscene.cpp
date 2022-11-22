#include "platformconfscene.hh"

PlatformConfScene::PlatformConfScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    alpha = 25;
    beta = -25;
    cameraFar = 55;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    _platformColor = QVector3D(1,0.5,0.5);
    initVertices();

    _platform = nullptr;

}

PlatformConfScene::~PlatformConfScene()
{

    makeCurrent();
    _platform->deleteModel();

    for(uint i=0; i<_sonars.size(); ++i){
        _sonars[i].sonar->deleteModel();
        for(uint j=0; j<_sonars[i].receiver.size(); ++j)
            _sonars[i].receiver[j]->deleteModel();
    }

}

void PlatformConfScene::createPlatform(QVector3D scale)
{
    makeCurrent();
    _platform = std::make_shared<Model>();
    _platform->setVertices(_platformVertices, _platformColorVertices);
    _platform->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    _platform->scale(scale);
}

void PlatformConfScene::addSonar(const Sonar &pSonar)
{
    makeCurrent();
    sonar_struct sonarStruct;
    sonarStruct.sonar = std::make_shared<Model>();
    sonarStruct.sonar->setModelId(pSonar.get_id());
    sonarStruct.sonar->scale(pSonar.get_size()/SIMULATION_SCALE);

    sonarColorVertices(pSonar.get_color());

    sonarStruct.sonar->setVertices(_sonarVertices, _sonarColorVertices);
    sonarStruct.sonar->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);

    sonarStruct.transmitter = std::make_shared<Model>();
    sonarStruct.transmitter->setVertices(_transmitterVertices, _transmitterColorVertices);
    sonarStruct.transmitter->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    sonarStruct.transmitter->setPosition(pSonar.transmitter_getLocPos()/SIMULATION_SCALE);
    sonarStruct.transmitter->scale(pSonar.transmitter_getSize()/SIMULATION_SCALE);

    for(uint i=0; i<pSonar.get_numOfRec(); ++i){
        std::shared_ptr<Model> rec(new Model());
        rec->setModelId(pSonar.rec_getId(i));
        rec->scale(pSonar.rec_getSize(i)/SIMULATION_SCALE);
        rec->setRotate(pSonar.rec_getRotation(i));
        rec->setPosition(pSonar.rec_getLocPos(i)/SIMULATION_SCALE);

        rec->setVertices(_receiverVertices, _receiverColorVertices);
        rec->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
        sonarStruct.receiver.push_back(rec);
    }

    _sonars.push_back(sonarStruct);
    sonarTranslate(_sonars.size()-1, QVector3D(0, -15, 10));
    update();

}

void PlatformConfScene::deleteSonar()
{
    if(_sonars.empty())
        return;

    makeCurrent();
     for(auto &sonar : _sonars.back().receiver){
        sonar->deleteModel();
        sonar.reset();
     }


    _sonars.back().sonar->deleteModel();
    _sonars.back().sonar.reset();
    _sonars.back().transmitter->deleteModel();
    _sonars.back().transmitter.reset();
    _sonars.pop_back();
    update();
}

void PlatformConfScene::deleteSonar(uint id)
{
    assert(id < _sonars.size());

    if(_sonars.empty())
        return;

    makeCurrent();
    for(auto &sonar : _sonars[id].receiver)
       sonar->deleteModel();

    _sonars[id].transmitter->deleteModel();
    _sonars[id].sonar->deleteModel();    
    _sonars.erase(_sonars.begin()+id);
    update();
}

void PlatformConfScene::selectSonar(int id)
{
    selectedSonar = id;
}

void PlatformConfScene::setSonarPosition(uint id, QVector3D const &newPosition)
{
    if(id > _sonars.size())
        return;

    QVector3D translate = newPosition - _sonars[id].sonar->getPostion();
    _sonars[id].sonar->setPosition(newPosition);
    _sonars[id].transmitter->translate(translate);
    for(auto &rec : _sonars[id].receiver)
        rec->translate(translate);
}

void PlatformConfScene::setSonarRotation(uint id, double newRotation)
{
    if(id < _sonars.size()){
        _sonars[id].sonar->setRotate(newRotation);
        computeAllRecPosition(id, newRotation, QVector3D(0,0,0));
    }
}

void PlatformConfScene::setPlatformScale(QVector3D newScale)
{
    _platform->scale(newScale);
}

void PlatformConfScene::setSonarId(uint id, uint newId)
{
    if(id < _sonars.size())
        _sonars[id].sonar->setModelId(newId);
}

QVector3D PlatformConfScene::getSonarPosition(uint id) const
{
    if(id < _sonars.size())
        return _sonars[id].sonar->getPostion();

    return QVector3D(0,0,0);
}

double PlatformConfScene::getSonarRotation(uint id) const
{
    if(id < _sonars.size())
        return _sonars[id].sonar->get_anglRoot();

    return 0;
}

QVector3D PlatformConfScene::getPlatformScale() const
{
    return _platform->getScale();
}

uint PlatformConfScene::getSonarId(uint id) const
{
    if(id < _sonars.size())
        return _sonars[id].sonar->getModelId();

    return 0;
}

QVector3D PlatformConfScene::getTransmitterPosition(uint sonarId) const
{
    assert(sonarId < _sonars.size());
    return _sonars[sonarId].transmitter->getPostion();
}

double PlatformConfScene::getTransmitterRotation(uint sonarId) const
{
    assert(sonarId < _sonars.size());
    return _sonars[sonarId].transmitter->get_anglRoot();
}

QVector3D PlatformConfScene::getReceiverGlPos(uint sonarId, uint recId) const
{
    assert(sonarId < _sonars.size());
    assert(recId < _sonars[sonarId].receiver.size());

    return _sonars[sonarId].receiver[recId]->getPostion()*SIMULATION_SCALE;

}

double PlatformConfScene::getReceiverRotation(uint sonarId, uint recId) const
{
    assert(sonarId < _sonars.size());
    assert(recId < _sonars[sonarId].receiver.size());

    return _sonars[sonarId].receiver[recId]->get_anglRoot();
}


void PlatformConfScene::clearScene()
{
    while(_sonars.size() > 0)
        deleteSonar();
}

void PlatformConfScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    if(!initShaders())
        qDebug() << "Nie udalo sie zainicjalizowac shaderow";

    getShaderParam();

    createPlatform(QVector3D(25, 14, 25));
}

void PlatformConfScene::paintGL()
{
    makeCurrent();
    _shaderProgram.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix, mMatrix, sMatrix;
    sMatrix.scale(0.35);
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation.map(QVector3D(0, 0, cameraFar));
    QVector3D cameraUpDirection = cameraTransformation.map(QVector3D(0, 1, 0));

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    _shaderProgram.setUniformValue(_pMatrixLocation, _pMatrix);
    _shaderProgram.setUniformValue(_vMatrixLocation, vMatrix);

    if(_platform != nullptr && _platform->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, _platform->getScaleMatrix());
        _platform->draw();
    }

    if(!_sonars.empty()){
    for(auto &sonar : _sonars){
        if(sonar.sonar->isCreated()){
            _shaderProgram.setUniformValue(_mMatrixLocation, sonar.sonar->get_mMatrix());
            sonar.sonar->draw();
            if(sonar.transmitter->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, sonar.transmitter->get_mMatrix());
                sonar.transmitter->draw();
            }
            for(auto &rec : sonar.receiver){
                if(rec->isCreated()){
                    _shaderProgram.setUniformValue(_mMatrixLocation, rec->get_mMatrix());
                    rec->draw();
                }
            }
        }
    }
    }

    _shaderProgram.release();
}

void PlatformConfScene::resizeGL(int width, int height)
{
    if(height == 0) height = 1;

    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)width/(float)height, 0.001f, 1000);

    glViewport(0, 0, width, height);
}

void PlatformConfScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void PlatformConfScene::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    double deltaX = (pos.x() - lastMousePosition.x())*0.3;
    double deltaY = (lastMousePosition.y() - pos.y())*0.3;

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -360) {
            beta = -360;
        }
        if (beta > 360) {
            beta = 360;
        }

        update();
    }

    lastMousePosition = event->pos();

    event->accept();
}

void PlatformConfScene::keyPressEvent(QKeyEvent *key)
{
    QVector3D translate(0,0,0);
    double rotation = 0;

    if(_sonars.size() > 0 && selectedSonar >= 0 && selectedSonar < int(_sonars.size())){
        if(key->key() == Qt::Key_Q)
            rotation = -5;
        if(key->key() == Qt::Key_E)
            rotation =5;
        if(key->key() == Qt::Key_W)
            translate = QVector3D(0, 0.2, 0);
        if(key->key() == Qt::Key_S)
            translate = QVector3D(0, -0.2, 0);
        if(key->key() == Qt::Key_A)
            translate = QVector3D(-0.2, 0, 0);
        if(key->key() == Qt::Key_D)
            translate =QVector3D(0.2, 0, 0);
        if(key->key() == Qt::Key_Z)
            translate = QVector3D(0, 0, 0.2);
        if(key->key() == Qt::Key_X)
            translate = QVector3D( 0, 0, -0.2);

        sonarTranslate(selectedSonar, translate);
        //_sonars[selectedSonar].sonar->rotate(rotation);
        emit sonarPositionChanged(_sonars[selectedSonar].sonar->getPostion(), _sonars[selectedSonar].sonar->get_anglRoot());
    }

    update();
}

void PlatformConfScene::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    if(delta.y() > 0){
        cameraFar *= 0.95;
    }else{
        cameraFar *= 1.05;
    }

    update();
    event->accept();
}

bool PlatformConfScene::initShaders()
{
    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader_platform.frag"))
        return false;

    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader_platform.vert"))
        return false;

    if(!_shaderProgram.link())
        return false;

    return true;
}

void PlatformConfScene::getShaderParam()
{
    _vertexAtrLocation = _shaderProgram.attributeLocation("vertex");
    _colorAtrLocation = _shaderProgram.attributeLocation("color");
    _pMatrixLocation = _shaderProgram.uniformLocation("pMatrix");
    _vMatrixLocation = _shaderProgram.uniformLocation("vMatrix");
    _mMatrixLocation = _shaderProgram.uniformLocation("mMatrix");
}

void PlatformConfScene::initVertices()
{
    _sonarVertices << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
                   << QVector3D( 0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5)
                   << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) // Back
                   << QVector3D(-0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) // Left
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) // Right
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5)
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5, -0.5) // Top
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5,  0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5,  0.5) // Bottom
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5, -0.5);

    _receiverVertices = _sonarVertices;
    _platformVertices = _sonarVertices;
    _transmitterVertices = _sonarVertices;

    for(uint i=0; i<6; ++i){
        _receiverColorVertices << QVector3D(1, 1, 0);
        _sonarColorVertices << QVector3D(1, 0, 0);
        _platformColorVertices << QVector3D(0,0.2,0.2);
        _transmitterColorVertices << QVector3D(0,0,0);
    }

    for(uint i=0; i<30; ++i){
        _receiverColorVertices << QVector3D(0, 0, 0);
        _sonarColorVertices << QVector3D(0,1,0);
        _transmitterColorVertices << QVector3D(0,0,0);
        _platformColorVertices << _platformColor;
    }
}

void PlatformConfScene::sonarColorVertices(QVector3D sonarColor)
{
    _sonarColorVertices.clear();
    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);

    for(uint i=0; i<30; i++) _sonarColorVertices << sonarColor;
}

void PlatformConfScene::computeAllRecPosition(uint id, double rotation, QVector3D translate)
{
    if(id > _sonars.size()){
        return;
    }

    for(auto &i : _sonars[id].receiver){
        i->translate(translate);
        i->rotate(rotation);
    }
}

void PlatformConfScene::sonarTranslate(uint id, const QVector3D &translate)
{

    if(id > _sonars.size())
        return;

    _sonars[id].sonar->translate(translate);
    _sonars[id].transmitter->translate(translate);
    for(auto &rec : _sonars[id].receiver)
        rec->translate(translate);
}

