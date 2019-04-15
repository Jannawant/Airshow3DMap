#include "xmlresource.h"
#include "codestd.h"
#include <memory>
#include <AppCode/mainapp.h>

void XmlResourceExtraData::parseXml(char *pRawBuffer)
{
    m_xmlDocument.Parse(pRawBuffer);
}

bool XmlResourceLoader::vLoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle)
{
    if(rawSize <= 0)
        return false;
    std::shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>(GCC_NEW XmlResourceExtraData());
    pExtraData->parseXml(rawBuffer);
    handle->setExtra(shared_ptr<XmlResourceExtraData>(pExtraData));
    return true;
}

TiXmlElement *XmlResourceLoader::loadAndReturnRootXmlElement(const char *resourceString)
{
    Resource resource(resourceString);
    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_resCache->getHandle(&resource); //this actually loads the XML file from the zip file
    shared_ptr<XmlResourceExtraData> pExtraData = std::static_pointer_cast<XmlResourceExtraData>(pResourceHandle->getExtra());
    return pExtraData->getRoot();
}

std::shared_ptr<IResourceLoader> createXmlResourceLoader()
{
    return std::shared_ptr<IResourceLoader>(GCC_NEW XmlResourceLoader());
}

































