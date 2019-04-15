#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "rendercomponentinterface.h"

class BaseRenderComponent : public RenderComponentInterface
{
protected:
    Color m_color;
    shared_ptr<SceneNode> m_pSceneNode;
public:
    virtual bool vInit(TiXmlElement* pData) override;
    virtual void vPostInit(void) override;
    virtual void vOnChanged(void) override;
    //virtual TiXmlElement* vGenerateXml(void) override;
    const Color getColor() const { return m_color;}
protected:
    //loads the SceneNode specific data(represented in the <SceneNode> tag)
    virtual bool vDelegateInit(TiXmlElement* pData) { return true;}
    virtual shared_ptr<SceneNode> vCreateSceneNode(void) = 0; // factory method to create the appropriate scene node
    Color loadColor(TiXmlElement * pData);
    //editor stuff
    virtual TiXmlElement* vCreateBaseElement(void) { return GCC_NEW TiXmlElement(vGetName());}
    virtual void vCreateInheritedXmlElements(TiXmlElement* pBaseElement) = 0;
private:
    virtual shared_ptr<SceneNode> vGetSceneNode(void) override;
};

class MeshRenderComponent: public BaseRenderComponent
{
    std::string m_meshObjectResource;
    std::string m_textureResource;
public:
    static const char *g_Name;
    virtual const char *vGetName() const override {return g_Name;}

protected:
    virtual shared_ptr<SceneNode> vCreateSceneNode(void) override; //factory method to create the appropriate scene node
    virtual void vCreateInheritedXmlElements(TiXmlElement * pBaseElement) override;
    bool vDelegateInit(TiXmlElement* pData) override;

};


























#endif // RENDERCOMPONENT_H
