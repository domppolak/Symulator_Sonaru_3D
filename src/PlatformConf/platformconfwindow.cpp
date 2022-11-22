#include "platformconfwindow.hh"
#include "ui_platformconfwindow.h"
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>

PlatformConfWindow::PlatformConfWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatformConfWindow)
{
    ui->setupUi(this);
    sizeWindow = new ChangeSizeWindow;

    setToolBar();
    setConnections();

    ui->idSonarSpinBox->setMinimum(0);
    ui->xCordSpinBox->setMinimum(-10000);
    ui->yCordSpinBox->setMinimum(-10000);
    ui->zCordSpinBox->setMinimum(-10000);
    ui->xCordSpinBox->setMaximum(10000);
    ui->yCordSpinBox->setMaximum(10000);
    ui->zCordSpinBox->setMaximum(10000);
    ui->rotSpinBox->setMinimum(-360);
    ui->rotSpinBox->setMaximum(360);

    SonarNumChanged();
    codLabelUpdate(0,0,0,0);
}

PlatformConfWindow::~PlatformConfWindow()
{
    delete ui;
    delete sizeWindow;
}

void PlatformConfWindow::codLabelUpdate(double x, double y, double z, double rot)
{

    ui->xCordSpinBox->setValue(x*SIMULATION_SCALE);
    ui->yCordSpinBox->setValue(y*SIMULATION_SCALE);
    ui->zCordSpinBox->setValue(z*SIMULATION_SCALE);
    ui->rotSpinBox->setValue(rot);
}

void PlatformConfWindow::setConnections()
{
    connect(sizeWindow, &ChangeSizeWindow::sonarSizeChanged, this, &PlatformConfWindow::onPlatformSizeChanged);
    connect(ui->SceneWidget, SIGNAL(sonarPositionChanged(QVector3D,double)), this, SLOT(on_sonarPositionChanged(QVector3D,double)));
}

void PlatformConfWindow::setToolBar()
{
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction("Save", this, SLOT(on_saveAction()));
    toolbar->addAction("Open file", this, SLOT(on_openAction()));
    toolbar->addAction("Add to simulation", this, SLOT(on_addAction()));
    layout()->setMenuBar(toolbar);
}

void PlatformConfWindow::updateScene()
{
    ui->SceneWidget->clearScene();

    for(uint i=0; i < _platform.get_NumOfSonars(); ++i){
        Sonar sonar = _platform.getSonar(i);
        ui->SceneWidget->addSonar(sonar);
    }

    ui->SceneWidget->update();
    SonarNumChanged();
}

void PlatformConfWindow::onPlatformSizeChanged()
{
    if(!sizeWindow->isSizeAccepted())
        return;

    sizeWindow->hide();
    ui->SceneWidget->setPlatformScale(sizeWindow->getSonarSize()/SIMULATION_SCALE);
    ui->SceneWidget->update();
    sizeWindow->setSizeAccepted(false);
}

void PlatformConfWindow::setSonarPosition(double x_mm, double y_mm, double z_mm, double newRotation)
{
    ui->SceneWidget->setSonarPosition(_idUpdatedSonar, QVector3D(x_mm, y_mm, z_mm)/SIMULATION_SCALE);
    ui->SceneWidget->setSonarRotation(_idUpdatedSonar, newRotation);
    ui->SceneWidget->update();
}

bool PlatformConfWindow::deleteSonarMsg()
{
    if(_idUpdatedSonar < 0)
        return false;

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Delete sonar"));
    msgBox.setText(QStringLiteral("Delete sonar %1 ?").arg(_idUpdatedSonar+1));
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *yes = msgBox.addButton(QStringLiteral("Yes"), QMessageBox::ActionRole);
    msgBox.addButton(QStringLiteral("No"), QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == yes)
        return true;

    return false;
}

void PlatformConfWindow::updateSonarId()
{
    for(uint i=0; i < _platform.get_NumOfSonars(); i++)
        _platform.sonar_setId(i, i);
}

void PlatformConfWindow::on_loadSonarPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to load sonar configuration"));
        qDebug() << "Failed to load sonar configuration";
    }else{
        _loadedSonar.resetSonar();
        _fileManager.readSonarConf(file, _loadedSonar);
        ui->loadSonarPushButton->setText("Sonar is loaded");
        _sonarIsLoaded = 1;
    }

    file.close();
}


