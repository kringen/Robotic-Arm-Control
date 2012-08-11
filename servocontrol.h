#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <QWidget>

namespace Ui {
    class servoControl;
}

class servoControl : public QWidget
{
    Q_OBJECT

public:
    explicit servoControl(QWidget *parent = 0);
    ~servoControl();
    Ui::servoControl *ui;

public slots:
    void on_dialServoPos_valueChanged(int value);

/*private:
    Ui::servoControl *ui;*/
private slots:

    void on_pushButton_clicked();
};

#endif // SERVOCONTROL_H
