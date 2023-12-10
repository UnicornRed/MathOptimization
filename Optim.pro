QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

QMAKE_LIBS += -lncurses
QMAKE_CXXFLAGS += -D GUI -g
QMAKE_TARGET = Optim
TARGET = Optim
OBJECTS_DIR = obj/

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/MathFunc.cpp \
    src/Optim.cpp \
    src/gui_optim.cpp \
    src/settings.cpp

HEADERS += \
    src/DiffStoper.h \
    src/MathFunc.h \
    src/OptMethod.h \
    src/Optimization.h \
    src/Point.h \
    src/gui_optim.h \
    src/settings.h \
    src/CursesOptim.h

FORMS += \
    src/gui_optim.ui \
    src/settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
