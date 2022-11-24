#include "platformconfscene.hh"

PlatformConfScene::PlatformConfScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    alpha = 25;
    beta = -25;
    cameraFar = 55;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    _platform = nullptr;
}

PlatformConfScene::~PlatformConfScene()
{

    makeCurrent();
    _platformModel->deletePlatformModel();
}

void PlatformConfScene::addSonar(const Sonar &inSonar)
{
    QVector3D translate(0, -15, _platformModel->getScale()[2]/2);
    std::shared_ptr<SonarModel> sonarModel(new SonarModel());
    sonarModel->setSonarColor(inSonar.get_color());
    sonarModel->setScale(inSonar.get_size()/SIMULATION_SCALE);
    sonarModel->setModelId(inSonar.get_id());
    sonarModel->setSonarColor(inSonar.get_color());

    addTransmitter(inSonar.getTransmitter(), sonarModel);

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        addReceiver(inSonar.getReceiver(recId), sonarModel);

    _platformModel->addSonarModel(sonarModel);

    makeCurrent();
    sonarModel->createMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    sonarModel->translateSonar(translate);
}

void PlatformConfScene::addReadConfPlatform(const Platform &inPlatform)
{
    if(_platformModel == nullptr)
        _platformModel = std::shared_ptr<PlatformModel>();

    _platformModel->deletePlatformModel();
    _platformModel->setScale(inPlatform.get_size()/SIMULATION_SCALE);

    for(uint sonarId = 0; sonarId < inPlatform.get_NumOfSonars(); ++sonarId)
        addSonarReadFromPlatformConf(inPlatform.getSonar(sonarId));

    makeCurrent();
    _platformModel->createAllMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void PlatformConfScene::deleteSonar()
{
    makeCurrent();
    _platformModel->deleteSonarModel();
    update();
}

void PlatformConfScene::deleteSonar(uint sonarId)
{
    assert(sonarId < _platformModel->getNumOfSonarModels());
    makeCurrent();
    _platformModel->deleteSonarModel(sonarId);
    update();
}

void PlatformConfScene::createPlatform()
{
    makeCurrent();
    _platformModel = std::make_shared<PlatformModel>();
    _platformModel->setPosition(QVector3D(0,0,0));
    _platformModel->setScale(25, 15, 25);
    _platformModel->createAllMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void PlatformConfScene::getPlatformParam(Platform &inPlatform)
{
    inPlatform.set_LocPos(_platformModel->getPostion());
    inPlatform.set_size(_platformModel->getScale()*SIMULATION_SCALE);

    for(uint sonarId=0; sonarId < inPlatform.get_NumOfSonars(); ++sonarId)
        getSonarParam(inPlatform.useSonar(sonarId), _platformModel->getSonarModel(sonarId));
}

void PlatformConfScene::getSonarParam(Sonar &inSonar, SonarModel const &sonModel)
{
    inSonar.set_LocPos(sonModel.getPostion()*SIMULATION_SCALE);
    inSonar.set_rotation(sonModel.get_anglRoot());
    inSonar.set_id(sonModel.getModelId());

    getTransmitterParam(inSonar.useTransmitter(), sonModel.getTransmitterModel());

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        getReceiverParam(inSonar.useReceiver(recId), sonModel.getReceiverModel(recId));
}

void PlatformConfScene::getTransmitterParam(Transmitter &inTransmitter, Model const &transmitterModel)
{
    inTransmitter.set_GlPos(transmitterModel.getPostion()*SIMULATION_SCALE);
    inTransmitter.set_rotation(transmitterModel.get_anglRoot());
}

void PlatformConfScene::getReceiverParam(Receiver &inReceiver, Model const &receiverModel)
{
    inReceiver.set_GlPos(receiverModel.getPostion()*SIMULATION_SCALE);
    inReceiver.set_id(receiverModel.getModelId());
    inReceiver.set_rotation(receiverModel.get_anglRoot());
}

void PlatformConfScene::addSonarReadFromPlatformConf(const Sonar &inSonar)
{
    std::shared_ptr<SonarModel> sonarModel(new SonarModel());
    sonarModel->setSonarColor(inSonar.get_color());
    sonarModel->setScale(inSonar.get_size()/SIMULATION_SCALE);
    sonarModel->setModelId(inSonar.get_id());
    sonarModel->setSonarColor(inSonar.get_color());
    sonarModel->setPosition(inSonar.get_LocPos()/SIMULATION_SCALE);

    addTransmitterReadFromPlatformConf(inSonar.getTransmitter(), sonarModel);

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        addReceiverReadFromPlatformConf(inSonar.getReceiver(recId), sonarModel);

    _platformModel->addSonarModel(sonarModel);

    makeCurrent();
    sonarModel->createMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void PlatformConfScene::addTransmitterReadFromPlatformConf(const Transmitter &inTransmitter, std::shared_ptr<SonarModel> &inSonarModel)
{
    Model &transmitterModel = inSonarModel->useTransmitterModel();
    transmitterModel.setPosition(inTransmitter.get_GlPos()/SIMULATION_SCALE);
    transmitterModel.setScale(inTransmitter.get_size()/SIMULATION_SCALE);
}

void PlatformConfScene::addReceiverReadFromPlatformConf(const Receiver &inReceiver, std::shared_ptr<SonarModel> &inSonarModel)
{
    inSonarModel->addReceiverModel();
    Model &receiverModel = inSonarModel->useReceiverModel(inSonarModel->getNumOfReceiverModels()-1);

    receiverModel.setPosition(inReceiver.get_GlPos()/SIMULATION_SCALE);
    receiverModel.setScale(inReceiver.get_size()/SIMULATION_SCALE);
    receiverModel.setModelId(inReceiver.get_id());
    receiverModel.setRotate(inReceiver.get_rotation());
}

void PlatformConfScene::selectSonar(int id)
{
    selectedSonar = id;
}

void PlatformConfScene::setSonarPosition(uint sonarId, QVector3D const &newPosition)
{
    if(sonarId > _sonars.size())
        return;

    _platformModel->useSonarModel(sonarId).setSonarPostion(newPosition);
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
    _platformModel->setScale(newScale);
}

void PlatformConfScene::setSonarId(uint sonarId, uint newId)
{
    if(sonarId < _platformModel->getNumOfSonarModels())
        _platformModel->useSonarModel(sonarId).setModelId(newId);
}

QVector3D PlatformConfScene::getSonarPosition(uint sonarId) const
{
    assert(sonarId < _platformModel->getNumOfSonarModels());
    return _platformModel->getSonarModel(sonarId).getPostion();
}

double PlatformConfScene::getSonarRotation(uint sonarId) const
{
    assert(sonarId < _platformModel->getNumOfSonarModels());
    return _platformModel->getSonarModel(sonarId).get_anglRoot();
}

QVector3D PlatformConfScene::getPlatformScale() const
{
    return _platformModel->getScale();
}

void PlatformConfScene::clearScene()
{
    makeCurrent();
    _platformModel->deleteAllSonarModels();
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

    createPlatform();
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



    if(_platformModel != nullptr){
        drawPlatform(_platformModel);

        for(uint sonarId = 0; sonarId < _platformModel->getNumOfSonarModels(); ++sonarId){
            drawSonar(_platformModel->getSonarModelPointer(sonarId));
            drawTransmitter(_platformModel->getSonarModel(sonarId).getTransmitterModelPointer());
            for(uint recId = 0; recId < _platformModel->getSonarModel(sonarId).getNumOfReceiverModels(); ++recId){
                drawReceiver(_platformModel->getSonarModel(sonarId).getReceiverModelPointer(recId));
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
    //double rotation = 0;

    if(selectedSonar >= 0 && selectedSonar < int(_platformModel->getNumOfSonarModels())){
        /*if(key->key() == Qt::Key_Q)
            rotation = -5;
        if(key->key() == Qt::Key_E)
            rotation =5;*/
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

        _platformModel->useSonarModel(selectedSonar).translateSonar(translate);
        emit sonarPositionChanged(_platformModel->getSonarModel(selectedSonar).getPostion(),
                                  _platformModel->getSonarModel(selectedSonar).get_anglRoot());
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

void PlatformConfScene::drawPlatform(const std::shared_ptr<PlatformModel> &inPlatformModel)
{
    if(inPlatformModel == nullptr) return;

    if(inPlatformModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inPlatformModel->get_mMatrix());
        inPlatformModel->draw();
    }
}

void PlatformConfScene::drawSonar(const std::shared_ptr<SonarModel> &inSonarModel)
{
    if(inSonarModel == nullptr) return;

    if(inSonarModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inSonarModel->get_mMatrix());
        inSonarModel->draw();
    }
}

void PlatformConfScene::drawReceiver(const std::shared_ptr<Model> &inReceiverModel)
{
    if(inReceiverModel == nullptr) return;

    if(inReceiverModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inReceiverModel->get_mMatrix());
        inReceiverModel->draw();
    }
}

void PlatformConfScene::drawTransmitter(const std::shared_ptr<Model> &inTransmitterModel)
{
    if(inTransmitterModel->isCreated() && inTransmitterModel != nullptr){
        _shaderProgram.setUniformValue(_mMatrixLocation, inTransmitterModel->get_mMatrix());
        inTransmitterModel->draw();
    }
}

void PlatformConfScene::addTransmitter(const Transmitter &inTransmitter, std::shared_ptr<SonarModel> &inSonarModel)
{
    Model &transmitterModel = inSonarModel->useTransmitterModel();
    transmitterModel.setPosition(inTransmitter.get_LocPos()/SIMULATION_SCALE);
    transmitterModel.setScale(inTransmitter.get_size()/SIMULATION_SCALE);
}

void PlatformConfScene::addReceiver(const Receiver &inReceiver, std::shared_ptr<SonarModel> &inSonarModel)
{
    inSonarModel->addReceiverModel();
    Model &receiverModel = inSonarModel->useReceiverModel(inSonarModel->getNumOfReceiverModels()-1);

    receiverModel.setPosition(inReceiver.get_LocPos()/SIMULATION_SCALE);
    receiverModel.setScale(inReceiver.get_size()/SIMULATION_SCALE);
    receiverModel.setModelId(inReceiver.get_id());
    receiverModel.setRotate(inReceiver.get_rotation());
}

