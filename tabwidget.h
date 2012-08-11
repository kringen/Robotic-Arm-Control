#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

namespace Ui {
    class tabWidget;
}

class tabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit tabWidget(QWidget *parent = 0);
    ~tabWidget();

private slots:
    void on_dial_1_valueChanged(int value);

private:
    Ui::tabWidget *ui;
};

#endif // TABWIDGET_H
