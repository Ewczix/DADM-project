QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    butterworth.cpp \
    ecg_baseline.cpp \
    filter.cpp \
    lms_filter.cpp \
    main.cpp \
    mainwindow.cpp \
    movingaverage.cpp \
    savitzkygolay.cpp

HEADERS += \
    butterworth.h \
    ecg_baseline.h \
    filter.h \
    lms_filter.h \
    mainwindow.h \
    movingaverage.h \
    savitzkygolay.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/eigen-3.4.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
