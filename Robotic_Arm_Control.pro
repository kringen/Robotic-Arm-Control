#-------------------------------------------------
#
# Project created by QtCreator 2012-03-24T22:55:49
#
#-------------------------------------------------

QT       += core gui

TARGET = RoboticArmControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    servocontrol.cpp \
    serialcom.cpp \
    hexspinbox.cpp

HEADERS  += mainwindow.h \
    servocontrol.h \
    serialcom.h \
    hexspinbox.h

FORMS    += mainwindow.ui \
    servocontrol.ui
