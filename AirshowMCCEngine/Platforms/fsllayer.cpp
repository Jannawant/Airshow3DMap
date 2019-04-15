#include "fsllayer.h"
#include <assert.h>

//static EGLint g_width = 0;
//static EGLint g_height = 0;
//static EGLDisplay egldisplay;
//static EGLConfig eglconfig;
//static EGLSurface eglsurface;
//static EGLContext eglcontext;
//static EGLNativeWindowType eglNativeWindow;
//static EGLNativeDisplayType eglNativeDisplayType;

FslLayer::FslLayer()
{

}

void FslLayer::initEGLSurface()
{
    static const EGLint s_configAttribs[] =
    {
        EGL_RED_SIZE,   5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE,  5,
        EGL_ALPHA_SIZE, 0,
        EGL_SAMPLES,    0,
        EGL_NONE
    };

    EGLint numconfigs;
    eglNativeDisplayType = fsl_getNativeDisplay();
    egldisplay = eglGetDisplay(eglNativeDisplayType);
    eglInitialize(egldisplay, nullptr, nullptr);
    assert(eglGetError() == EGL_SUCCESS);
    eglBindAPI(EGL_OPENGL_ES_API);

    eglChooseConfig(egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
    assert(eglGetError() == EGL_SUCCESS);
    assert(numconfigs == 1);

    eglNativeWindow = fsl_createwindow( eglNativeDisplayType);
    assert(eglNativeWindow);

    eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, eglNativeWindow, nullptr);
    assert(eglGetError() == EGL_SUCCESS);

    EGLint ContextAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    eglcontext = eglCreateContext(egldisplay, eglconfig, nullptr, ContextAttribList);
    assert(eglGetError() == EGL_SUCCESS);
    eglMakeCurrent(egldisplay, eglsurface, eglsurface, eglcontext);
    assert(eglGetError() == EGL_SUCCESS);
}

void FslLayer::closeEGLSurface(void)
{
    //Cleanup();
    eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    assert(eglGetError() == EGL_SUCCESS);
    eglDestroyContext(egldisplay, eglcontext);
    eglDestroySurface(egldisplay, eglsurface);
    fsl_destroywindow(eglNativeWindow, eglNativeDisplayType);
    eglTerminate(egldisplay);
    assert(eglGetError() == EGL_SUCCESS);
    eglReleaseThread();
}

void FslLayer::swapBuffers()
{
    eglSwapBuffers(egldisplay, eglsurface);
}

EGLNativeWindowType FslLayer::getWindow() const
{
    return eglNativeWindow;
}

EGLSurface FslLayer::getSurface() const
{
    return eglsurface;
}

void FslLayer::getCursor(Point &point)
{
    getCursorPointer(eglNativeDisplayType, &point.x, &point.y);
}


EGLNativeDisplayType FslLayer::getDisplay() const
{
    return eglNativeDisplayType;
}
