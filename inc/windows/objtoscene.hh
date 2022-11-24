#ifndef OBJTOSCENE_HH
#define OBJTOSCENE_HH

#include <QDialog>
#include <QModelIndexList>
#include "constant.hh"
#include "changesizewindow.hh"

namespace Ui {
class ObjToScene;
}

class ObjToScene : public QDialog
{
    Q_OBJECT

signals:
    void objct2Add(Objects const &obj);

public:
    explicit ObjToScene(QWidget *parent = nullptr);
    ~ObjToScene();
    void updateSizeCordLabel(double x, double y, double z);
    void updateSizeCordLabel(QVector3D const &cord);
    void updateLabel(uint numOfObjects, Objects const &obj);

private slots:
    void on_platfromPushButton_clicked();
    void on_pointPushButton_clicked();
    void on_flatSurfacePushButton_clicked();
    void on_stakePushButton_clicked();
    void on_sizePushButton_clicked();

private:
    Ui::ObjToScene *ui;
    ChangeSizeWindow *_sizeWindow;
};

#endif // OBJTOSCENE_HH
