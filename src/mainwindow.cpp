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
}

MainWindow::~MainWindow()
{
    delete figuresMenu;
    delete receiverMenu;
    delete ui;
    delete _sonarConfWindow;
    delete _platformConfWindow;
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
    qDebug() << "sonar action";
}

void MainWindow::on_platformAction()
{
    _platformConfWindow->show();
    qDebug() << "platform action";
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

