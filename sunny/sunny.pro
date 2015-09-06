TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    Mutex.h \
    Nocopyable.h \
    SunnyConfig.h \
    Thread.h \
    Condition.h \
    Socket.h \
    LogStream.h \
    ThreadPool.h

