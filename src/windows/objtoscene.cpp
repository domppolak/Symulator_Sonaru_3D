#include "objtoscene.hh"
#include "ui_objtoscene.h"

ObjToScene::ObjToScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjToScene)
{
    ui->setupUi(this);
    _sizeWindow = new ChangeSizeWindow();
    ui->x_doubleSpinBox->setMaximum(10000);
    ui->x_doubleSpinBox->setMinimum(-10000);
    ui->y_doubleSpinBox->setMaximum(10000);
    ui->y_doubleSpinBox->setMinimum(-10000);
    ui->z_doubleSpinBox->setMaximum(10000);
    ui->z_doubleSpinBox->setMinimum(-10000);
}

ObjToScene::~ObjToScene()
{
    delete ui;
    delete _sizeWindow;
}

void ObjToScene::updateSizeCordLabel(double x, double y, double z)
{
    ui->x_doubleSpinBox->setValue(x);
    ui->y_doubleSpinBox->setValue(y);
    ui->z_doubleSpinBox->setValue(z);
}

void ObjToScene::updateSizeCordLabel(const QVector3D &cord)
{
    updateSizeCordLabel(cord[0], cord[1], cord[2]);
}

void ObjToScene::updateLabel(uint numOfObjects, const Objects &obj)
{
    QString textLabel;
    switch(obj)
    {
        case PLATFORM:
            textLabel = "Platform: " + QString::number(numOfObjects);
            break;
        case POINT:
            textLabel = "Point: " + QString::number(numOfObjects);
            break;
        case STAKE:
            textLabel = "Stake: " + QString::number(numOfObjects);
            break;
        case FLAT_SURFACE:
            textLabel = "Flat_Surface: " + QString::number(numOfObjects);
            break;
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    qDebug() << textLabel;
    ui->numOfObjLabel->setText(textLabel);
}

void ObjToScene::on_platfromPushButton_clicked()
{
    ui->sizePushButton->hide();
    _sizeWindow->setTitleDependOnObj(PLATFORM);
    emit objct2Add(PLATFORM);
}

void ObjToScene::on_pointPushButton_clicked()
{
    if(ui->sizePushButton->isHidden())
        ui->sizePushButton->show();

    _sizeWindow->setTitleDependOnObj(POINT);
    emit objct2Add(POINT);
}


void ObjToScene::on_flatSurfacePushButton_clicked()
{
    if(ui->sizePushButton->isHidden())
        ui->sizePushButton->show();

    _sizeWindow->setTitleDependOnObj(FLAT_SURFACE);
    emit objct2Add(FLAT_SURFACE);
}


void ObjToScene::on_stakePushButton_clicked()
{
    if(ui->sizePushButton->isHidden())
        ui->sizePushButton->show();

    _sizeWindow->setTitleDependOnObj(STAKE);
    emit objct2Add(STAKE);
}


void ObjToScene::on_sizePushButton_clicked()
{
    _sizeWindow->show();
}

