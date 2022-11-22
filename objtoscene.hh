#ifndef OBJTOSCENE_HH
#define OBJTOSCENE_HH

#include <QDialog>
#include <QModelIndexList>

namespace Ui {
class ObjToScene;
}

class ObjToScene : public QDialog
{
    Q_OBJECT

public:
    explicit ObjToScene(QWidget *parent = nullptr);
    ~ObjToScene();

private slots:
    void on_listView_indexesMoved(const QModelIndexList &indexes);
    void on_platfromPushButton_clicked();
    void on_objectPushButton_clicked();

private:
    Ui::ObjToScene *ui;
};

#endif // OBJTOSCENE_HH
