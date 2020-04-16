CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++1z

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
    error.cpp \
    reflected_value.cc \
    shell_runtime.cpp

HEADERS += \
    ShellCallbacks.h \
    error.h \
    function_control.h \
    function_control_base.h \
    function_pointer.h \
    refl.hh \
    reflected_function_collection.h \
    reflected_functions.hh \
    reflected_functions_base.h \
    reflected_object.hh \
    reflected_object_base.h \
    reflected_object_collection.h \
    reflected_value.hh \
    shell_runtime.h \
    variable_control.h \
    variable_control_base.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Readme

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


LIBS += -lstdc++fs
