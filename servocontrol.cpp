#include "servocontrol.h"
#include "ui_servocontrol.h"

servoControl::servoControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::servoControl)
{
    ui->setupUi(this);
    //Submit immediately upon value change
    connect(ui->spinBoxServoPos,SIGNAL(valueChanged(int)),ui->pushButton,SIGNAL(clicked()));
}

servoControl::~servoControl()
{
    delete ui;
}

void servoControl::on_dialServoPos_valueChanged(int value)
{
    this->ui->spinBoxServoPos->setValue(value);
}


void servoControl::on_pushButton_clicked()
{
   int currentPos = this->ui->spinBoxServoPos->value();
   this->ui->dialServoPos->setValue(currentPos);
}
