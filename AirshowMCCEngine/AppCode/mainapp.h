#ifndef MAINAPP_H
#define MAINAPP_H

//#include "ResourceCache/resourcecache.h"
#include "string"
#include "applogic.h"

#include "ctime"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <time.h>
#include <numeric>
#include <chrono>
#include <thread>
#include "Platforms/screen.h"
#include <iostream>
#include "camera.h"

class ResCache;

using namespace  std;

typedef struct timespec timeStruct;

//void sighandler(int signal)
//{
//    cout<< "Caught signal , setting flaq to quit."<< signal<<endl;
//    quit = true;
//}


//class Timer
//{
//private:
//	// Псевдонимы типов используются для удобного доступа к вложенным типам
//	using clock_t = std::chrono::high_resolution_clock;
//	using second_t = std::chrono::duration<double, std::ratio<1> >;

//	std::chrono::time_point<clock_t> m_beg;

//public:
//	Timer() : m_beg(clock_t::now())
//	{
//	}

//	void reset()
//	{
//		m_beg = clock_t::now();
//	}

//	double elapsed() const
//	{
//		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
//	}
//};

class Timer
{
    bool isRunning = false;
    timeStruct m_start;
    timeStruct m_end;
public:
    void start()
    {
        clock_gettime(CLOCK_MONOTONIC, &m_start);
        isRunning = true;
    }
    void end()
    {
        clock_gettime(CLOCK_MONOTONIC,&m_end);
        isRunning = false;
    }
    static double getCurrentTimeMs()
    {
        timeStruct t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return (t.tv_sec*1e9 + t.tv_nsec)/1e6;
    }
    double endElapsedTime()
    {
        if(isRunning)
        {
            end();
            return ((m_end.tv_sec - m_start.tv_sec)*1e9 + (m_end.tv_nsec - m_start.tv_nsec))/1e6;
        }
        return 0;
    }
};

class MainApp
{
    //m_DesktopSize
    //m_ScreenSize
    bool m_quit;
    class ScreenSurface * m_screen;
    Camera *m_MainCamera;
public:
    MainApp();
    bool initInstance();
    std::string getAppTitle();
    std::string getAppDirectory();
    ScreenSurface * getScreen() {return m_screen;}
    void render();
    int pumpUntilMessage();
    //getInstance window
    //onDisplayChange
    //onClose

    //bool loadString(std::string language);

    //shared_ptr<IRenderer> m_renderer;
    //static Renderer getRendererImpl();

    //EventManager *m_eventManager;
    class ResCache *m_resCache;
    class Scene *m_scene; //add me
    class AppLogic *m_logic;
    AppLogic * getAppLogic() const { return m_logic;}
    Scene * getScene() const { return m_scene;}

};

extern MainApp * g_pApp;

#endif // MAINAPP_H
