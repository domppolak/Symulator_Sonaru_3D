#include "sonarconfscene.hh"
#include <QDebug>
#include <QTime>
#include <iterator>
//PVM


SonarConfScene::SonarConfScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    alpha = 25;
    beta = -25;
    cameraFar = 20;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    _sonarColor = QVector3D(0,1,0);
    initVertices();
}

SonarConfScene::~SonarConfScene()
{
    makeCurrent();
    sonarModel->deleteModel();
    for(auto &i : recModels) i->deleteModel();
}

void SonarConfScene::createReceiver(uint id)
{
    makeCurrent();

    std::shared_ptr<Model> rec(new Model());
    rec->setVertices(_receiverVertices, _receiverColorVertices);

    rec->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    rec->setModelId(id);

    rec->setPosition(QVector3D(0,0,0));
    rec->translate(QVector3D(0, -5, 0));
    rec->setScale(QVector3D(0.5, 0.5, 1.25));

    recModels.push_back(rec);
}

void SonarConfScene::deleteReceiver(uint id)
{
    assert(id < recModels.size());

    makeCurrent();
    if(!recModels.empty()){
        recModels[id]->deleteModel();
        recModels[id] = nullptr;
        recModels.erase(recModels.begin()+id);
    }

    update();
}

void SonarConfScene::deleteReceiver()
{
    makeCurrent();

    if(recModels.size()>0){
        recModels.back()->deleteModel();
        recModels.back() = nullptr;
        recModels.pop_back();
    }
    update();
}

void SonarConfScene::createSonar(QVector3D scale)
{
    makeCurrent();

    sonarModel = std::make_shared<Model>();
    sonarModel->setVertices(_sonarVertices, _sonarColorVertices);

    sonarModel->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);

    sonarModel->setModelId(0);
    sonarModel->setScale(scale);
}

void SonarConfScene::createTransmitter(QVector3D sonarScale)
{
    makeCurrent();
    transmitterModel = std::make_shared<Model>();
    transmitterModel->setVertices(_transmitterVertices, _transmitterColorVertices);

    transmitterModel->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);

    transmitterModel->setScale(0.5, 0.5, 0.5);
    transmitterModel->translate(0,0, sonarScale[2]/2.1);
}

void SonarConfScene::setRecPosition(uint id, QVector3D xyz)
{
    if(id < recModels.size())
        recModels[id]->setPosition(xyz);
}

void SonarConfScene::setRecRotation(uint id, double rot)
{
    if(id < recModels.size())
        recModels[id]->setRotate(rot);
}

double SonarConfScene::getRecRotation(uint id) const
{
    if(id < recModels.size())
        return recModels[id]->get_anglRoot();

    return 0;
}

QVector3D SonarConfScene::getRecPosition(uint id) const{

    if(id < recModels.size())
        return recModels[id]->getPostion();

    return QVector3D(0,0,0);
}

uint SonarConfScene::getRecId(uint id) const
{
    if(id < recModels.size()){
        return recModels[id]->getModelId();
    }

    return 0;
}

QVector3D SonarConfScene::getRecScale(uint id) const
{
    if(id < recModels.size()){
        return recModels[id]->getScale();
    }

    return QVector3D(0,0,0);
}

QVector3D SonarConfScene::getSonarScale() const
{
    return sonarModel->getScale();
}

void SonarConfScene::setSonarScale(QVector3D scale)
{
    sonarModel->setScale(scale);
}

void SonarConfScene::setRecScale(uint id, QVector3D scale)
{
    if(id < recModels.size())
        recModels[id]->setScale(scale);
}

int SonarConfScene::isSceneInitialized()
{
    return this->sceneInitialized;
}

QVector3D SonarConfScene::getTransmitterPosision() const
{
    return this->transmitterModel->getPostion();
}

QVector3D SonarConfScene::getTransmitterScale() const
{
    return this->transmitterModel->getScale();
}

bool SonarConfScene::initShaders()
{
    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader.frag"))
        return false;

    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader.vert"))
        return false;

    if(!_shaderProgram.link())
        return false;

    return true;
}

void SonarConfScene::getShaderParam()
{
    _vertexAtrLocation = _shaderProgram.attributeLocation("vertex");
    _colorAtrLocation = _shaderProgram.attributeLocation("color");
    _pMatrixLocation = _shaderProgram.uniformLocation("pMatrix");
    _vMatrixLocation = _shaderProgram.uniformLocation("vMatrix");
    _mMatrixLocation = _shaderProgram.uniformLocation("mMatrix");
}

