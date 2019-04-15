#include "screen.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

ScreenSurface::ScreenSurface()
{
    m_layer = nullptr;
}

ScreenSurface::~ScreenSurface()
{
    m_layer->closeEGLSurface();
    delete m_layer;
}

bool ScreenSurface::init()
{
    m_layer = new FslLayer();
    if(!m_layer)
        return false;
    m_layer->initEGLSurface();
    return true;
}

void ScreenSurface::swapBuffers()
{
    m_layer->swapBuffers();
}

void ScreenSurface::onChange()
{
    eglQuerySurface(m_layer->getDisplay(), m_layer->getSurface(), EGL_WIDTH, &m_width);
    eglQuerySurface(m_layer->getDisplay(), m_layer->getSurface(), EGL_WIDTH, &m_height);
}

void ScreenSurface::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}

void ScreenSurface::getCursor(Point &point)
{
    m_layer->getCursor(point);
}

EGLNativeDisplayType ScreenSurface::getDisplay()
{
    return m_layer->getDisplay();
}

EGLNativeWindowType ScreenSurface::getWindow()
{
    return m_layer->getWindow();
}
