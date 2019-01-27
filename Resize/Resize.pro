#-------------------------------------------------
#
# Project created by QtCreator 2017-12-23T17:08:16
#
#-------------------------------------------------

#Remove widgets ?
QT       += core widgets
QT       += network


TARGET = Resize
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    socket.cpp \
    resizesettings.cpp \
	mainapplication.cpp

HEADERS += \
    socket.h \
    resizesettings.h \
	mainapplication.h
