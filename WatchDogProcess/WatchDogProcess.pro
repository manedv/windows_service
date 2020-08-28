#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T18:54:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WatchDogProcess
TEMPLATE = app

## QSingleApplication
include($$PWD/QSingleApplication/QtSingleApplication.pri)

macx{
    QT += macextras


QMAKE_LFLAGS += -F/System/Library/Frameworks
LIBS += -dead_strip -framework IOKit -framework CoreFoundation -framework Carbon -framework Cocoa -framework Security
LIBS += -lobjc

#SOURCES += main.cpp\
#    proctoringwatchdog.cpp \
#    proctortrackthread.cpp

HEADERS  +=\
    LetsMoveAppDelegate.h \
    PFMoveApplication.h \
    PFMoveApplication.m \
    cocoaHelpers.h

#FORMS    += proctoringwatchdog.ui


OBJECTIVE_SOURCES += \
       LetsMoveAppDelegate.m \
       PFMoveApplication.m \
       cocoaHelpers.mm

DISTFILES += \
    LetsMoveAppDelegate.m
}


win32{

include(service_src/qtservice.pri)

win32-g++:LIBS += libUser32
win32-msvc*:LIBS += User32.lib
}

#Comman files for mac and win
SOURCES += main.cpp\
    proctortrackthread.cpp \
    proctoringwatchdog.cpp \
    WaitingScreen.cpp

HEADERS  += \
    proctortrackthread.h \
    proctoringwatchdog.h \
    WaitingScreen.h

FORMS    +=    proctoringwatchdog.ui \
                WaitingScreen.ui

RESOURCES += \
    res.qrc








