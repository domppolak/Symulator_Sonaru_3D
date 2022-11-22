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
#include "platformmodel.hh"

class MainScene :  public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();

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


    float alpha;
    float beta;
    float cameraFar;
    QPoint lastMousePosition;
};
#endif // MAINSCENE_HH
