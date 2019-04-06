//#include "SDL2/SDL_image.h"
//#include "SDL2/SDL.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EGL/egl.h"
#include "fsl_egl.h"
#include "fslutil.h"

#include <iostream>
#include <vector>

#include "shader.h"
#include "geoparse.h"

#include "camera.h"
#include "signal.h"
#include "ctime"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "scene.h"

using namespace std;


static Scene scene;
////static const int BYTES_PER_FLOAT = sizeof(float);

//static GLint g_hbaseMapLoc = 0;

static EGLDisplay egldisplay;
static EGLConfig eglconfig;
static EGLSurface eglsurface;
static EGLContext eglcontext;
static EGLNativeWindowType eglNativeWindow;
static EGLNativeDisplayType eglNativeDisplayType;

static float g_fEarthRotation = 1.0f;
////static std::clock_t g_PreviousTicks;
////static std::clock_t g_CurrentTicks;

static Shader shader("shader.vert", "shader.frag");

static Camera g_Camera;
static glm::vec3 g_InitialCameraPosition;
static glm::quat g_InitialCameraRotation;

#include <SOIL.h>
GLuint LoadTexture(const std::string& file)
{
    //glGenTextures( 1, texture );
    GLuint textureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if(!textureID)
    {
        cout<<"Failed load texture"<<endl;
        return 0;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}

void render()
{
    g_fEarthRotation -= 0.1;
//    g_Camera.Rotate(glm::quat(0.3,0.0,0.0,1.0));
//    g_Camera.ApplyViewMatrix();
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0), glm::radians(g_fEarthRotation), glm::vec3(0,1,0));// * glm::scale(glm::mat4(1.0), glm::vec3(1.0) );
    glm::vec4 eyePosW = glm::vec4( g_Camera.GetPosition(), 1 );
    //glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix();//* modelMatrix;
    glm::mat4 mv = g_Camera.GetViewMatrix()*modelMatrix;
    glm::mat4 proj = g_Camera.GetProjectionMatrix();

    //std::cout<<glm::to_string(mvp)<<endl;

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    shader.Use();

    //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matModelViewProj"), 1, GL_FALSE, glm::value_ptr(mvp) );
    glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matModelView"), 1, GL_FALSE, glm::value_ptr(mv) );
    glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matProj"), 1, GL_FALSE, glm::value_ptr(proj) );
    glUniformMatrix4fv( glGetUniformLocation(shader.m_program, "g_matModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
    glUniform4fv( glGetUniformLocation(shader.m_program, "g_cameraEye"), 1, glm::value_ptr( eyePosW ) );

//    glUniformMatrix4fv(glGetUniformLocation(shader.m_program,"g_matModelView"),1,0,matModelView);
//    glUniformMatrix4fv(glGetUniformLocation(shader.m_program, "g_matProj"), 1, 0, matProj);
//    glUniform3fv(glGetUniformLocation(shader.m_program, "g_cameraEye"),1,eyePos);
    //glUniform3fv(glGetUniformLocation(shader.m_program, "g_cameraLightPosition"),1, lightPos);
    //glUniform4fv(glGetUniformLocation(shader.m_program, "g_diffuseSpecularAmbientShininess"),1,difShin);

    for(uint32_t node : scene.nodes)
    {
        const SceneNode *instance = &scene.nodes[node];
        const Mesh *mesh = &scene.meshes[instance->meshId];
//        cout <<"mesh index count  = "<< mesh->indexCount <<endl;
//        cout << "mesh vertex count ="<<mesh->vertexCount<<endl;

        glBindBuffer(GL_ARRAY_BUFFER, mesh->positionBO);
        glVertexAttribPointer(SCENE_POSITION_ATTRIB_LOCATION,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),nullptr);
        glEnableVertexAttribArray(SCENE_POSITION_ATTRIB_LOCATION);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->texCoordBO);
        glVertexAttribPointer(SCENE_TEXCOORD_ATTRIB_LOCATION,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec2),nullptr);
        glEnableVertexAttribArray(SCENE_TEXCOORD_ATTRIB_LOCATION);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBO);
        glVertexAttribPointer(SCENE_NORMAL_ATTRIB_LOCATION,3,GL_FLOAT,GL_FALSE,sizeof(float),nullptr);
        glEnableVertexAttribArray(SCENE_NORMAL_ATTRIB_LOCATION);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBO);
        glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(mesh->indexCount),GL_UNSIGNED_INT, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }

    /////////////////////////////////*Draw aircraft*//////////////////////////////////////
    ///
    ///
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glVertexAttribPointer(shader.m_vertex, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE, 0 , nullptr);
//    glEnableVertexAttribArray( shader.m_vertex );
//    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(idxVertAircraft.size() * sizeof(glm::vec3)), idxVertAircraft.data(), GL_STATIC_DRAW);