void PlatformConfWindow::on_addSonarPushButton_clicked()
{
    if(_sonarIsLoaded < 0)
        return;

    _platform.addSonar(_loadedSonar);
    ui->SceneWidget->addSonar(_loadedSonar);
    ui->SceneWidget->update();
    updateSonarId();
    SonarNumChanged();
}


void PlatformConfWindow::on_deletePushButton_clicked()
{

    if(_idUpdatedSonar < 0 && _platform.get_NumOfSonars() > 0){
        QMessageBox::critical(this, QStringLiteral("Delete sonar"), QStringLiteral("Select sonar to delete"));
        return;
    }

    if(!deleteSonarMsg() && _idUpdatedSonar < 0)
        return;

    if(_platform.deleteSonar(_idUpdatedSonar)){
        ui->SceneWidget->deleteSonar(_idUpdatedSonar);
        updateSonarId();
        SonarNumChanged();
    }
}

void PlatformConfWindow::on_xCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(arg1, ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void PlatformConfWindow::on_yCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(ui->xCordSpinBox->value(), arg1, ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void PlatformConfWindow::on_zCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), arg1, ui->rotSpinBox->value());
}

void PlatformConfWindow::on_rotSpinBox_valueChanged(double arg1)
{
    setSonarPosition(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), arg1);
}

void PlatformConfWindow::on_sizePushButton_clicked()
{
    sizeWindow->show();
    sizeWindow->updateLabel(ui->SceneWidget->getPlatformScale()*SIMULATION_SCALE);
}

void PlatformConfWindow::on_saveAction()
{
    _platform.set_size(ui->SceneWidget->getPlatformScale());
    for(uint i=0; i<_platform.get_NumOfSonars(); ++i){
        _platform.sonar_setLocPos(i, ui->SceneWidget->getSonarPosition(i)*SIMULATION_SCALE);
        _platform.sonar_setRotation(i, ui->SceneWidget->getSonarRotation(i));

        Sonar &sonar = _platform.useSonar(i);
        sonar.transmitter_setGlPos(ui->SceneWidget->getTransmitterPosition(i)*SIMULATION_SCALE);
        sonar.transmitter_setRotation(ui->SceneWidget->getTransmitterRotation(i));

        for(uint j=0; j<sonar.get_numOfRec(); ++j){
            sonar.rec_setGlPos(j, ui->SceneWidget->getReceiverGlPos(i, j));
            sonar.rec_setRotation(j, ui->SceneWidget->getReceiverRotation(i,j));
        }
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)") + ".xml";
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        qDebug() << "Failed to open file";
        return;
    }

    _fileManager.savePlatformConf(file, _platform);
    file.close();
}

void PlatformConfWindow::on_openAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open platform configuration file"));
        return;
    }else{
        _platform.resetPlatform();

        _fileManager.readPlatformConf(file, _platform);
    }

    file.close();

    updateScene();
}

void PlatformConfWindow::on_addAction()
{

}

void PlatformConfWindow::on_sonarPositionChanged(QVector3D position, double rotation)
{
    codLabelUpdate(position[0], position[1], position[2], rotation);
}

void PlatformConfWindow::SonarNumChanged()
{
    ui->num_label->setText(QStringLiteral("Number of sonar: %1").arg(_platform.get_NumOfSonars()));
    ui->idSonarSpinBox->setMaximum(_platform.get_NumOfSonars());
}

void PlatformConfWindow::on_idSonarSpinBox_valueChanged(int arg1)
{
    if(_platform.get_NumOfSonars() == 0 && arg1 == 0){
        _idUpdatedSonar =  -1;
        codLabelUpdate(0.0, 0.0, 0.0, 0.0);
        ui->idSonarSpinBox->setValue(0);        
    }else{
        _idUpdatedSonar = arg1-1;
        ui->SceneWidget->selectSonar(_idUpdatedSonar);
        QVector3D sonarPosition = ui->SceneWidget->getSonarPosition(_idUpdatedSonar);
        double sonarRotation = ui->SceneWidget->getSonarRotation(_idUpdatedSonar);
        codLabelUpdate(sonarPosition[0],sonarPosition[1],sonarPosition[2],sonarRotation);
    }
}

