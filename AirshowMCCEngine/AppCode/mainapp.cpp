#include "mainapp.h"
#include "codestd.h"
#include "ResourceCache/resourcecache.h"
#include "ResourceCache/xmlresource.h"
#include "Graphics3D/scene.h"

MainApp* g_pApp = nullptr;

static int g_fps = 0;
const int MAX_FPS = 60;
const int MAX_FRAMESKIP = 10;
const double MS_PER_FRAME60 = 1000.0/60.0;
const double MS_PER_FRAME30 = 1000.0/30.0;


static timeStruct g_startTime;


float clocksToMs(clock_t ticks)
{
    return static_cast<float>(ticks) / static_cast<float>(CLOCKS_PER_SEC) * 1000;
}

double countFPS()
{
    static const int NUM_SAMPLES = 100;
    static double frameTimes[NUM_SAMPLES] = {0};
    static int currentFrame = 0;
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);
    double frameTimeMs = ((current_time.tv_sec - g_startTime.tv_sec)*1e9 + (current_time.tv_nsec - g_startTime.tv_nsec))/1000000.0;
    g_startTime = current_time;
    cout<<"Frame Time ="<<frameTimeMs <<endl;
    frameTimes[currentFrame % NUM_SAMPLES] = frameTimeMs;
    int count = 0;
    currentFrame++;
    if(currentFrame < NUM_SAMPLES){
        count = currentFrame;
    }
    else{
        count = NUM_SAMPLES;
    }
    float sum = 0;
    for(int i=0; i<NUM_SAMPLES;i++)
    {
        sum += frameTimes[i];
    }
    float frameTimeAverage = sum / (float)count;
    cout << "avFrameTime = " << frameTimeAverage<<endl;
    if(frameTimeAverage > 0){
        g_fps = 1000.0f / frameTimeAverage;
    }
    else{
        g_fps = 60.0f;
    }
    cout<<"FPS = "<<g_fps<<endl;
}

MainApp::MainApp()
{
    g_pApp = this;
    m_quit = false;
    m_resCache = nullptr;
    m_scene = nullptr; //add
    m_screen = nullptr;
}

bool MainApp::initInstance()
{
    m_screen = new ScreenSurface;
    if(!m_screen->init())
    {
        GCC_ERROR("Failed to initialize ScreenSurface!");
        return false;
    }
    IResourceFile *zipFile = GCC_NEW ResourceZipFile(L"Assets.zip");

    m_resCache = GCC_NEW ResCache(10, zipFile);
    if(!m_resCache)
    {
        GCC_ERROR("Failed to allocate resource cache!");
        return false;
    }
    if(!m_resCache->init())
    {
        GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
        return false;
    }
    extern shared_ptr<IResourceLoader> createXmlResourceLoader();
    extern shared_ptr<IResourceLoader> createMeshObjectResourceLoader();
    extern shared_ptr<IResourceLoader> createJPGResourceLoader();
    m_resCache->registerLoader(createXmlResourceLoader());
    m_resCache->registerLoader(createMeshObjectResourceLoader());
    m_resCache->registerLoader(createJPGResourceLoader());

    //add me
    m_scene = new Scene();
    if(!m_scene)
    {
        GCC_ERROR("MainApp::initInstance : Failed to create Scene");
        return false;
    }
    m_MainCamera = new Camera(1920,1080);
    m_MainCamera->setMoveSpeed(1.0);
    m_MainCamera->init(m_scene);


    m_logic = GCC_NEW AppLogic;
    if(!m_logic)
    {
        GCC_ERROR("MainApp::initInstance : Failed to create Logic");
        return false;
    }
    if(!m_logic->init())
    {
        GCC_ERROR("MainApp::initInstance : Failed to init Logic");
        return false;
    }

    return true;
}


