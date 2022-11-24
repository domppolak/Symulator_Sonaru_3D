#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QAction>

#include "sonarconfwindow.hh"
#include "platformconfwindow.hh"
#include "objtoscene.hh"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setMenuBar();
private slots:
    void on_saveAction();
    void on_sonarAction();
    void on_platformAction();
    void on_transmiterFigureAction();
    void on_receiverFiguresAction();

    void on_pushButton_3_clicked();

    void on_startSimPushButton_clicked();

    void on_stopSimPushButton_clicked();

    void on_deletePushButton_clicked();

    void on_objc2Add(Objects const &obj);
private:
    Ui::MainWindow *ui;
    SonarConfWindow *_sonarConfWindow;
    PlatformConfWindow *_platformConfWindow;
    ObjToScene *_addObjToScene;

    QMenu *figuresMenu;
    QMenu *receiverMenu;

    Platform _platform;
};
#endif // MAINWINDOW_HH
