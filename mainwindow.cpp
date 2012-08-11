#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_servocontrol.h"
#include "serialcom.h"
#include "hexspinbox.h"

SerialCom *sc = new SerialCom();
int byteCount = 10;
int positionByteArray[10];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HexSpinBox *startByte = new HexSpinBox();
    HexSpinBox *endByte = new HexSpinBox();
    QLabel *startLabel = new QLabel();
    startLabel->setAlignment(Qt::AlignRight);
    startLabel->setText("Start Byte:");
    QLabel *endLabel = new QLabel();
    endLabel->setAlignment(Qt::AlignRight);
    endLabel->setText("End Byte:");
    ui->horizontalLayoutServos->addWidget(startLabel);
    ui->horizontalLayoutServos->addWidget(startByte);
    connect(startByte,SIGNAL(valueChanged(int)),this,SLOT(updateStartByte(int)));
    startByte->setValue(0xC8);
    ui->horizontalLayoutServos->addWidget(endLabel);
    ui->horizontalLayoutServos->addWidget(endByte);
    connect(endByte,SIGNAL(valueChanged(int)),this,SLOT(updateEndByte(int)));
    endByte->setValue(0xD2);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectSerial(QString device, int baud)
{
     sc->Open(device, baud);
     connect(sc,SIGNAL(showReceived(QString)),this->ui->textEdit1, SLOT(append(QString)));
     connect(sc,SIGNAL(closeStatus(QString)),this->ui->textEdit1,SLOT(append(QString)));
     sc->startListen();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    removeDials();
    addDials(this->ui->spinBox->value());
    byteCount = this->ui->spinBox->value() + 2; // Add 2 for start and end byte
}

void MainWindow::addDials(int numDials)
{
    int row = 0;
    int column = 0;

    for (int i = 0; i<numDials; i++)
    {

        if(i%4 == 0 && i > 0)
        {
            row++;
            column = 0;
        }

        //Assign the servo name for referring to later on
        QString dialName = QString::number(i + 1);

        servoControl *dialInstance = new servoControl(this);
        dialInstance->ui->dialServoPos->setObjectName(dialName);
        dialInstance->ui->labelServoNumber->setText("Servo # " + dialName);

        // Set up signal and slot
        connect(dialInstance->ui->dialServoPos,SIGNAL(valueChanged(int)), this, SLOT(updatePositionArray(int)));
        // Set to initial 90
        dialInstance->ui->dialServoPos->setValue(90);
        // Create array for sending to serial
        positionByteArray[i+1] = dialInstance->ui->dialServoPos->value();
        // Add to main window QGridLayout
        this->ui->gridLayout_dials->addWidget(dialInstance,row,column);

        // Prepare the table
     //   this->ui->tableWidget->setRowCount(2);
        this->ui->tableWidget->setColumnCount(i + 1);
\
        // Set initial positions
        // dec
        this->ui->tableWidget->setItem(0,i,new QTableWidgetItem(QString::number(90)));
        // hex
        this->ui->tableWidget->setItem(1,i,new QTableWidgetItem(QString("%1").arg(90,0,16)));

        column++;
    }


}

void MainWindow::updateStartByte(int start)
{
    positionByteArray[0] = start;
}

void MainWindow::updateEndByte(int end)
{
    positionByteArray[8] = end;
    positionByteArray[9] = byteCount;
}

void MainWindow::updatePositionArray(int pos)
{
    QString senderName = "";
    if(QObject* dialObject = sender())
    {
        senderName = dialObject->objectName();
    }


    //Update table with dial value
    int servoNum = senderName.toInt(); // use toInt(16) for hex

    positionByteArray[servoNum] = pos; //QString("%1").arg(pos,0,16);

    // dec
    this->ui->tableWidget->setItem(0,servoNum - 1,new QTableWidgetItem(QString::number(pos)));
    // hex
    this->ui->tableWidget->setItem(1,servoNum - 1,new QTableWidgetItem(QString("%1").arg(pos,0,16)));

      sc->Write(positionByteArray,3);
      QString sentString = "<font color='red'>Sent:";
      for (int i=0; i < 10;i++)
      {
          sentString.append(QString("%1").arg(positionByteArray[i],0,16));
      }

      this->ui->textEdit1->append(sentString + "</font>");


}

void MainWindow::removeDials()
{
    int i = 0;
    QLayoutItem *child;
    while((child = this->ui->gridLayout_dials->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }
}

void MainWindow::sendSerial()
{

}

void MainWindow::on_btnSendSerial_clicked()
{
    sendSerial();
}

void MainWindow::on_pushButton_clicked()
{
    connectSerial(this->ui->cboPort->currentText(),this->ui->cboBaud->currentIndex());
}
