#-------------------------------------------------
#
# Project created by QtCreator 2018-11-10T12:32:15
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = live_stream_upload
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    datathread.cpp \
    rawdata.cpp \
    facefilter.cpp \
    bilateralfilter.cpp \
    mediaencode.cpp \
    sendtortmp.cpp \
    videocapture.cpp \
    audiorecord.cpp \
    controller.cpp


HEADERS  += widget.h \
    datathread.h \
    rawdata.h \
    facefilter.h \
    bilateralfilter.h \
    mediaencode.h \
    sendtortmp.h \
    videocapture.h \
    audiorecord.h \
    controller.h

INCLUDEPATH += ..\include

LIBS += G:\QT5.6.2\ffmpeg-3.2.2-win32-dev\lib\avformat.lib
LIBS += G:\QT5.6.2\ffmpeg-3.2.2-win32-dev\lib\avutil.lib
LIBS += G:\QT5.6.2\ffmpeg-3.2.2-win32-dev\lib\avcodec.lib
LIBS += G:\QT5.6.2\ffmpeg-3.2.2-win32-dev\lib\swscale.lib
LIBS += G:\QT5.6.2\ffmpeg-3.2.2-win32-dev\lib\swresample.lib

LIBS += G:\rmtp_live_stream_platform\live_stream_upload\lib\opencv_*.lib

FORMS    += widget.ui
