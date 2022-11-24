#ifndef PLATFORMCONFWINDOW_HH
#define PLATFORMCONFWINDOW_HH

#include <QDialog>
#include <memory>
#include "filemanager.hh"
#include "platform.hh"
#include "changesizewindow.hh"

namespace Ui {
class PlatformConfWindow;
}

class PlatformConfWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlatformConfWindow(QWidget *parent = nullptr);
    ~PlatformConfWindow();

private:
    void codLabelUpdate(double x, double y, double z, double rot);
    void setConnections();
    void setToolBar();
    void updateScene();
    void onPlatformSizeChanged();
    void setSonarPosition(double x_mm, double y_mm, double z_mm, double newRotation);
    bool deleteSonarMsg();
    void updateSonarId();

private slots:
    void on_loadSonarPushButton_clicked();
    void on_addSonarPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_xCordSpinBox_valueChanged(double arg1);
    void on_yCordSpinBox_valueChanged(double arg1);
    void on_zCordSpinBox_valueChanged(double arg1);
    void on_rotSpinBox_valueChanged(double arg1);
    void on_sizePushButton_clicked();
    void on_idSonarSpinBox_valueChanged(int arg1);

    void on_saveAction();
    void on_openAction();
    void on_addAction();

    void on_sonarPositionChanged(QVector3D position, double rotation);
    void SonarNumChanged();


signals:
    void sonarCordChanged(double x_mm, double y_mm, double z_mm, double rot_deg);

private:
    Ui::PlatformConfWindow *ui;
    ChangeSizeWindow *sizeWindow;

    Platform _platform;
    FileManager _fileManager;
    int _idUpdatedSonar = -1;
    int _sonarIsLoaded = -1;

    Sonar _loadedSonar;

    static constexpr uint SIMULATION_SCALE = 10;
};

#endif // PLATFORMCONFWINDOW_HH
