#ifndef SCREEN_H
#define SCREEN_H

#include "fsllayer.h"

class ScreenSurface
{
    int m_width;
    int m_height;
    FslLayer *m_layer;
public:
    ScreenSurface();
    ~ScreenSurface();
    bool init();
    void swapBuffers();
    void onChange();
    void clear();
    void getCursor(Point &point);
    EGLNativeDisplayType getDisplay();
    EGLNativeWindowType getWindow();
    FslLayer * getFslLayer(){return m_layer;}
    void close();
};

#endif // SCREEN_H
