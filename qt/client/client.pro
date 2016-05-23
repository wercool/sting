#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T10:44:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           serial/src/impl/list_ports/list_ports_linux.cc \
           serial/src/impl/unix.cc \
           serial/src/serial.cc \

HEADERS  += mainwindow.h \
            serial/include/serial/impl/unix.h \
            serial/include/serial/serial.h \
            serial/include/serial/v8stdint.h \

FORMS    += mainwindow.ui
