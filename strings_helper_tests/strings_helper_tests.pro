include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        tst_split.h

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../string_helpers/release/ -lstring_helpers
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../string_helpers/debug/ -lstring_helpers
else:unix: LIBS += -L$$OUT_PWD/../string_helpers/ -lstring_helpers

INCLUDEPATH += $$PWD/../string_helpers
DEPENDPATH += $$PWD/../string_helpers

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../string_helpers/release/libstring_helpers.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../string_helpers/debug/libstring_helpers.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../string_helpers/release/string_helpers.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../string_helpers/debug/string_helpers.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../string_helpers/libstring_helpers.a
