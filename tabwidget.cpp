#include "tabwidget.h"
#include "ui_tabwidget.h"

tabWidget::tabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::tabWidget)
{
    ui->setupUi(this);
}

tabWidget::~tabWidget()
{
    delete ui;
}

void tabWidget::on_dial_1_valueChanged(int value)
{
    int pos = value;

    if(ui->dial_1->value() < 38)
    {
        ui->dial_1->setValue(38);
        pos = 0;
    }
    else if(ui->dial_1->value() > 138)
    {
        ui->dial_1->setValue(138);
        pos = 180;
    }
    else
    {
       //pos = value * (100 / 180)  ;
    }

    ui->spinBox_1->setValue((int)pos);
}
