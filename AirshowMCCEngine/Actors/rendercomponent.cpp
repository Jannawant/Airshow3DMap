#include "rendercomponent.h"
#include "Utilities/templates.h"
#include "codestd.h"
#include "Graphics3D/meshnode.h"
#include "rendercomponent.h"
#include "transformcomponent.h"
#include "actor.h"
#include "AppCode/mainapp.h"
#include "Graphics3D/scene.h"
#include "Graphics3D/material.h"
#include "ResourceCache/resourcecache.h"

const char* MeshRenderComponent::g_Name = "MeshRenderComponent";
//const char* SphereRenderComponent::g_Name = "SphereRenderComponent";
//const char* TeapotRenderComponent::g_Name = "TeapotRenderComponent";
//const char* GridRenderComponent::g_Name = "GridRenderComponent";
//const char* LightRenderComponent::g_Name = "LightRenderComponent";
//const char* SkyRenderComponent::g_Name = "SkyRenderComponent";


bool BaseRenderComponent::vInit(TiXmlElement * pData)
{
    //color
    TiXmlElement * pColorNode = pData->FirstChildElement("Color");
    if(pColorNode)
        m_color = loadColor(pColorNode);
    return vDelegateInit(pData);
}

void BaseRenderComponent::vPostInit(void)
{
    shared_ptr<SceneNode> pSceneNode(vGetSceneNode());
    g_pApp->m_scene->addChild(1, pSceneNode);

    //shared_ptr<EvtData_New_Render_Component> pEvent(GCC_NEW EvtData_New_Render_Component(m_pOwner->getId(), pSceneNode));
    //IEventManager::Get()->vTriggerEvent(pEvent);
}

void BaseRenderComponent::vOnChanged(void)
{
    //shared_ptr<EvtData_Modified_Render_Component> pEvent(GCC_NEW EvtData_Modified_Render_Component(m_pOwner->getId()));
    //IEventManager::get()->vTriggerEvent(pEvent);
}

//TiXmlElement* BaseRenderComponent::VGenerateXml(void)
//{
//    TiXmlElement* pBaseElement = VCreateBaseElement();

//    // color
//    TiXmlElement* pColor = GCC_NEW TiXmlElement("Color");
//    pColor->SetAttribute("r", ToStr(m_color.r).c_str());
//    pColor->SetAttribute("g", ToStr(m_color.g).c_str());
//    pColor->SetAttribute("b", ToStr(m_color.b).c_str());
//    pColor->SetAttribute("a", ToStr(m_color.a).c_str());
//    pBaseElement->LinkEndChild(pColor);

//    // create XML for inherited classes
//    VCreateInheritedXmlElements(pBaseElement);

//    return pBaseElement;
//}

shared_ptr<SceneNode> BaseRenderComponent::vGetSceneNode(void)
{
    if(!m_pSceneNode)
        m_pSceneNode = vCreateSceneNode();
    return m_pSceneNode;
}


Color BaseRenderComponent::loadColor(TiXmlElement* pData)
{
    Color color;

    double r = 1.0;
    double g = 1.0;
    double b = 1.0;
    double a = 1.0;

    pData->Attribute("r", &r);
    pData->Attribute("g", &g);
    pData->Attribute("b", &b);
    pData->Attribute("a", &a);

    color.r = (float)r;
    color.g = (float)g;
    color.b = (float)b;
    color.a = (float)a;

    return color;
}

shared_ptr<SceneNode> MeshRenderComponent::vCreateSceneNode(void)
{
    cout<<"MeshRenderComponent vCreateSceneNode "<<endl;
//    return shared_ptr<SceneNode>();
    // get the transform component
        shared_ptr<TransformComponent> pTransformComponent = makeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));
        if (!pTransformComponent)
        {
            cout<<"No transform in MeshRenderComponent , error"<<endl;
            // can't render without a transform
            return shared_ptr<SceneNode>();
        }
        WeakBaseRenderComponentPtr wbrcp(this);
        cout<<"Get Tranform: "<<endl;
        Mat4x4 mat = pTransformComponent->getTransform();
        cout<<mat<<endl;
        shared_ptr<MeshNode> mesh(GCC_NEW MeshNode(m_pOwner->getId(),wbrcp,m_meshObjectResource.c_str(), RenderPass_Actor, &mat));

        //Get the texture
        Resource resTexture(m_textureResource);
        shared_ptr<ResHandle> pTextureHandle = g_pApp->m_resCache->getHandle(&resTexture);
        shared_ptr<TextureResourceExtraData> extra2 = static_pointer_cast<TextureResourceExtraData>(pTextureHandle->getExtra());
        auto p = extra2->getTexture();
        mesh->getShader().setDiffuseTexture(p);

        Texture *t2 = new Texture();
        t2->loadFromFile("Bumpmap.jpg",GL_TEXTURE_2D);
        mesh->getShader().setBumpTexture(t2);

        Texture *t3 = new Texture();
        t3->loadFromFile("Normalmap.jpg",GL_TEXTURE_2D);
        mesh->getShader().setNormalTexture(t3);

        return static_pointer_cast<SceneNode>(mesh);        //Static_pointer_cast add by me to avoid warning copy-move c++11
}

void MeshRenderComponent::vCreateInheritedXmlElements(TiXmlElement *)
{
    GCC_ERROR("MeshRenderComponent::vGenerateSubclassXml() not implemented");
}

bool MeshRenderComponent::vDelegateInit(TiXmlElement* pData)
{

    TiXmlElement* pTexture = pData->FirstChildElement("Texture");
    if (pTexture)
    {
        m_textureResource = pTexture->FirstChild()->Value();
        cout<<"vDelegateInit textureResource = "<<m_textureResource<<endl;
    }

    //"StaticMesh" -  name of the tag in actor xml file
    TiXmlElement* pStaticMesh = pData->FirstChildElement("StaticMesh");
    if (pStaticMesh)
    {
        m_meshObjectResource = pStaticMesh->FirstChild()->Value();
        cout<<"vDelegateInit meshResource = "<<m_meshObjectResource<<endl;
    }



    return true;
}









































