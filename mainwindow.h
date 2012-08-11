#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QTimer>
#include "servocontrol.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();

private slots:
    void on_spinBox_valueChanged(int arg1);
    void updatePositionArray(int pos);
    void sendSerial();
    void connectSerial(QString device, int baud);

    void on_btnSendSerial_clicked();

    void on_pushButton_clicked();
    void updateStartByte(int start);
    void updateEndByte(int end);

private:
    Ui::MainWindow *ui;
    void addDials(int numDials);
    void removeDials();

};

#endif // MAINWINDOW_H