void MainApp::render()
{
        //screen.onChange();
        g_pApp->m_screen->clear();
        //glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0), glm::radians(g_fEarthRotation), glm::vec3(0,1,0));// * glm::scale(glm::mat4(1.0), glm::vec3(1.0) );
        //glm::vec4 eyePosW = glm::vec4( g_Camera.GetPosition(), 1 );
        //glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix() ;
        //glm::mat4 mv = g_Camera.GetViewMatrix()*modelMatrix;
        //glm::mat4 proj = g_Camera.GetProjectionMatrix();

        //glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0), glm::radians(g_fEarthRotation), glm::vec3(0,1,0));// * glm::scale(glm::mat4(1.0), glm::vec3(1.0) );
        //glm::vec4 eyePosW = glm::vec4( g_Camera.GetPosition(), 1 );
        //glm::mat4 mv = cameraNode->getView().getMat4x4();
        //glm::mat4 mv = g_Camera.GetViewMatrix();

        //glm::mat4x4 mvp = cameraNode->getProjection().getMat4x4() * earthNode->vGet()->toWorld().getMat4x4();
        //glm::mat4 mvp = (cameraNode->getProjection()).getMat4x4() * cameraNode->getView().getMat4x4();
        //glm::mat4 mvp = (cameraNode->getProjection()).getMat4x4() * platformCamera::glCamera()->view();
        //cout<<endl<<endl<<endl;
        //glm::mat4 proj = cameraNode->getProjection().getMat4x4();

        //std::cout<<glm::to_string(mvp)<<endl;

    //    cout<<"view = "<<endl;
    //    printMat(platformCamera::glCamera()->view() );
    //    cout<<"proj = "<<endl;
    //    printMat(cameraNode->getProjection().getMat4x4());
    //    cout<<"mvp"<<endl;
    //    //printMat(cameraNode->getProjection().getMat4x4() * earthNode->vGet()->toWorld().getMat4x4());
    //    printMat(mvp);

    //    cout<<"view = ";
    //    printMat(g_Camera.GetViewMatrix());
    //    cout<<endl<<"proj = ";
    //    printMat(g_Camera.GetProjectionMatrix());
    //    cout<<endl<<"mvp = ";
    //    printMat(g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix());
        //printMat(mvp);



        //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matModelViewProj"), 1, GL_FALSE, glm::value_ptr(mvp) );
        //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matModelView"), 1, GL_FALSE, glm::value_ptr(mv) );
        //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matProj"), 1, GL_FALSE, glm::value_ptr(proj) );
        //glUniformMatrix4fv( glGetUniformLocation(shader.m_program, "g_matModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
        //glUniform4fv( glGetUniformLocation(shader.m_program, "g_cameraEye"), 1, glm::value_ptr( eyePosW ) );

    //    glUniformMatrix4fv(glGetUniformLocation(shader.m_program,"g_matModelView"),1,0,matModelView);
    //    glUniformMatrix4fv(glGetUniformLocation(shader.m_program, "g_matProj"), 1, 0, matProj);
    //    glUniform3fv(glGetUniformLocation(shader.m_program, "g_cameraEye"),1,eyePos);
        //glUniform3fv(glGetUniformLocation(shader.m_program, "g_cameraLightPosition"),1, lightPos);
        //glUniform4fv(glGetUniformLocation(shader.m_program, "g_diffuseSpecularAmbientShininess"),1,difShin);

    //    Mat4x4 t;
    //    t.setPosition(-Vec3(0,0,3));
    //    Mat4x4 r;
    //    r.buildRotationY(g_fEarthRotation);
    //    t  = t*r;
    //    earthNode->vSetTransform(&t);


        g_pApp->m_scene->onRender();
        //cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
            //earthNode->vRender(&scene);

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

        g_pApp->m_screen->swapBuffers();
//        previous = current;

}


