#ifndef MAINSCENE_HH
#define MAINSCENE_HH

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
#include <memory>
#include "objtoscene.hh"
#include "platformmodel.hh"
#include "platform.hh"
#include "stakemodel.hh"
#include "pointmodel.hh"
#include "flatsurfacemodel.hh"
#include "gridmodel.hh"

class MainScene :  public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();
    void addPlatform();
    void deletePlatform();
    void setPlatform(Platform const &inPlatform);
    Platform const &getPlatfrom() const;
    Platform &usePlatfrom();
    void addPoleModel();
    void addPointModel();
    void addFlatSurfaceModel();
    void addGridModel();
    QVector3D getLastObjPosition(Objects const &obj) const;
    uint getNumfOfObj(Objects const &obj) const;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool initShaders();
    void getShaderParam();
    void drawPlatform(std::shared_ptr<PlatformModel> const &inPlatformModel);
    void drawSonar(std::shared_ptr<SonarModel> const &inSonarModel);
    void drawReceiver(std::shared_ptr<Model> const &inReceiverModel);
    void drawTransmitter(std::shared_ptr<Model> const &inTransmitterModel);

    void addSonar(Sonar const &inSonar, std::shared_ptr<PlatformModel> &inPlatformModel);
    void addTransmitter(Transmitter const &inTransmitter, SonarModel &inSonarModel);
    void addReceiver(Receiver const &inReceiver, SonarModel &inSonarModel);

    QOpenGLShaderProgram _shaderProgram;
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    std::shared_ptr<PlatformModel> _platformModel = nullptr;
    std::vector<std::shared_ptr<PoleModel>> _poleModel;
    std::vector<std::shared_ptr<PointModel>> _pointModel;
    std::vector<std::shared_ptr<FlatSurfaceModel>> _flatSurfaceModel;
    std::shared_ptr<GridModel> _gridModel = nullptr;

    Platform _platform;

    float alpha;
    float beta;
    float cameraFar;
    static constexpr float TURN_SPEED = 0.3;
    static constexpr uint SIMULATION_SCALE = 10;
    QPoint lastMousePosition;
};

inline void MainScene::setPlatform(const Platform &inPlatform)
{
    _platform = inPlatform;
}

inline const Platform &MainScene::getPlatfrom() const
{
    return _platform;
}

inline Platform &MainScene::usePlatfrom()
{
    return _platform;
}
#endif // MAINSCENE_HH
