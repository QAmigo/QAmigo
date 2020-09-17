#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T22:40:12
#
#-------------------------------------------------

QT       += core gui serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QAmigo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += HOT_PLUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/AdvancedTab/protocal.cpp \
        src/AdvancedTab/protocaldataitem.cpp \
        src/AdvancedTab/protocalheaderitem.cpp \
        src/AdvancedTab/tabadvanced.cpp \
        src/AdvancedTab/datavisualizationgraph.cpp \
        src/AdvancedTab/decoder.cpp \
        src/AdvancedTab/nameallocator.cpp \
        src/AdvancedTab/textinput.cpp \
        src/AdvancedTab/vartype.cpp \
        src/mainwindow.cpp \
        src/main.cpp \
        src/SimpleTab/serialsendbox.cpp \
        src/SimpleTab/tabcomsimple.cpp \
        src/SimpleTab/transfercounter.cpp \
        src/AdvancedTab/texttranslator.cpp

HEADERS += \
        src/AdvancedTab/datavisualizationgraph.h \
        src/AdvancedTab/decoder.h \
        src/AdvancedTab/nameallocator.h \
        src/AdvancedTab/protocal.h \
        src/AdvancedTab/protocaldataitem.h \
        src/AdvancedTab/protocalheaderitem.h \
        src/AdvancedTab/tabadvanced.h \
        src/AdvancedTab/textinput.h \
        src/AdvancedTab/vartype.h \
        src/mainwindow.h \
        src/SimpleTab/serialsendbox.h \
        src/SimpleTab/tabcomsimple.h \
        src/SimpleTab/transfercounter.h \
        src/AdvancedTab/texttranslator.h \
        src/tabplugininterface.h

FORMS += \
        mainwindow.ui \
        src/AdvancedTab/textinput.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/icons.qrc

TRANSLATIONS = languages/Serial-Amigo_en.ts \
        languages/Serial-Amigo_zh.ts

contains(DEFINES, HOT_PLUG) {
        message('Compiling with hotplug HotPlug Supported...')

        win32: LIBS += -L$$PWD/lib/QDevicewatcher/win32 -lQDeviceWatcherd2
        unix:!macx: LIBS += -L$$PWD/lib/QDevicewatcher/linux/ -lQDeviceWatcher

        INCLUDEPATH += $$PWD/lib/QDevicewatcher/include
        DEPENDPATH += $$PWD/lib/QDevicewatcher
} else {
        message('Compiling without hotplug HotPlug Supported...')
}
