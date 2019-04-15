#include <AppCode/mainapp.h>
#include "Graphics3D/texture.h"

using namespace std;

//static Camera g_Camera;
//static glm::vec3 g_InitialCameraPosition;
//static glm::quat g_InitialCameraRotation;

int init()
{

//    Mat4x4 b;
//    b.setPosition(-Vec3(0,0,3));
//    earthNode->vSetTransform(&b);

    //cameraNode->setTarget(earthNode);
    //scene.addChild(1, earthNode);
    //cout<< "SIZE = " << scene.m_Root->m_Children[RenderPass_Actor]-> << endl;

//    cout<<"setup shader"<<endl;
//    earthNode->setupShader("shader.vert", "shader.frag");
//    cout<<"getShader"<<endl;
//    earthNode->getShader().init();
//    cout<<"after getShader"<<endl;

//    g_InitialCameraPosition = glm::vec3( 0, 0, 3 );
//    g_Camera.SetPosition( g_InitialCameraPosition );
//    g_Camera.SetRotation( g_InitialCameraRotation );

//    g_Camera.SetProjectionRH( 40.0f, 1920/static_cast<float>(1080), 0.1f, 200.0f );

//    loadObj("earth.obj", VertexPositions, VertexTexCoords, VertexNormals);
//    indexVBO(VertexPositions, VertexTexCoords, VertexNormals, indices, indexed_vertices, indexed_uvs, indexed_normals);
//    VertexPositions.clear(); VertexNormals.clear(); VertexTexCoords.clear();
//    loadObj("Tu204.obj",VertexPositions, VertexTexCoords, VertexNormals);
//    indexVBO(VertexPositions, VertexTexCoords, VertexNormals, indicesAircraft, idxVertAircraft, idxUVAircraft, idxNrmAircraft);
//    cout << "indexed sizes = " << indexed_vertices.size() << " " << indexed_uvs.size() << " " << indexed_normals.size() << endl;

//    Point zero = geoPointFromLatLong(0,0,WorldSide::NORTH,0,0, WorldSide::EAST);
//    Point tow = geoPointFromLatLong(41,73, WorldSide::NORTH, 71,27,WorldSide::WEST );
//    Point hal = geoPointFromLatLong(20,0,WorldSide::SOUTH,50,0,WorldSide::EAST);
//    Point Beijing = geoPointFromLatLong(39,54,WorldSide::NORTH,116,24, WorldSide::EAST);
//    Point Perth = geoPointFromLatLong(31,57,WorldSide::SOUTH, 115,52, WorldSide::EAST);

//    VertexPositions.insert(VertexPositions.end(),{zero.x,zero.y,zero.z});
//    VertexPositions.insert(VertexPositions.end(),{tow.x,tow.y,tow.z});
//    VertexPositions.insert(VertexPositions.end(),{hal.x,hal.y,hal.z});
//    VertexPositions.insert(VertexPositions.end(),{Beijing.x,Beijing.y,Beijing.z});
//    VertexPositions.insert(VertexPositions.end(),{Perth.x,Perth.y,Perth.z});

    //Texture texture1("Albedo.jpg",GL_TEXTURE_2D, 2);

    return 1;
}

int main()
{
    MainApp *app = GCC_NEW MainApp;
    if(!app->initInstance())
    {
        GCC_ERROR("Cant init main app");
    }
    //XmlResourceLoader::loadAndReturnRootXmlElement("assets/actors/earth.xml");
    //XmlResourceLoader::loadAndReturnRootXmlElement("assets/actors/grid.xml",app.m_resCache);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    init();

    app->pumpUntilMessage();
////    signal(SIGINT, sighandler);
////    signal(SIGTERM, sighandler);
//    //const GLubyte *glStr =  glGetString(GL_EXTENSIONS);

////    cout<<glGetString(GL_EXTENSIONS)<<endl<<glGetString(GL_ES_VERSION_2_0)
////        <<endl<<glGetString(GL_VERSION)<<glGetString(GL_VENDOR)<<glGetString(GL_RENDERER);
//    //SDL_Init (SDL_INIT_VIDEO);
//    //SDL_SetVideoMode (320, 200, 8, 0);

//    //Atom wmDeleteMessage = XInternAtom(eglNativeDisplayType, "WM_DELETE_WINDOW", False);


//    cout<<"end loop"<<endl;
//    XCloseDisplay(eglNativeDisplayType);
//    cout<<"deinit"<<endl;
    //deinit();
    return 0;
}
