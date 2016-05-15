#-------------------------------------------------
#
# Project created by QtCreator 2012-04-16T14:33:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lighting2
TEMPLATE = app


SOURCES += main.cpp \
    DsprParam.cpp \
    DsprController.cpp \
    QtpComp.cpp \
    QtpDiag.cpp \
    QtpMain.cpp \
    QtpPin.cpp \
    QtpWire.cpp \

HEADERS  += DsprParam.h \
    DsprController.h \
    QtpComp.h \
    QtpDiag.h \
    QtpMain.h \
    QtpPin.h \
    QtpWire.h \
    dspatch/DspCircuit.h \
    dspatch/DspCircuitThread.h \
    dspatch/DspComponent.h \
    dspatch/DspComponentThread.h \
    dspatch/DspParameter.h \
    dspatch/DspPlugin.h \
    dspatch/DspPluginLoader.h \
    dspatch/DspRunType.h \
    dspatch/DspSignal.h \
    dspatch/DspSignalBus.h \
    dspatch/DspThread.h \
    dspatch/DspThreadNull.h \
    dspatch/DspThreadUnix.h \
    dspatch/DspThreadWin.h \
    dspatch/DspWire.h \
    dspatch/DspWireBus.h \
    DSPatch.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/link/ -lDSPatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/link/ -lDSPatch
else:unix: LIBS += -L$$PWD/link/ -lDSPatch

INCLUDEPATH += $$PWD/link
DEPENDPATH += $$PWD/link

RESOURCES += \
    QtpRes.qrc
