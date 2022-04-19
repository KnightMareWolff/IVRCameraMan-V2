QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += _CRT_SECURE_NO_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#defines the output location
DESTDIR = $$PWD/../IVRTool/bin

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#OpenCV 4.5.3 - Compiled with MSVC2019 64Bits(With "d" for debug version)
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454d

INCLUDEPATH += $$PWD/../libs/ocv454/include
DEPENDPATH += $$PWD/../libs/ocv454/include

#FFMPEG API
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavcodec
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavutil
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavdevice
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavfilter
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavformat
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lswresample
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lswscale

INCLUDEPATH += $$PWD/../libs/ffmpeg/include
DEPENDPATH += $$PWD/../libs/ffmpeg/include

#Link with IVRLowLevelSDK Library - Please note here we are linking an Internal Library! (For Other Tools is an External Library)
win32: LIBS += -L$$PWD/../libs/IVRLowLevelSDK/ -lIVRLowLevelSDK

INCLUDEPATH += $$PWD/../libs/IVRLowLevelSDK
DEPENDPATH += $$PWD/../libs/IVRLowLevelSDK
