TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    fsl_egl.c \
    objloader.cpp \
    shader.cpp \
    geoparse.cpp \
    fslutil.cpp \
    camera.cpp \
    scene.cpp \
    main.cpp \
    tiny_obj_loader.cpp

#DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /home/mugen/GLES2.0/common/inc

#DEFINES +=  ARM
DEFINES += X86 GLM_ENABLE_EXPERIMENTAL
#QMAKE_CXXFLAGS += -pg -O2   #pg - gprof options
#QMAKE_CFLAGS += -pg
#QMAKE_LFLAGS += -pg
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
#QMAKE_CXXFLAGS  += -fprofile-arcs -ftest-coverage #gcov options
#QMAKE_LFLAGS += -lprofiler
#QMAKE_LFLAGS_DEBUG += -lprofiler
contains(DEFINES, ARM)
{
    LIBS += -lEGL -lGLESv2
}
contains(DEFINES, X86)
{
    DEFINES += EGL_USE_X11
    LIBS +=  -L/usr/lib/x86_64-linux-gnu/ -lEGL -lGLESv2  -lX11 -lSDL2_image -lSDL2 -lSOIL #-lpthread #-lprofiler #lprofiler - google-perftool
    #LIBS +=  -L./ libSOIL.a
}

HEADERS += \
    fsl_egl.h \
    fslutil.h \
    #SOIL.h
    objloader.h \
    shader.h \
    geoparse.h \
    camera.h \
    scene.h \
    tiny_obj_loader.h \
    packed_freelist.h \
    preamble.h

DISTFILES += \
    earth.frag \
    earth.vert

