#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T14:49:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaserCameraCalibration
TEMPLATE = app

INCLUDEPATH += /usr/include \
               /usr/include/pcl-1.7 \
               /usr/include/eigen3 \
               /usr/include/opencv \
               /usr/include/opencv2 \
               /usr/include/boost \
               /usr/include/vtk-5.8


LIBS += -L/usr/lib \
        -L/usr/lib/x86_64-linux-gnu\
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lpcl_common \
        -lpcl_io \
        -lpcl_visualization \
        -lvtkCommon \
        -lvtkFiltering \
        -lvtkWidgets \
        -lvtkHybrid \
        -lvtkCharts \
        -lvtkRendering \
        -lvtkImaging \
        -lvtkGraphics \
        -lvtkIO \
        -lboost_system \
        -lboost_thread

SOURCES += main.cpp\
        mainwindow.cpp \
        capture.cpp

HEADERS  += mainwindow.h \
        capture.h

FORMS    += mainwindow.ui
