#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11T16:01:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drive
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialog.cpp \
    eventmanagement.cpp \
    mattoimageconversion.cpp \
    captureinbackground.cpp

HEADERS += \
        mainwindow.h \
    dialog.h \
    eventmanagement.h \
    mattoimageconversion.h \
    captureinbackground.h

FORMS += \
        mainwindow.ui \
    dialog.ui


LIBS += -mwindows
INCLUDEPATH += C:\opencv-build2\install\include
INCLUDEPATH += C:\boost_1_66_0\

LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_core320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_highgui320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_imgcodecs320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_imgproc320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_features2d320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_calib3d320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_ximgproc320.dll
LIBS += C:\opencv-build2\install\x64\mingw\bin\libopencv_videoio320.dll

RESOURCES += \
    resources.qrc
