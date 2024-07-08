QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Use relative paths for Python libraries and includes
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/python/libs -lpython38
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/python/libs -lpython38

INCLUDEPATH += $$PWD/python/include
DEPENDPATH += $$PWD/python/include

DISTFILES +=
