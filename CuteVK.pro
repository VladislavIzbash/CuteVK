QT += core gui webengine webenginewidgets KWidgetsAddons

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/api/ApiClient.cpp \
    src/chat/MainWindow.cpp \
    src/chat/dialoglist/DialogEntryDelegate.cpp \
    src/chat/dialoglist/DialogListModel.cpp \
    src/chat/dialoglist/DialogListWidget.cpp \
    src/main.cpp

HEADERS += \
    src/api/ApiClient.h \
    src/chat/MainWindow.h \
    src/chat/dialoglist/DialogEntryDelegate.h \
    src/chat/dialoglist/DialogListModel.h \
    src/chat/dialoglist/DialogListWidget.h

FORMS += \
    src/chat/MainWindow.ui

TRANSLATIONS += \
    resources/translations/CuteVK_ru_RU.ts

RESOURCES += \
    resources.qrc

system(lrelease \"$$_PRO_FILE_\")

tr.commands = lupdate \"$$_PRO_FILE_\" && lrelease \"$$_PRO_FILE_\"
PRE_TARGETDEPS += tr
QMAKE_EXTRA_TARGETS += tr

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


