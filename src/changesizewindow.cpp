#include "changesizewindow.hh"
#include "ui_changesizewindow.h"


ChangeSizeWindow::ChangeSizeWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeSizeWindow)
{
    ui->setupUi(this);
    ui->xSpinBox->setMinimum(0);
    ui->ySpinBox->setMinimum(0);
    ui->zSpinBox->setMinimum(0);
    ui->xSpinBox->setMaximum(10000);
    ui->ySpinBox->setMaximum(10000);
    ui->zSpinBox->setMaximum(10000);

    //buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     //| QDialogButtonBox::Cancel);

    //connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    //connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ChangeSizeWindow::~ChangeSizeWindow()
{
    delete ui;
}

void ChangeSizeWindow::updateLabel(QVector3D sonarSize)
{
    ui->xSpinBox->setValue(sonarSize[0]);
    ui->ySpinBox->setValue(sonarSize[1]);
    ui->zSpinBox->setValue(sonarSize[2]);
}

void ChangeSizeWindow::updateSonarSize(double x, double y, double z)
{
    _sonarSize = QVector3D(x,y,z);
}

void ChangeSizeWindow::on_buttonBox_accepted()
{
    updateSonarSize(ui->xSpinBox->value(), ui->ySpinBox->value(), ui->zSpinBox->value());
    sizeAccepted = true;
    emit sonarSizeChanged();
}

