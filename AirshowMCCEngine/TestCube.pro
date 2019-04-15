TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    objloader.cpp \
    geoparse.cpp \
    camera.cpp \
    main.cpp \
    tiny_obj_loader.cpp \
    codestd.cpp \
    platformcamera.cpp \
    Actors/actorcomponent.cpp \
    Actors/rendercomponent.cpp \
    Actors/actorfactory.cpp \
    Actors/actor.cpp \
    Utilities/stringUtility.cpp \
    ResourceCache/iresourceloader.cpp \
    ResourceCache/xmlresource.cpp \
    ResourceCache/resourcecache.cpp \
    ResourceCache/zipfile.cpp \
    Platforms/fslutil.cpp \
    Platforms/fsl_egl.c \
    Platforms/fsllayer.cpp \
    Platforms/screen.cpp \
    Actors/transformcomponent.cpp \
    Graphics3D/geometry.cpp \
    Graphics3D/iscenenode.cpp \
    Graphics3D/meshnode.cpp \
    Graphics3D/scene.cpp \
    Graphics3D/scenenode.cpp \
    Graphics3D/shader.cpp \
    Graphics3D/material.cpp \
    Graphics3D/texture.cpp \
    MainLoop/process.cpp \
    MainLoop/processmanager.cpp \
    AppCode/mainapp.cpp \
    AppCode/applogic.cpp \
    Graphics3D/movementcontroller.cpp

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
    LIBS +=  -L/usr/lib/x86_64-linux-gnu/ -lEGL -lGLESv2  -lX11 -lSDL2_image -lSDL2 -lSOIL -ltinyxml -lz #-lpthread #-lprofiler #lprofiler - google-perftool
    #LIBS +=  -L./ libSOIL.a
}

HEADERS += \
    objloader.h \
    geoparse.h \
    camera.h \
    tiny_obj_loader.h \
    packed_freelist.h \
    preamble.h \
    codestd.h \
    platformcamera.h \
    Actors/actorcomponent.h \
    Actors/rendercomponentinterface.h \
    Actors/rendercomponent.h \
    Actors/actorfactory.h \
    Actors/actor.h \
    Utilities/templates.h \
    Utilities/stringUtility.h \
    ResourceCache/iresourceloader.h \
    ResourceCache/xmlresource.h \
    ResourceCache/resourcecache.h \
    ResourceCache/zipfile.h \
    Platforms/fsl_egl.h \
    Platforms/fslutil.h \
    Platforms/fsllayer.h \
    Platforms/screen.h \
    Actors/transformcomponent.h \
    Graphics3D/geometry.h \
    Graphics3D/iscenenode.h \
    Graphics3D/meshnode.h \
    Graphics3D/scene.h \
    Graphics3D/scenenode.h \
    Graphics3D/shader.h \
    Graphics3D/material.h \
    Graphics3D/texture.h \
    fslutil.h \
    SOIL.h \
    MainLoop/process.h \
    MainLoop/processmanager.h \
    AppCode/mainapp.h \
    AppCode/applogic.h \
    Graphics3D/movementcontroller.h \
    Utilities/types.h

DISTFILES += \
    earth.frag \
    earth.vert

