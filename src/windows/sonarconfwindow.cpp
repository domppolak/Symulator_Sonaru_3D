#include "sonarconfwindow.hh"
#include "ui_sonarConfWindow.h"
#include <QDebug>
#include <QVector3D>
#include <QMessageBox>



QString color[] = {"Green", "Blue", "Purple", "White", "Orange", "Brown", "Cyan"};
QVector3D color_vec[] {QVector3D(0,1,0), QVector3D(0,0,1), QVector3D(0.5,0,0.5),
            QVector3D(1,1,1), QVector3D(1,0.5,0), QVector3D(0.55,0.27,0.07), QVector3D(0,1,1)};

SonarConfWindow::SonarConfWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SonarConfWindow)
{
    ui->setupUi(this);
    setToolBar();

    _signalConfWindow = new SignalConfWindown;
    sonarSizeWindow = new ChangeSizeWindow;

    setConnections();
    ui->num_label->setText(QStringLiteral("Number of receivers: %1").arg(_sonar.get_numOfRec()));
    ui->idRecSpinBox->setMaximum(_sonar.get_numOfRec());
    ui->idRecSpinBox->setMinimum(0);
    ui->xCordSpinBox->setMinimum(-100);
    ui->yCordSpinBox->setMinimum(-100);
    ui->zCordSpinBox->setMinimum(-100);
    ui->rotSpinBox->setMinimum(-360);
    ui->rotSpinBox->setMaximum(360);
    ui->xCordSpinBox->setValue(0);
    ui->yCordSpinBox->setValue(0);
    ui->zCordSpinBox->setValue(0);
    ui->rotSpinBox->setValue(0);

    ui->sonarColor_comboBox->addItem("Green");
    ui->sonarColor_comboBox->addItem("Blue");
    ui->sonarColor_comboBox->addItem("Purple");
    ui->sonarColor_comboBox->addItem("White");
    ui->sonarColor_comboBox->addItem("Orange");
    ui->sonarColor_comboBox->addItem("Brown");
    ui->sonarColor_comboBox->addItem("Cyan");
    ui->sonarColor_comboBox->setCurrentIndex(0);

}

SonarConfWindow::~SonarConfWindow()
{
    delete _signalConfWindow;
    delete sonarSizeWindow;
    delete ui;
}

void SonarConfWindow::cordLabelUpdate(double x, double y, double z, double rot)
{
    ui->xCordSpinBox->setValue(x*SIMULATION_SCALE);
    ui->yCordSpinBox->setValue(y*SIMULATION_SCALE);
    ui->zCordSpinBox->setValue(z*SIMULATION_SCALE);
    ui->rotSpinBox->setValue(rot);
}

void SonarConfWindow::setConnections()
{
    connect(this, SIGNAL(numRecChanged()), this, SLOT(on_numRecChanged()));
    connect(ui->SceneWidget, SIGNAL(rec_centerPositionChanged(QVector3D,double)),
            this, SLOT(on_recCenterPositionChanged(QVector3D,double)));
    connect(sonarSizeWindow, SIGNAL(sonarSizeChanged()), this, SLOT(onSonarSizeChanged()));
    connect(_signalConfWindow, &SignalConfWindown::signalParamChanged,
            this, &SonarConfWindow::onSignalParamChanged);

}

void SonarConfWindow::setToolBar()
{
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction("Save", this, SLOT(on_saveAction()));
    toolbar->addAction("Open file", this, SLOT(on_openAction()));
    toolbar->addAction("Add to simulation", this, SLOT(on_addAction()));
    toolbar->addAction("Transmitter", this, SLOT(on_transAction()));
    layout()->setMenuBar(toolbar);
}

void SonarConfWindow::updateScene()
{
    ui->SceneWidget->clearScene();

    ui->SceneWidget->setSonarColor(_sonar.get_color());
    ui->SceneWidget->setSonarScale(_sonar.get_size()/SIMULATION_SCALE);

    ui->SceneWidget->setTransmitterPosition(_sonar.transmitter_getLocPos()/SIMULATION_SCALE);
    ui->SceneWidget->setTransmitterScale(_sonar.transmitter_getSize()/SIMULATION_SCALE);

    for(uint i=0; i<_sonar.get_numOfRec(); i++){
        ui->SceneWidget->createReceiver(_sonar.rec_getId(i));
        ui->SceneWidget->setRecPosition(i, _sonar.rec_getLocPos(i)/SIMULATION_SCALE);
        ui->SceneWidget->setRecRotation(i, _sonar.rec_getRotation(i));
        ui->SceneWidget->setRecScale(i, _sonar.rec_getSize(i)/SIMULATION_SCALE);
    }

    ui->SceneWidget->update();
    emit numRecChanged();
}

bool SonarConfWindow::deleteReceiverMsg()
{
    if(_idUpdatedRec < 0)
        return false;

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Delete receiver"));
    msgBox.setText(QStringLiteral("Delete receiver %1 ?").arg(_idUpdatedRec+1));
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *yes = msgBox.addButton(QStringLiteral("Yes"), QMessageBox::ActionRole);
    msgBox.addButton(QStringLiteral("No"), QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == yes)
        return true;

    return false;
}

void SonarConfWindow::updateRecId()
{
    uint i=0;
    while(++i <= _sonar.get_numOfRec())
        _sonar.rec_setId(i-1, i);
}

