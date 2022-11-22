#include "objtoscene.hh"
#include "ui_objtoscene.h"

ObjToScene::ObjToScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjToScene)
{
    ui->setupUi(this);
}

ObjToScene::~ObjToScene()
{
    delete ui;
}

void ObjToScene::on_listView_indexesMoved(const QModelIndexList &indexes)
{

}


void ObjToScene::on_platfromPushButton_clicked()
{

}


void ObjToScene::on_objectPushButton_clicked()
{

}

