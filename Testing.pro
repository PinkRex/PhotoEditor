QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Helper.cpp \
    ImageView.cpp \
    UIInitializer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DrawingInterface.h \
    Helper.h \
    ImageView.h \
    PhotoEditorPluginInterface.h \
    UIInitializer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/x64/mingw/lib/ -llibopencv_highgui451.dll
win32: LIBS += -L$$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/x64/mingw/lib/ -llibopencv_imgproc451.dll
win32: LIBS += -L$$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/x64/mingw/lib/ -llibopencv_core451.dll
win32: LIBS += -L$$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/x64/mingw/lib/ -llibopencv_imgcodecs451.dll

INCLUDEPATH += $$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/include
DEPENDPATH += $$PWD/../../../Downloads/opencv4.5.1_MinGW/opencv4.5.1_MinGW/install/include
