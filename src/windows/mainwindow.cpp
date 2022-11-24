#include "mainwindow.hh"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _sonarConfWindow = new SonarConfWindow(this);
    _platformConfWindow = new PlatformConfWindow(this);
    _addObjToScene = new ObjToScene(this);

    setMenuBar();
    connect(_addObjToScene, &ObjToScene::objct2Add, this, &MainWindow::on_objc2Add);
}

MainWindow::~MainWindow()
{
    delete figuresMenu;
    delete receiverMenu;
    delete ui;
    delete _sonarConfWindow;
    delete _platformConfWindow;
    delete _addObjToScene;
}

void MainWindow::setMenuBar()
{
    ui->menubar->addAction("Save", this, SLOT(on_saveAction()));
    ui->menubar->addAction("Sonar", this, SLOT(on_sonarAction()));
    ui->menubar->addAction("Platform", this, SLOT(on_platformAction()));


    figuresMenu = new QMenu("Figures");
    ui->menubar->addMenu(figuresMenu);
    figuresMenu->addAction("Transmiter figure", this, SLOT(on_transmiterFigureAction()));

    receiverMenu = new QMenu("Receiver figures");
    figuresMenu->addMenu(receiverMenu);
    receiverMenu->addAction("All figures", this, SLOT(on_receiverFiguresAction()));
}

void MainWindow::on_saveAction()
{
    qDebug() << "save action";
}

void MainWindow::on_sonarAction()
{
    _sonarConfWindow->show();
}

void MainWindow::on_platformAction()
{
    _platformConfWindow->show();
}

void MainWindow::on_transmiterFigureAction()
{
    qDebug() << "transmiter action";
}

void MainWindow::on_receiverFiguresAction()
{
    qDebug() << "receiver figures action";
}

void MainWindow::on_pushButton_3_clicked()
{
    _addObjToScene->show();
}


void MainWindow::on_startSimPushButton_clicked()
{
    /*_platform.set_size(250, 140, 250);
    _platform.set_LocPos(0,0,0);
    _platform.addSonar();
    _platform.useSonar(0).set_LocPos(0, 150, 100);
    _platform.useSonar(0).set_color(1, 0.5, 0);
    _platform.useSonar(0).set_size(50, 35, 50);
    _platform.useSonar(0).set_id(0);

    _platform.useSonar(0).useTransmitter().set_GlPos(0, 150, 123.81);
    _platform.useSonar(0).useTransmitter().set_size(5, 5, 5);

    _platform.useSonar(0).addReceiver();
    _platform.useSonar(0).useReceiver(0).set_GlPos(0, 100, 100);
    _platform.useSonar(0).useReceiver(0).set_size(5, 5, 12.5);

    ui->SceneWidget->setPlatform(_platform);
    ui->SceneWidget->addPlatform();
    ui->SceneWidget->update();*/
    ui->SceneWidget->addPointModel();
    ui->SceneWidget->update();

}


void MainWindow::on_stopSimPushButton_clicked()
{
    ui->SceneWidget->addPoleModel();
    ui->SceneWidget->update();
}


void MainWindow::on_deletePushButton_clicked()
{
    ui->SceneWidget->addFlatSurfaceModel();
    ui->SceneWidget->update();
}

void MainWindow::on_objc2Add(const Objects &obj)
{
    switch(obj)
    {
        case PLATFORM:
            ui->SceneWidget->addPlatform();
            ui->SceneWidget->update();
            break;
        case POINT:
            ui->SceneWidget->addPointModel();
            ui->SceneWidget->update();
            break;
        case STAKE:
            ui->SceneWidget->addPoleModel();
            ui->SceneWidget->update();
            break;
        case FLAT_SURFACE:
            ui->SceneWidget->addFlatSurfaceModel();
            ui->SceneWidget->update();
            break;
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

   _addObjToScene->updateSizeCordLabel(ui->SceneWidget->getLastObjPosition(obj)*10);
   _addObjToScene->updateLabel(ui->SceneWidget->getNumfOfObj(obj), obj);
}

