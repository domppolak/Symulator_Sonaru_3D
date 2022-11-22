#include "mainscene.hh"

MainScene::MainScene(QWidget *parent)
    : QOpenGLWidget(parent)
{


}

MainScene::~MainScene()
{

}

void MainScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    _platformModel = std::make_shared<PlatformModel>();
}

void MainScene::paintGL()
{

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

}

void MainScene::mouseMoveEvent(QMouseEvent *event)
{

}

void MainScene::keyPressEvent(QKeyEvent *key)
{

}

void MainScene::wheelEvent(QWheelEvent *event)
{

}
