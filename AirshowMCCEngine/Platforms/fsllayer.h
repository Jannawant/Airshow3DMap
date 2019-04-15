#ifndef FSLLAYER_H
#define FSLLAYER_H

#include "fsl_egl.h"
#include "Utilities/types.h"

class FslLayer
{
public:
    FslLayer();
    void initEGLSurface();
    void closeEGLSurface(void);
    void swapBuffers();
    EGLNativeDisplayType getDisplay() const;
    EGLNativeWindowType getWindow() const;
    EGLSurface getSurface() const;
    void getCursor(Point &point);
private:
    EGLint g_width = 0;
    EGLint g_height = 0;
    EGLDisplay egldisplay;
    EGLConfig eglconfig;
    EGLSurface eglsurface;
    EGLContext eglcontext;
    EGLNativeWindowType eglNativeWindow;
    EGLNativeDisplayType eglNativeDisplayType;
};

#endif // FSLLAYER_H
