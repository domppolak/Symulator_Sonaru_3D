#include "sonarmodel.hh"

SonarModel::SonarModel()
{
    Model();
    initAllVertices();
    transmitterModel = std::make_shared<Model>();
}

SonarModel::~SonarModel()
{
    deleteSonarModel();
}

void SonarModel::deleteAllReceiverModels()
{
    for(auto &rec : receiverModels)
        rec->deleteModel();

    receiverModels.clear();
}

void SonarModel::addReceiverModel()
{
    receiverModels.push_back(std::make_shared<Model>());
}

void SonarModel::deleteSonarModel()
{
    deleteModel();
    transmitterModel->deleteModel();
    for(auto &rec : receiverModels)
        rec->deleteModel();
}

void SonarModel::deleteReceiverModel()
{
    receiverModels.back()->deleteModel();
    receiverModels.pop_back();
}

void SonarModel::deleteReceiverModel(const uint &recId)
{
    assert(recId < receiverModels.size());
    receiverModels[recId]->deleteModel();
    receiverModels.erase(receiverModels.cbegin()+recId);
}

void SonarModel::setSonarColor(const QVector3D &sonarColor)
{
    _sonarColorVertices.clear();
    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);
    for(uint i=0; i<36; i++) _sonarVertices << sonarColor;
    setColorVertices(_sonarColorVertices);
}

void SonarModel::createMeshes(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation)
{
    if(!isCreated()){
        setVertices(_sonarVertices, _sonarColorVertices);
        createMesh(program, _vertexLocation, _colorLocation);
    }
    if(!transmitterModel->isCreated()){
        transmitterModel->setVertices(_transmitterVertices, _transmitterColorVertices);
        transmitterModel->createMesh(program, _vertexLocation, _colorLocation);
    }

    for(auto &rec : receiverModels){
        if(rec->isCreated()) continue;
        rec->setVertices(_receiverVertices, _receiverColorVertices);
        rec->createMesh(program, _vertexLocation, _colorLocation);
    }
}

void SonarModel::setSonarPostion(const QVector3D &newPosition)
{
    QVector3D _translate = newPosition - getPostion();
    setPosition(newPosition);
    transmitterModel->translate(_translate);

    for(auto &rec : receiverModels)
        rec->translate(_translate);
}

void SonarModel::translateSonar(const QVector3D &_translate)
{
    translate(_translate);
    transmitterModel->translate(_translate);

    for(auto &rec : receiverModels)
        rec->translate(_translate);
}

void SonarModel::initAllVertices()
{
    initSonar_pos_color_Vertices();
    initReceiver_pos_color_Vertices();
    initTransmitter_pos_color_Vertices();
}

void SonarModel::initReceiver_pos_color_Vertices()
{
    _receiverVertices = _sonarVertices;

    for(uint i=0; i<6; i++) _receiverColorVertices << QVector3D(1,1,0);
    for(uint i=0; i<36; i++) _receiverColorVertices << QVector3D(0,0,0);
}

void SonarModel::initTransmitter_pos_color_Vertices()
{
    _transmitterVertices = _sonarVertices;

    for(uint i=0; i<36; ++i) _transmitterColorVertices << QVector3D(0,0,0);
}

void SonarModel::initSonar_pos_color_Vertices()
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

    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);
    for(uint i=0; i<36; i++) _sonarColorVertices<< QVector3D(0,1,0);
}
