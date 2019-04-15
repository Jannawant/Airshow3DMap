#ifndef MATERIAL_H
#define MATERIAL_H

#include "codestd.h"
#include "ResourceCache/iresourceloader.h"
#include "texture.h"

//class Texture;
class Material
{
    Material3D m_3DMaterial;
public:
    Material();
    void setAmbient(const Color &color);
    const Color getAmbient(){return m_3DMaterial.ambient;}
    void setDiffuse(const Color &color);
    const Color getDiffuse(){return m_3DMaterial.diffuse;}
    void setSpecular(const Color &color, const float power);
    void getSpecular(Color &color, float &power)
    {color = m_3DMaterial.specular; power = m_3DMaterial.power;}
    void setEmissive(const Color &color);
    const Color getEmissive(){return m_3DMaterial.emissive;}
    void setAlpha(const float alpha);
    bool hasAlpha() const {return getAlpha() != fOPAQUE;}
    float getAlpha() const {return m_3DMaterial.diffuse.a;}
    void D3DUse();
};

class TextureResourceExtraData : public IResourceExtraData
{
    friend class TextureResourceLoader;
public:
    TextureResourceExtraData();
    virtual ~TextureResourceExtraData(){SAFE_DELETE(m_pTexture); /*SAFE_RELEASE(m_pSamplerLinear);*/}
    virtual std::string vToString() { return "TextureResourceExtraData" ; }
    Texture *  getTexture() {return m_pTexture;}
    void readTextureFromBuffer(char * rawBuffer, int size);
    //SamplerState * const * getSampler() {return &m_pSamplerLinear;}
protected:
    Texture *m_pTexture;
    //ShaderResourceView * m_pTexture;
    //SamplerState * m_pSamplerLinear;
};

class TextureResourceLoader: public IResourceLoader
{
public:
    TextureResourceLoader();
    virtual bool vUseRawFile() override {return false;}
    virtual bool vDiscardRawBufferAfterLoad() override {return  true;}
    virtual unsigned int vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override;
    virtual bool vLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) override;
};












































































#endif // MATERIAL_H