//    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//    glVertexAttribPointer(shader.m_texCoord, TEXTURE_COORDINATE_DATA_SIZE, GL_FLOAT, 0, 0, nullptr);
//    glEnableVertexAttribArray(shader.m_texCoord);
//    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(idxUVAircraft.size() * sizeof(glm::vec2)), idxUVAircraft.data(), GL_STATIC_DRAW);

//    //glEnableVertexAttribArray(shader.m_normal);
//    //glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//    //glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexed_normals.size() * sizeof(glm::vec3)), idxNrmAircraft.data(), GL_STATIC_DRAW);
//    //glVertexAttribPointer(shader.m_normal,NORMAL_DATA_SIZE,GL_FLOAT,GL_FALSE, 0, nullptr);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indicesAircraft.size() * sizeof(unsigned short)), indicesAircraft.data() , GL_STATIC_DRAW);
//    glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(indicesAircraft.size()),GL_UNSIGNED_SHORT, nullptr);

    ////////////////////////*Draw Cities*///////////////////////////////////////////////////

    //glDrawArrays(GL_TRIANGLES,0,indices.size());
    //glDrawArrays(GL_TRIANGLES, 0, static_cast<int>((VertexPositions.size())));
    //glDrawArrays(GL_POINTS, static_cast<int>((VertexPositions.size()-15)/3),5);

    // Cleanup
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    glDisableVertexAttribArray( shader.m_vertex );
//    glDisableVertexAttribArray( shader.m_normal);
//    glDisableVertexAttribArray( shader.m_texCoord);
}

void initEGLSurface()
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

void closeEGLSurface(void)
{
    cout << "Cleaning up..."<<endl;
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


int init()
{
    g_InitialCameraPosition = glm::vec3( 0, 0, 3 );
    g_Camera.SetPosition( g_InitialCameraPosition );
    g_Camera.SetRotation( g_InitialCameraRotation );

    g_Camera.SetProjectionRH( 40.0f, 1920/static_cast<float>(1080), 0.1f, 200.0f );

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

    {
        GLuint g_EarthTexture;
        if(!(g_EarthTexture = LoadTexture("Albedo.jpg")))
            return 0;
        //g_hbaseMapLoc = glGetUniformLocation(shader.m_program, "s_texture");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_EarthTexture);

        //glUniform1i(g_hbaseMapLoc, 0);
        glEnable(GL_TEXTURE_2D);

    }
    return 1;
}

//static bool quit = false;

//void sighandler(int signal)
//{
//    cout<< "Caught signal , setting flaq to quit."<< signal<<endl;
//    quit = true;
//}


int main()
{
    initEGLSurface();
    scene.init();
    shader.init();
    std::vector<uint32_t> loadedMeshIDs;
    loadedMeshIDs.clear();
    loadMeshes("earth.obj",scene, loadedMeshIDs);
    for (uint32_t loadedMeshID : loadedMeshIDs)
    {
        uint32_t newInstanceID;
        AddInstance(scene, loadedMeshID, newInstanceID);

        // scale up the cube
        //uint32_t newTransformID = scene.nodes[newInstanceID].TransformID;
        //scene->Transforms[newTransformID].Scale = glm::vec3(2.0f);
    }

////    signal(SIGINT, sighandler);
////    signal(SIGTERM, sighandler);
    init();

//    //const GLubyte *glStr =  glGetString(GL_EXTENSIONS);

////    cout<<glGetString(GL_EXTENSIONS)<<endl<<glGetString(GL_ES_VERSION_2_0)
////        <<endl<<glGetString(GL_VERSION)<<glGetString(GL_VENDOR)<<glGetString(GL_RENDERER);
//    //SDL_Init (SDL_INIT_VIDEO);
//    //SDL_SetVideoMode (320, 200, 8, 0);

//    //Atom wmDeleteMessage = XInternAtom(eglNativeDisplayType, "WM_DELETE_WINDOW", False);
    EGLint width = 0;
    EGLint height = 0;
    bool quit = false;
    while(!quit)
    {
        XEvent e;
        while(XPending(eglNativeDisplayType))
        {
            cout << "xnext event"<<endl;
            XNextEvent(eglNativeDisplayType,&e);
        }
//        switch (e.type)
//        {
//            case ClientMessage:
//                if(e.xclient.data.l[0] == wmDeleteMessage)
//                {
//                    cout << "Destroy window" << endl;
//                    XDestroyWindow(eglNativeDisplayType,e.xclient.window);
//                    quit = true;
//                    break;
//                }
//                            cout << "etype = " << e.type << endl;
//            break;
//        }

        eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &width);
        eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &height);
        render();
        eglSwapBuffers(egldisplay, eglsurface);

    }
//    cout<<"end loop"<<endl;
//    XCloseDisplay(eglNativeDisplayType);
//    cout<<"deinit"<<endl;
    //deinit();
    return 0;
}
