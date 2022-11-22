#ifndef PLATFORMCONFSCENE_HH
#define PLATFORMCONFSCENE_HH

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QPoint>
#include <QMatrix4x4>
#include <QVector4D>
#include <QMouseEvent>
#include <model.hh>
#include "sonar.hh"
#include <memory>

struct sonar_struct{
    std::shared_ptr<Model> sonar;
    std::shared_ptr<Model> transmitter;
    QVector<std::shared_ptr<Model>> receiver;
};

class PlatformConfScene : public QOpenGLWidget, public QOpenGLFunctions
{

    Q_OBJECT

public:
    explicit PlatformConfScene(QWidget *parent = nullptr);
    ~PlatformConfScene();

    void createPlatform(QVector3D scale);
    void addSonar(Sonar const &sonar);
    void deleteSonar();
    void deleteSonar(uint id);
    void selectSonar(int id);

    void setSonarPosition(uint id, QVector3D const &newPosition);
    void setSonarRotation(uint id, double newRotation);
    void setPlatformScale(QVector3D newScale);
    void setSonarId(uint id, uint newId);


    QVector3D getSonarPosition(uint id) const;
    double getSonarRotation(uint id) const;
    QVector3D getPlatformScale() const;
    uint getSonarId(uint id) const;

    QVector3D getTransmitterPosition(uint sonarId) const;
    double getTransmitterRotation(uint sonarId) const;

    QVector3D getReceiverGlPos(uint sonarId, uint recId) const;
    double getReceiverRotation(uint sonarId, uint recId) const;

    void clearScene();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void sonarPositionChanged(QVector3D position, double rotation);

private:
    bool initShaders();
    void getShaderParam();
    void initVertices();
    void sonarColorVertices(QVector3D sonarColor);
    void computeAllRecPosition(uint id, double rotation, QVector3D translate);
    void sonarTranslate(uint id, QVector3D const &translate);

    int selectedSonar = -1;

    //models
    std::shared_ptr<Model> _platform;
    std::vector<sonar_struct> _sonars;
    QVector3D _platformColor;

    //matrixes
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    //shader program settings
    QOpenGLShaderProgram _shaderProgram;
    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    //vertices
    QVector<QVector3D> _sonarVertices;
    QVector<QVector3D> _transmitterVertices;
    QVector<QVector3D> _receiverVertices;
    QVector<QVector3D> _platformVertices;

    //color vertices
    QVector<QVector3D> _sonarColorVertices;
    QVector<QVector3D> _transmitterColorVertices;
    QVector<QVector3D> _receiverColorVertices;
    QVector<QVector3D> _platformColorVertices;

    //camera settings
    float alpha;
    float beta;
    float cameraFar;
    static constexpr float TURN_SPEED = 0.3;
    static constexpr uint SIMULATION_SCALE = 10;
    QPoint lastMousePosition;



};

#endif // PLATFORMCONFSCENE_HH
