#include "mainscene.hh"

MainScene::MainScene(QWidget *parent)
    : QOpenGLWidget(parent)
{

    alpha = 25;
    beta = -25;
    cameraFar = 55;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    _platformModel = nullptr;

}

MainScene::~MainScene()
{
    makeCurrent();
    _platformModel->deletePlatformModel();
}

void MainScene::addPlatform()
{
    makeCurrent();
    _platformModel = std::make_shared<PlatformModel>();
    _platformModel->setPosition(_platform.get_LocPos()/SIMULATION_SCALE);
    _platformModel->setScale(_platform.get_size()/SIMULATION_SCALE);

    for(uint sonarId = 0; sonarId < _platform.get_NumOfSonars(); ++sonarId)
        addSonar(_platform.getSonar(sonarId), _platformModel);

    _platformModel->createAllMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void MainScene::addPoleModel()
{
    makeCurrent();
    _poleModel.push_back(std::make_shared<PoleModel>());
    _poleModel.back()->setColor(QVector3D(0,0,0));
    _poleModel.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);

}

void MainScene::addPointModel()
{
    makeCurrent();
    _pointModel.push_back(std::make_shared<PointModel>());
    _pointModel.back()->setColor(QVector3D(1,0,0));
    _pointModel.back()->translate(0, 5, 0);
    _pointModel.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void MainScene::addFlatSurfaceModel()
{
    makeCurrent();
    _flatSurfaceModel.push_back(std::make_shared<FlatSurfaceModel>());
    _flatSurfaceModel.back()->setColor(QVector3D(0.5, 0.5, 0));
    _flatSurfaceModel.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void MainScene::addGridModel()
{
    makeCurrent();
    _gridModel = std::make_shared<GridModel>();
    _gridModel->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

QVector3D MainScene::getLastObjPosition(const Objects &obj) const
{
    switch(obj)
    {
        case PLATFORM:
                return _platformModel->getPostion();
        case POINT:
                return _pointModel.back()->getPostion();
        case STAKE:
                return _poleModel.back()->getPostion();
        case FLAT_SURFACE:
                return _flatSurfaceModel.back()->getPostion();
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    return QVector3D(0,0,0);
}

uint MainScene::getNumfOfObj(const Objects &obj) const
{
    switch(obj)
    {
        case PLATFORM:
                return 1;
        case POINT:
                return _pointModel.size();
        case STAKE:
                return _poleModel.size();
        case FLAT_SURFACE:
                return _flatSurfaceModel.size();
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    return 0;
}

void MainScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    if(!initShaders())
        qDebug() << "Nie udalo sie zainicjalozowac shaderow";

    getShaderParam();
    addGridModel();
}

void MainScene::paintGL()
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

    if(_gridModel != nullptr && _gridModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, _gridModel->get_mMatrix());
        _gridModel->drawGrid();
    }

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

    if(!_poleModel.empty()){
        for(auto &poleModel: _poleModel){
            if(poleModel->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, poleModel->get_mMatrix());
                poleModel->draw_line();
            }
        }
    }

    if(!_pointModel.empty()){
        for(auto &pointModel : _pointModel){
            if(pointModel->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, pointModel->get_mMatrix());
                pointModel->draw_point();
            }
        }
    }

    if(!_flatSurfaceModel.empty()){
        for(auto &flatSurface : _flatSurfaceModel){
            if(flatSurface->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, flatSurface->get_mMatrix());
                flatSurface->draw();
            }
        }
    }

    _shaderProgram.release();
}

void MainScene::resizeGL(int width, int height)
{
    if(height == 0) height = 1;

    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)width/(float)height, 0.001f, 1000);

    glViewport(0, 0, width, height);
}

void MainScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void MainScene::mouseMoveEvent(QMouseEvent *event)
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

void MainScene::keyPressEvent(QKeyEvent *key)
{

}

void MainScene::wheelEvent(QWheelEvent *event)
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

bool MainScene::initShaders()
{
    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader_mainScene.frag"))
        return false;

    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader_mainScene.vert"))
        return false;

    if(!_shaderProgram.link())
        return false;

    return true;
}

void MainScene::getShaderParam()
{
    _vertexAtrLocation = _shaderProgram.attributeLocation("vertex");
    _colorAtrLocation = _shaderProgram.attributeLocation("color");
    _pMatrixLocation = _shaderProgram.uniformLocation("pMatrix");
    _vMatrixLocation = _shaderProgram.uniformLocation("vMatrix");
    _mMatrixLocation = _shaderProgram.uniformLocation("mMatrix");
}

void MainScene::drawPlatform(std::shared_ptr<PlatformModel> const &inPlatformModel)
{
    if(inPlatformModel == nullptr) return;

    if(inPlatformModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inPlatformModel->get_mMatrix());
        inPlatformModel->draw();
    }
}

void MainScene::drawSonar(std::shared_ptr<SonarModel> const &inSonarModel)
{
    if(inSonarModel == nullptr) return;

    if(inSonarModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inSonarModel->get_mMatrix());
        inSonarModel->draw();
    }
}

void MainScene::drawReceiver(std::shared_ptr<Model> const &inReceiverModel)
{
    if(inReceiverModel == nullptr) return;

    if(inReceiverModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inReceiverModel->get_mMatrix());
        inReceiverModel->draw();
    }
}

void MainScene::drawTransmitter(std::shared_ptr<Model> const &inTransmitterModel)
{
    if(inTransmitterModel->isCreated() && inTransmitterModel != nullptr){
        _shaderProgram.setUniformValue(_mMatrixLocation, inTransmitterModel->get_mMatrix());
        inTransmitterModel->draw();
    }
}

void MainScene::addSonar(const Sonar &inSonar, std::shared_ptr<PlatformModel> &inPlatformModel)
{
    inPlatformModel->addSonarModel();
    SonarModel &sonModel = inPlatformModel->useSonarModel(inPlatformModel->getNumOfSonarModels()-1);
    sonModel.setPosition(inSonar.get_LocPos()/SIMULATION_SCALE);
    sonModel.setScale(inSonar.get_size()/SIMULATION_SCALE);
    sonModel.setSonarColor(inSonar.get_color());
    sonModel.setModelId(inSonar.get_id());

    addTransmitter(inSonar.getTransmitter(), sonModel);

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        addReceiver(inSonar.getReceiver(recId), sonModel);
}

void MainScene::addTransmitter(const Transmitter &inTransmitter, SonarModel &inSonarModel)
{
    Model &transmitterModel = inSonarModel.useTransmitterModel();
    transmitterModel.setPosition(inTransmitter.get_GlPos()/SIMULATION_SCALE);
    transmitterModel.setScale(inTransmitter.get_size()/SIMULATION_SCALE);
}

void MainScene::addReceiver(Receiver const &inReceiver, SonarModel &inSonarModel)
{
    inSonarModel.addReceiverModel();
    Model &receiverModel = inSonarModel.useReceiverModel(inSonarModel.getNumOfReceiverModels()-1);

    receiverModel.setPosition(inReceiver.get_GlPos()/SIMULATION_SCALE);
    receiverModel.setScale(inReceiver.get_size()/SIMULATION_SCALE);
    receiverModel.setModelId(inReceiver.get_id());
}
