QT       += core gui opengl openglwidgets
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    objtoscene.cpp \
    platformmodel.cpp \
    qcustomplot/qcustomplot.cpp \
    sonarmodel.cpp \
    src/PlatformConf/platformconfscene.cpp \
    src/PlatformConf/platformconfwindow.cpp \
    src/Scene/camera.cpp \
    src/Scene/model.cpp \
    src/SonarConf/signalconfwindown.cpp \
    src/SonarConf/sonarconfscene.cpp \
    src/SonarConf/sonarconfwindow.cpp \
    src/changesizewindow.cpp \
    src/filemanager.cpp \
    src/main.cpp \
    src/mainscene.cpp \
    src/mainwindow.cpp \
    src/platform.cpp \
    src/receiver.cpp \
    src/sonar.cpp \
    src/transmittedsignal.cpp \
    src/transmitter.cpp \
    src/vector3D.cpp

HEADERS += \
    inc/PlatformConf/platformconfscene.hh \
    inc/PlatformConf/platformconfwindow.hh \
    inc/Scene/camera.hh \
    inc/Scene/model.hh \
    inc/SonarConf/signalconfwindown.hh \
    inc/SonarConf/sonarconfscene.hh \
    inc/SonarConf/sonarconfwindow.hh \
    inc/changesizewindow.hh \
    inc/filemanager.hh \
    inc/mainscene.hh \
    inc/mainwindow.hh \
    inc/platform.hh \
    inc/receiver.hh \
    inc/sonar.hh \
    inc/transmittedsignal.hh \
    inc/transmitter.hh \
    inc/vector3D.hh \
    objtoscene.hh \
    platformmodel.hh \
    qcustomplot/qcustomplot.h \
    sonarmodel.hh

FORMS += \
    forms/changesizewindow.ui \
    forms/mainwindow.ui \
    forms/signalconfwindown.ui \
    forms/sonarConfWindow.ui \
    forms/platformconfwindow.ui \
    objtoscene.ui

TRANSLATIONS += \
    translation/Symulator_sonaru_pl_PL.ts

INCLUDEPATH += \
    inc\
    inc/PlatformConf\
    inc/Scene\
    inc/SonarConf\
    src\
    src/PlatformConf\
    src/Scene\
    src/SonarConf\
    forms\
    translation\


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

STATECHARTS +=

DISTFILES += \
 shaders/color_shader.frag \
 shaders/color_shader.vert

RESOURCES += \
 res.qrc