int MainApp::pumpUntilMessage()
{
    g_pApp->m_screen->clear();
    bool quit = false;
    double startTime = Timer::getCurrentTimeMs();
    Timer updateTimer;
    updateTimer.start();
    static double deltaTime = 0;
    static int updates = 0;
    static int frames = 0;
    double frameMs = 0;
    while(!quit)
    {
        XEvent e;

        while(XPending(g_pApp->m_screen->getDisplay()))
        {
            cout << "xnext event"<<endl;
            XNextEvent(g_pApp->m_screen->getDisplay(),&e);
            cout << "etype = " << e.type << endl;
            switch (e.type)
            {
                case ClientMessage:
                {
                        Atom wmDeleteMessage = XInternAtom(m_screen->getDisplay(), "WM_DELETE_WINDOW", False);
                        if(e.xclient.data.l[0] == wmDeleteMessage)
                        {
                            cout << "Destroy window" << endl;
                            XDestroyWindow(g_pApp->m_screen->getDisplay(),e.xclient.window);
                            quit = true;
                            break;
                        }
                        break;
                }
                case KeyPress:
                    cout<<"Key press: "<< e.xkey.keycode<<" "<< XKeysymToString(XKeycodeToKeysym(m_screen->getDisplay(), e.xkey.keycode, 0))<<endl;

                    switch(XLookupKeysym(&e.xkey, 0))
                    {
                        case XK_A: cout<<"cout A"<<endl; break;
                        case XK_Up:
                        {
                            cout<<"UP UP"<<endl;
                            m_MainCamera->moveUp();
                            break;
                        }
                        case XK_Down:
                        {
                            cout<<"Down Down"<<endl;
                            m_MainCamera->moveDown();
                            break;
                        }
                        case XK_Left:
                        {
                            cout<<"Left Left"<<endl;
                            m_MainCamera->moveLeft();
                            break;
                        }
                        case XK_Right:
                        {
                            cout<<"Right Right"<<endl;
                            m_MainCamera->moveRight();
                            break;
                        }
                    }
                    break;
            }
        }

//        do
//        {
//            XNextEvent(g_pApp->m_screen->getDisplay(),&e);
//            switch (e.type)
//            {
//                case ClientMessage:
//                Atom wmDeleteMessage = XInternAtom(m_screen->getDisplay(), "WM_DELETE_WINDOW", False);
//                if(e.xclient.data.l[0] == wmDeleteMessage)
//                {
//                    cout << "Destroy window" << endl;
//                    XDestroyWindow(m_screen->getDisplay(),e.xclient.window);
//                    quit = true;
//                    break;
//                }
//                cout << "etype = " << e.type << endl;
//                break;
//            }
//        }while(XPending(g_pApp->m_screen->getDisplay()));


        frameMs = updateTimer.endElapsedTime();
        cout << "TIME elapsed " << frameMs<<endl;
        deltaTime += frameMs;
        //updateTimer.start();
        while(deltaTime >= MS_PER_FRAME60)
        {
            g_pApp->m_logic->vOnUpdate(Timer::getCurrentTimeMs(), frameMs);
            deltaTime -= MS_PER_FRAME60;
            updates++;
            //deltaTime = 0;
        }
        updateTimer.start();
        render();
        frames++;
        if(Timer::getCurrentTimeMs() - startTime > 1000)
        {
            startTime += 1000;
            cout << "Frames = "<<frames << " Updates = "<<updates<<endl;
            frames = 0;
            updates = 0;
        }
//        if(1000.0f / MAX_FPS > frameMs)
//        {
//            cout<<"sleep "<<1000.0f/MAX_FPS - frameMs<<endl;
//            std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000.0f/MAX_FPS - frameMs)));
//        }

        //countFPS();

    }
}

//bool MainApp::loadStrings(std::string language)
//{
//	std::string languageFile = "Strings\\";
//	languageFile += language;
//	languageFile += ".xml";

//	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(languageFile.c_str());
//	if (!pRoot)
//	{
//		GCC_ERROR("Strings are missing.");
//		return false;
//	}

//    // Loop through each child element and load the component
//    for (TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
//    {
//		const char *pKey=pElem->Attribute("id");
//		const char *pText=pElem->Attribute("value");
//		const char *pHotkey=pElem->Attribute("hotkey");
//		if (pKey && pText)
//		{
//			wchar_t wideKey[64];
//			wchar_t wideText[1024];
//			AnsiToWideCch(wideKey, pKey, 64);
//			AnsiToWideCch(wideText, pText, 1024);
//			m_textResource[std::wstring(wideKey)] = std::wstring(wideText);

//			if (pHotkey)
//			{
//				m_hotkeys[std::wstring(wideKey)] = MapCharToKeycode(*pHotkey);
//			}
//		}
//	}
//	return true;
//}
