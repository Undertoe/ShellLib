TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../shellLib/release/ -lshellLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../shellLib/debug/ -lshellLib
else:unix: LIBS += -L$$OUT_PWD/../shellLib/ -lshellLib

INCLUDEPATH += $$PWD/../shellLib
DEPENDPATH += $$PWD/../shellLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../shellLib/release/libshellLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../shellLib/debug/libshellLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../shellLib/release/shellLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../shellLib/debug/shellLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../shellLib/libshellLib.a

LIBS += -lstdc++fs