void SonarConfScene::initVertices()
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
    _transmitterVertices = _sonarVertices;

    for(uint i=0; i<6; ++i){
        _receiverColorVertices << QVector3D(1, 1, 0);
        _sonarColorVertices << QVector3D(1, 0, 0);
    }

    for(uint i=0; i<30; ++i){
        _receiverColorVertices << QVector3D(0, 0, 0);
        _sonarColorVertices << _sonarColor;
    }

    for(uint i=0; i<36; ++i)
        _transmitterColorVertices << QVector3D(0,0,0);
}


void SonarConfScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    if(!initShaders())
        qDebug() << "Nie udalo sie zainicjalizowac shaderow";

    getShaderParam();

    createSonar(QVector3D(5, 3.5, 5));
    createTransmitter(QVector3D(5, 3.5, 5));
    sceneInitialized = 1;
}

void SonarConfScene::paintGL()
{
    makeCurrent();
    _shaderProgram.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix, mMatrix;
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation.map(QVector3D(0, 0, cameraFar));
    QVector3D cameraUpDirection = cameraTransformation.map(QVector3D(0, 1, 0));

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    _shaderProgram.setUniformValue(_pMatrixLocation, _pMatrix);
    _shaderProgram.setUniformValue(_vMatrixLocation, vMatrix);

    if(sonarModel != nullptr && sonarModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, sonarModel->getScaleMatrix());
        sonarModel->draw();
    }

    if(transmitterModel != nullptr && sonarModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, transmitterModel->get_mMatrix());
        transmitterModel->draw();
    }

    for(auto rec : recModels){
        if(rec->isCreated()){
            _shaderProgram.setUniformValue(_mMatrixLocation, rec->get_mMatrix());
            rec->draw();
        }
    }

    _shaderProgram.release();
}

void SonarConfScene::resizeGL(int width, int height)
{
    if(height == 0) height = 1;

    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)width/(float)height, 0.001f, 1000);

    glViewport(0, 0, width, height);
}

void SonarConfScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void SonarConfScene::mouseMoveEvent(QMouseEvent *event)
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

void SonarConfScene::keyPressEvent(QKeyEvent *key)
{
    if(recModels.size() > 0 && selectedRecId >= 0 && selectedRecId < int(recModels.size())){
        if(key->key() == Qt::Key_Q)
            recModels[selectedRecId]->rotate(-5);
        if(key->key() == Qt::Key_E)
            recModels[selectedRecId]->rotate(5);
        if(key->key() == Qt::Key_W)
            recModels[selectedRecId]->translate(QVector3D(0, 0.2, 0));
        if(key->key() == Qt::Key_S)
            recModels[selectedRecId]->translate(QVector3D(0, -0.2, 0));
        if(key->key() == Qt::Key_A)
            recModels[selectedRecId]->translate(QVector3D(-0.2, 0, 0));
        if(key->key() == Qt::Key_D)
            recModels[selectedRecId]->translate(QVector3D(0.2, 0, 0));
        if(key->key() == Qt::Key_Z)
            recModels[selectedRecId]->translate(QVector3D(0, 0, 0.2));
        if(key->key() == Qt::Key_X)
            recModels[selectedRecId]->translate(QVector3D(0, 0, -0.2));

        emit rec_centerPositionChanged(recModels[selectedRecId]->getPostion(), recModels[selectedRecId]->get_anglRoot());
    }

    update();

}

void SonarConfScene::wheelEvent(QWheelEvent *event)
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

void SonarConfScene::clearScene()
{
    while(recModels.size() > 0)
        deleteReceiver();
}

void SonarConfScene::addReceiversToScene(uint numOfRec)
{
    uint i=0;
    while(i < numOfRec)
        createReceiver(++i);

}

void SonarConfScene::setSonarColor(QVector3D color)
{
    makeCurrent();
    _sonarColorVertices.clear();
    _sonarColor = color;

    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);

    for(uint i=0; i<30; i++) _sonarColorVertices << _sonarColor;

    sonarModel->changeColor(_shaderProgram, _colorAtrLocation, _sonarColorVertices);
    update();
}

QVector3D SonarConfScene::getSonarColor() const
{
    return _sonarColor;
}

void SonarConfScene::changeSonarSize(QVector3D newSize)
{
    sonarModel->setScale(newSize);
    transmitterModel->setPosition(0,0,0);
    transmitterModel->translate(0,0, newSize[2]/2.1);
}

void SonarConfScene::setTransmitterPosition(QVector3D newPosition)
{
    transmitterModel->setPosition(newPosition);
}

void SonarConfScene::setTransmitterScale(QVector3D newScale)
{
    transmitterModel->setScale(newScale);
}

