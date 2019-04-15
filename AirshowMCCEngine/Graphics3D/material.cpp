#include "material.h"
#include "ResourceCache/resourcecache.h"

Material::Material()
{
    //ZeroMemory(&m_3DMaterial, sizeof(Material3D);
    m_3DMaterial.diffuse = g_White;
    m_3DMaterial.ambient = Color(0.10f, 0.10f, 0.10f, 1.0f);
    m_3DMaterial.specular = g_White;
    m_3DMaterial.emissive = g_Black;
}

void Material::setAmbient(const Color &color)
{
    m_3DMaterial.ambient = color;
}

void Material::setDiffuse(const Color &color)
{
    m_3DMaterial.diffuse = color;
}

void Material::setSpecular(const Color &color, const float power)
{
    m_3DMaterial.specular = color;
    m_3DMaterial.power = power;
}

void Material::setEmissive(const Color &color)
{
    m_3DMaterial.emissive = color;
}

void Material::setAlpha(const float alpha)
{
    m_3DMaterial.diffuse.a = alpha;
}

void Material::D3DUse()
{
    //D3DDevice()->setMaterial(&m_d3dMaterial);
}


class JpgResourceLoader : public TextureResourceLoader
{
public:
    virtual std::string vGetPattern() override { return "*.jpg"; }
    virtual ~JpgResourceLoader(){}
};

shared_ptr<IResourceLoader> createJPGResourceLoader()
{
    cout<<"Create JPG Resource Loader"<<endl;
    return shared_ptr<IResourceLoader>(GCC_NEW JpgResourceLoader());
}

TextureResourceExtraData::TextureResourceExtraData()
{

    //m_pTexture = NULL;
    //m_pSamplerLinear = NULL;
}

void TextureResourceExtraData::readTextureFromBuffer(char *rawBuffer, int size)
{
    m_pTexture = new Texture();
    m_pTexture->loadFromBuffer(rawBuffer, size);
    //m_pTexture->bind();
}

TextureResourceLoader::TextureResourceLoader()
{
    cout<<"TextureResourceLoader constructor"<<endl;
}

unsigned int TextureResourceLoader::vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    //This will keep the resource cache from allocating memory for the texture, so OpenGL can manage it on its own
    return 0;
}

bool TextureResourceLoader::vLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
    cout<<"TextureResourceLoader::vLoadResource"<<endl;
    shared_ptr<TextureResourceExtraData> pExtraData = shared_ptr<TextureResourceExtraData>
            (GCC_NEW TextureResourceExtraData());
    pExtraData->readTextureFromBuffer(rawBuffer,rawSize);
    handle->setExtra(shared_ptr<TextureResourceExtraData>(pExtraData));
    return true;

    //Load the Texture
    //if(FAILED(createShaderResourceViewFromMemory(D3dGetDevice(), rawBuffer, rawSize, NULL, NULL, &extra->m_pTexture, NULL)))
        //return false;

    // Create the sample state
//    D3D11_SAMPLER_DESC sampDesc;
//    ZeroMemory( &sampDesc, sizeof(sampDesc) );
//    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//    sampDesc.MinLOD = 0;
//    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//    if( FAILED( DXUTGetD3D11Device()->CreateSamplerState( &sampDesc, &extra->m_pSamplerLinear ) ) )
//        return false;

//    handle->SetExtra(shared_ptr<D3DTextureResourceExtraData11>(extra));
//    return true;
//    GCC_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
//    return false;
//    }
}







