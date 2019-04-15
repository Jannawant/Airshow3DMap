#include "codestd.h"

Color g_White( 1.0f, 1.0f, 1.0f, fOPAQUE );
Color g_Black( 0.0f, 0.0f, 0.0f, fOPAQUE );
Color g_Cyan( 0.0f, 1.0f, 1.0f, fOPAQUE );
Color g_Red( 1.0f, 0.0f, 0.0f, fOPAQUE );
Color g_Green( 0.0f, 1.0f, 0.0f, fOPAQUE );
Color g_Blue( 0.0f, 0.0f, 1.0f, fOPAQUE );
Color g_Yellow( 1.0f, 1.0f, 0.0f, fOPAQUE );
Color g_Gray40( 0.4f, 0.4f, 0.4f, fOPAQUE );
Color g_Gray25( 0.25f, 0.25f, 0.25f, fOPAQUE );
Color g_Gray65( 0.65f, 0.65f, 0.65f, fOPAQUE );
Color g_Transparent (1.0f, 0.0f, 1.0f, fTRANSPARENT );

// Note! If you change these direction vectors around to suit yourself
// Make sure you change it so it retains a left handed
// coordinate system
Vec3 g_Right(1.0f, 0.0f, 0.0f);
Vec3 g_Up(0.0f, 1.0f, 0.0f);
Vec3 g_Forward(0.0f, 0.0f, 1.0f);


Vec4 g_Up4(g_Up.getVec3().x, g_Up.getVec3().y, g_Up.getVec3().z, 0.0f);
Vec4 g_Right4(g_Right.getVec3().x, g_Right.getVec3().y, g_Right.getVec3().z, 0.0f);
Vec4 g_Forward4(g_Forward.getVec3().x, g_Forward.getVec3().y, g_Forward.getVec3().z, 0.0f);


const float fOPAQUE = 1.0f;
const int iOPAQUE = 1;
const float fTRANSPARENT = 0.0f;
const int iTRANSPARENT = 0;

const int MEGABYTE = 1024 * 1024;
const float SIXTY_HERTZ = 16.66f;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


// [rez] Uncomment this #define to dump every call to new to the output window
//#define MEM_LOG
