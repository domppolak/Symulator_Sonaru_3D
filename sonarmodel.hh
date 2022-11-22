#ifndef SONARMODEL_HH
#define SONARMODEL_HH

#include "model.hh"

class SonarModel : public Model
{

public:
    SonarModel();
    ~SonarModel();

    void deleteAllReceiverModels();
    void addReceiverModel();
    void deleteSonarModel();
    void deleteReceiverModel();
    void deleteReceiverModel(uint const &recId);
    void setSonarColor(QVector3D const &sonarColor);
    void createMeshes(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation);
    void setSonarPostion(QVector3D const &newPosition);
    void translateSonar(QVector3D const &_translate);
    uint getNumOfReceiverModels();

    std::shared_ptr<Model> const &getTransmitterModelPointer() const;
    std::shared_ptr<Model> &useTransmitterModelPointer();
    Model const &getTransmitterModel() const;
    Model &useTransmitterModel();

    std::shared_ptr<Model> const &getReceiverModelPointer(const uint &recId) const;
    std::shared_ptr<Model> &useReceiverModelPointer(const uint &recId);
    Model const &getReceiverModel(uint const &recId) const;
    Model &useReceiverModel(uint const &recId);


private:
    void initAllVertices();
    void initReceiver_pos_color_Vertices();
    void initTransmitter_pos_color_Vertices();
    void initSonar_pos_color_Vertices();

    std::shared_ptr<Model> transmitterModel;
    std::vector<std::shared_ptr<Model>> receiverModels;

    //POSITION VERTICES
    QVector<QVector3D> _sonarVertices;
    QVector<QVector3D> _transmitterVertices;
    QVector<QVector3D> _receiverVertices;

    //COLOR VERTICIES
    QVector<QVector3D> _sonarColorVertices;
    QVector<QVector3D> _transmitterColorVertices;
    QVector<QVector3D> _receiverColorVertices;
};

inline uint SonarModel::getNumOfReceiverModels()
{
    return receiverModels.size();
}

inline const std::shared_ptr<Model> &SonarModel::getTransmitterModelPointer() const
{
    return transmitterModel;
}

inline std::shared_ptr<Model> &SonarModel::useTransmitterModelPointer()
{
    return transmitterModel;
}

inline const Model &SonarModel::getTransmitterModel() const
{
    return *transmitterModel;
}

inline Model &SonarModel::useTransmitterModel()
{
    return *transmitterModel;
}

inline const std::shared_ptr<Model> &SonarModel::getReceiverModelPointer(const uint &recId) const
{
    assert(recId < receiverModels.size());
    return receiverModels[recId];
}

inline std::shared_ptr<Model> &SonarModel::useReceiverModelPointer(const uint &recId)
{
    assert(recId < receiverModels.size());
    return receiverModels[recId];
}

inline const Model &SonarModel::getReceiverModel(const uint &recId) const
{
    assert(recId < receiverModels.size());
    return *receiverModels[recId];
}

inline Model &SonarModel::useReceiverModel(const uint &recId)
{
    assert(recId < receiverModels.size());
    return *receiverModels[recId];
}


#endif // SONARMODEL_HH