void SonarConfWindow::on_saveAction()
{
    _sonar.set_size(ui->SceneWidget->getSonarScale()*SIMULATION_SCALE);
    _sonar.set_color(ui->SceneWidget->getSonarColor());

    _sonar.transmitter_setLocPos(ui->SceneWidget->getTransmitterPosision()*SIMULATION_SCALE);
    _sonar.transmitter_setSize(ui->SceneWidget->getTransmitterScale()*SIMULATION_SCALE);

    for(uint i=0; i<_sonar.get_numOfRec(); ++i){
        _sonar.rec_setSize(i, ui->SceneWidget->getRecScale(i)*SIMULATION_SCALE);
        _sonar.rec_setLocPos(i, ui->SceneWidget->getRecPosition(i)*SIMULATION_SCALE);
        _sonar.rec_setRotation(i, ui->SceneWidget->getRecRotation(i));
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)") + ".xml";
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to save file"));
        return;
    }

    _fileManager.saveSonarConf(file, _sonar);
    file.close();
}

void SonarConfWindow::on_openAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open sonar configuration file"));
        return;
    }else{
        _sonar.resetSonar();
        if(_fileManager.readSonarConf(file, _sonar)){
            updateScene();
        }else
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a sonar configuration file!"));
    }

    file.close();
}

void SonarConfWindow::on_addAction()
{
    qDebug() << "add";
}

void SonarConfWindow::on_transAction()
{
    _signalConfWindow->show();
}

void SonarConfWindow::on_addPushButton_clicked()
{  
    uint id =_sonar.addReceiver();
    ui->SceneWidget->createReceiver(id);
    ui->SceneWidget->update();
    emit numRecChanged();
}

void SonarConfWindow::on_deletePushButton_clicked()
{
    if(_idUpdatedRec < 0 && _sonar.get_numOfRec() > 0){
        QMessageBox::critical(this, QStringLiteral("Delete receiver"), QStringLiteral("Select receiver to delete"));
        return;
    }

    if(!deleteReceiverMsg() || _idUpdatedRec < 0)
        return;

    if(_sonar.deleteReceiver(_idUpdatedRec)){
        ui->SceneWidget->deleteReceiver(_idUpdatedRec);
        ui->SceneWidget->update();
        updateRecId();
        emit numRecChanged();
    }
}

void SonarConfWindow::on_numRecChanged()
{
    ui->num_label->setText(QStringLiteral("Number of receivers: %1").arg(_sonar.get_numOfRec()));
    ui->idRecSpinBox->setMaximum(_sonar.get_numOfRec());
}

void SonarConfWindow::updateRecCord(double x_mm, double y_mm, double z_mm, double rot_deg)
{
    ui->SceneWidget->setRecPosition(_idUpdatedRec, QVector3D(x_mm, y_mm, z_mm)/SIMULATION_SCALE);
    ui->SceneWidget->setRecRotation(_idUpdatedRec, rot_deg);
    ui->SceneWidget->update();
}

void SonarConfWindow::on_idRecSpinBox_valueChanged(int arg1)
{
    if(_sonar.get_numOfRec() == 0 || arg1 == 0){
        _idUpdatedRec = -1;
        cordLabelUpdate(0.0, 0.0, 0.0, 0.0);
        ui->idRecSpinBox->setValue(0);
    }else{
        _idUpdatedRec = arg1-1;
        ui->SceneWidget->selectRec(_idUpdatedRec);
        QVector3D recCenterPosition = ui->SceneWidget->getRecPosition(_idUpdatedRec);
        double rot = ui->SceneWidget->getRecRotation(_idUpdatedRec);
        cordLabelUpdate(recCenterPosition[0], recCenterPosition[1], recCenterPosition[2], rot);
    }
}

void SonarConfWindow::on_xCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(arg1, ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void SonarConfWindow::on_yCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(ui->xCordSpinBox->value(), arg1, ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void SonarConfWindow::on_zCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), arg1, ui->rotSpinBox->value());
}

void SonarConfWindow::on_rotSpinBox_valueChanged(double arg1)
{
    updateRecCord(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), arg1);
}
void SonarConfWindow::on_recCenterPositionChanged(QVector3D position, double rot_deg)
{
    cordLabelUpdate(position[0], position[1], position[2], rot_deg);
}

void SonarConfWindow::on_sonarColor_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->SceneWidget->isSceneInitialized() < 0)
        return;

    for(uint i=0; i<7; ++i){
        if(arg1 == color[i]){
            ui->SceneWidget->setSonarColor(color_vec[i]);
        }
    }
}

void SonarConfWindow::onSonarSizeChanged()
{
    if(sonarSizeWindow->isSizeAccepted()){
        sonarSizeWindow->hide();
        ui->SceneWidget->changeSonarSize(sonarSizeWindow->getSonarSize()/SIMULATION_SCALE);
        ui->SceneWidget->update();
        sonarSizeWindow->setSizeAccepted(false);
    }
}

void SonarConfWindow::on_sizePushButton_clicked()
{
    sonarSizeWindow->show();
    sonarSizeWindow->updateLabel(ui->SceneWidget->getSonarScale()*SIMULATION_SCALE);
}

void SonarConfWindow::onSignalParamChanged()
{
    _signalConfWindow->getSignalParam(_sonar.useTransmittedSignal());
}

