QT       += core gui
QT       += opengl
QT       += openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera_setting.cpp \
    cell_color.cpp \
    color_scheme.cpp \
    global_variables.cpp \
    gscv_import_wizard.cpp \
    main.cpp \
    mainwindow.cpp \
    opengl_view_widget.cpp \
    palette.cpp \
    sc_packer.cpp \
    sc_reader.cpp \
    settings.cpp \
    settings_dialog.cpp \
    statistic.cpp \
    utils.cpp \
    language_manager.cpp

HEADERS += \
    camera_setting.h \
    cell_color.h \
    color_scheme.h \
    global_variables.h \
    gscv_import_wizard.h \
    mainwindow.h \
    opengl_view_widget.h \
    palette.h \
    sc_packer.h \
    sc_reader.h \
    settings_dialog.h \
    settings.h \
    statistic.h \
    utils.h \
    language_manager.h

FORMS += \
    camera_setting.ui \
    cell_color.ui \
    color_scheme.ui \
    mainwindow.ui \
    settings_dialog.ui \
    statistic.ui

TRANSLATIONS += \
    language/zh_CN.ts \
    language/en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += include
LIBS += -lz

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
