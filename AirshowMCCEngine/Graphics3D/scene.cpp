#include "scene.h"
#include "codestd.h"
#include <ctime>
#include <iostream>
#include "platformcamera.h"

//void Scene::renderAlphaPass()
//{
//    D3DRendererAlphaPass11 alphaPass;
//    m_AlphaSceneNodes.sort();
//    while(!m_AlphaSceneNodes.empty())
//    {
//        AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
//        DXUTGetD3DDevice()->setTransform(D3DTS_WORLD, &((*i)->m_Concat));
//        (*i)->m_pNode->vRender(this);
//        delete(*i);
//        m_AlphaSceneNodes.pop_back();
//    }
//}

Scene::Scene()
{
    m_Root.reset(GCC_NEW RootNode());
    //m_LightManager = GCC_NEW LightManager;
}

Scene::~Scene()
{
    //SAFE_DELETE(m_LightManager);
}


HRESULT Scene::onRender()
{
    if(m_Root && m_Camera)
    {
        //The scene root could be anything, but it
        //is usually a SceneNode with the identity matrix
        m_Camera->setViewTransform(this);
        //m_LightManager->calcLighting(this);
        if(m_Root->vPreRender(this) == S_OK)
        {
            //cout<<"Scene::onRender -> if preRender"<<endl;
            m_Root->vRender(this);
            m_Root->vRenderChildren(this);
            m_Root->vPostRender(this);
        }
        //cout<<"end render scene"<<endl;
        //renderAlphaPass();
        return S_OK;
    }
}

HRESULT Scene::onRestore()
{
    if(!m_Root)
        return S_OK;
    return m_Root->vOnRestore(this);
}



HRESULT Scene::onUpdate(const int deltaMilliseconds)
{
    if(!m_Root)
        return S_OK;
//    static DWORD lastTime = timeGetTime();
//    DWORD elapsedTime = 0;
//    DWORD now = timeGetTime();
//    elapsedTime = now - lastTime;
//    lastTime = now;
//    return m_Root->vOnUpdate(this,elapsedTime);

    /* My addition for linux */
    static std::clock_t lastTime = std::clock();
    std::clock_t now = std::clock();

    float elapsedTime = (float)(now-lastTime)/(float)CLOCKS_PER_SEC;
    lastTime = now;
    m_Root->vOnUpdate(this,elapsedTime);
//    float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

//    float cameraSpeed = 1.0f;
//    if ( g_bShift )
//    {
//        cameraSpeed = 5.0f;
//    }
    //g_Camera.Translate( glm::vec3( g_D - g_A, g_Q - g_E, g_S - g_W ) * cameraSpeed * fDeltaTime );
//    g_fEarthRotation += fRotationRate1 * fDeltaTime;
//    g_fEarthRotation = fmod(g_fEarthRotation, 360.0f);

    return 1;
}

shared_ptr<ISceneNode> Scene::findActor(ActorId id)
{
    SceneActorMap::iterator i = m_ActorMap.find(id);
    if(i==m_ActorMap.end())
    {
        return shared_ptr<ISceneNode>();
    }
    return i->second;
}

void Scene::pushAndSetMatrix(const Mat4x4 &toWorld)
{
    m_MatrixStack.push(toWorld);
    //m_MatrixStack->MultMatrixLocal(&toWorld);
    //DXUTGetD3DDevice()->SetTransform(D3DTS_WORLD, m_MatrixStack->GetTop());
    //glCamera()->setTransform(m_MatrixStack.top());
    platformCamera::glCamera()->setTransform(m_MatrixStack.top());
}

void Scene::popMatrix()
{
    m_MatrixStack.pop();
    //DXUTGetD3DDevice()->SetTransform(D3DTS_WORLD, m_MatrixStack->GetTop());
    //glCamera()->setTransform(m_MatrixStack.top());
    platformCamera::glCamera()->setTransform(m_MatrixStack.top());
}

const Mat4x4 & Scene::getTopMatrix()
{
    return m_MatrixStack.top();
}


































