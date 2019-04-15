#ifndef CODESTD_H
#define CODESTD_H

#include "Graphics3D/geometry.h"
#include <memory>
#include <tinyxml.h>
#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

struct Color{
    float r,g,b,a;
    Color(){}
    Color(float _r, float _g, float _b, float _a): r(_r), g(_g), b(_b), a(_a){}
};

struct Material3D
{
    Color diffuse;
    Color ambient;
    Color specular;
    Color emissive;
    float power;
};

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef uint64_t ComponentId;
const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;


typedef long HRESULT;
typedef unsigned short DWORD;
const HRESULT S_OK = 0;


extern Color g_White;
extern Color g_Black;
extern Color g_Cyan;
extern Color g_Red;
extern Color g_Green;
extern Color g_Blue;
extern Color g_Yellow;
extern Color g_Gray40;
extern Color g_Gray25;
extern Color g_Gray65;
extern Color g_Transparent;

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;

#if defined(_DEBUG)
#	define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define GCC_NEW new
#endif

// Useful #defines

extern const float fOPAQUE;
extern const int iOPAQUE;
extern const float fTRANSPARENT;
extern const int iTRANSPARENT;

extern const int MEGABYTE;
extern const float SIXTY_HERTZ;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//#define GCC_ERROR(str) do { (void)sizeof(str); } while(0)
//#define GCC_WARNING(str) do { (void)sizeof(str); } while(0)
//#define GCC_INFO(str) do { (void)sizeof(str); } while(0)
//#define GCC_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0)
//#define GCC_ASSERT(expr) do { (void)sizeof(expr); } while(0)

#define GCC_ERROR(str) assert((str , false))
//#define GCC_WARNING(str) assert(false && str)
//#define GCC_INFO(str) assert(false && str)
//#define GCC_LOG(tag, str) assert(false && str)
#define GCC_ASSERT(expr) assert(expr)

//#define UNUSED(expr) do {(void)expr;} while (0)
#define UNUSED __attribute__((unused))

#if !defined(FAILED)
    #define FAILED(x) (((HRESULT)(x))<0)
#endif

#define SUCCEEDED(x) (((HRESULT)(x)) >= 0)

#if !defined(V_RETURN)
    #define V_RETURN(x) (assert(x))
#endif

#if !defined(SAFE_DELETE)
    #define SAFE_DELETE(x) if(x) delete x; x=nullptr;
#endif

#if !defined(SAFE_DELETE_ARRAY)
    #define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=nullptr;
#endif

#if !defined(SAFE_RELEASE)
    #define SAFE_RELEASE(x) if(x) x->Release(); x=nullptr;
#endif

#ifdef UNICODE
    #define _tcssprintf wsprintf
    #define tcsplitpath _wsplitpath
#else
    #define _tcssprintf sprintf
    #define tcsplitpath _splitpath
#endif

//#define __STR2__(x) #x
//#define __STR1__(x) __STR2__(x)
//#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_GAMECODE4_2010_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_GAMECODE4_2010			107
#define IDI_SMALL				108
#define IDC_GAMECODE4_2010			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif
// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

#endif // CODESTD_H
