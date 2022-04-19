
TEMPLATE = lib
DEFINES += IVRLOWLEVELSDK_LIBRARY
DEFINES += _CRT_SECURE_NO_WARNINGS
CONFIG  += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#defines the output location
DESTDIR = $$PWD/../libs/IVRLowLevelSDK

SOURCES += \
    IVR_BackStageUI.cpp \
    IVR_CameraReader.cpp \
    IVR_CameraTake.cpp \
    IVR_ClassifierClass.cpp \
    IVR_ClassifierFeature.cpp \
    IVR_ClassifierProblem.cpp \
    IVR_Config.cpp \
    IVR_Frame.cpp \
    IVR_ImageClassifier.cpp \
    IVR_ImageFilter.cpp \
    IVR_ImageReader.cpp \
    IVR_LauncherUI.cpp \
    IVR_LowLevelSDK.cpp \
    IVR_MediaHandler.cpp \
    IVR_VideoReader.cpp \
    IVR_VirtualCamera.cpp

HEADERS += \
    IVRLowLevelSDK_global.h \
    IVR_BackStageUI.h \
    IVR_CameraReader.h \
    IVR_CameraTake.h \
    IVR_ClassifierClass.h \
    IVR_ClassifierFeature.h \
    IVR_ClassifierProblem.h \
    IVR_Config.h \
    IVR_Frame.h \
    IVR_ImageClassifier.h \
    IVR_ImageFilter.h \
    IVR_ImageReader.h \
    IVR_LauncherUI.h \
    IVR_LockFreeQueue.h \
    IVR_LowLevelSDK.h \
    IVR_MediaHandler.h \
    IVR_VideoReader.h \
    IVR_VirtualCamera.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

#OpenCV 4.5.3 - Compiled with MSVC2019 64Bits(With "d" for debug version)
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454d

INCLUDEPATH += $$PWD/../libs/ocv454/include
DEPENDPATH += $$PWD/../libs/ocv454/include

INCLUDEPATH += $$PWD/../libs/Qt/vc16
DEPENDPATH += $$PWD/../libs/Qt/vc16
#QtCore
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Cored
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Core

#QtGUI
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Guid
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Gui

#Qt Widgets
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgetsd
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgets

#Qt Concurrent
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrentd
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrent

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

QT += widgets



