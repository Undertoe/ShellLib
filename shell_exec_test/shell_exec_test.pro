TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

unix:!macx: LIBS += -L$$OUT_PWD/../shellLib/ -lshellLib

INCLUDEPATH += $$PWD/../shellLib
DEPENDPATH += $$PWD/../shellLib

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../shellLib/libshellLib.a

LIBS += -lstdc++fs
