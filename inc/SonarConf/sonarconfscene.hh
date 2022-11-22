#ifndef SONARCONFSCENE_HH
#define SONARCONFSCENE_HH

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

#include "model.hh"

class SonarConfScene :  public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SonarConfScene(QWidget *parent = nullptr);
    ~SonarConfScene();

    void createReceiver(uint id);
    void deleteReceiver();
    void deleteReceiver(uint id);
    void createSonar(QVector3D scale);
    void createTransmitter(QVector3D sonarScale);
    void selectRec(int id) { selectedRecId = id; }

    void setRecPosition(uint id, QVector3D xyz);
    void setRecRotation(uint id, double rot);
    void setRecScale(uint id, QVector3D scale);

    QVector3D getRecPosition(uint id) const;
    double getRecRotation(uint id) const;
    QVector3D getRecScale(uint id) const;
    uint getRecId(uint id) const;

    QVector3D getSonarScale() const;
    void setSonarScale(QVector3D scale);


    void clearScene();
    void addReceiversToScene(uint numOfRec);

    void setSonarColor(QVector3D color);
    QVector3D getSonarColor() const;
    void changeSonarSize(QVector3D newSize);

    int isSceneInitialized();

    void setTransmitterPosition(QVector3D newPosition);
    void setTransmitterScale(QVector3D newScale);

    QVector3D getTransmitterPosision() const;
    QVector3D getTransmitterScale() const;

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
    void initVertices();

signals:
    void rec_centerPositionChanged(QVector3D, double rot);

private:
    QOpenGLShaderProgram _shaderProgram;
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    std::shared_ptr<Model> sonarModel = nullptr;
    std::shared_ptr<Model> transmitterModel = nullptr;
    std::vector<std::shared_ptr<Model>> recModels;

    float alpha;
    float beta;
    float cameraFar;
    QPoint lastMousePosition;

    int selectedRecId = -1;
    QVector3D _sonarColor;

    //POSITION VERTICES
    QVector<QVector3D> _sonarVertices;
    QVector<QVector3D> _transmitterVertices;
    QVector<QVector3D> _receiverVertices;
    QVector<QVector3D> _gridVertices;


    //COLOR VERTICIES
    QVector<QVector3D> _sonarColorVertices;
    QVector<QVector3D> _transmitterColorVertices;
    QVector<QVector3D> _receiverColorVertices;
    QVector<QVector3D> _gridColor;

    int sceneInitialized = -1;
};


#endif // SONARCONFSCENE_H
