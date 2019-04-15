#ifndef XMLRESOURCE_H
#define XMLRESOURCE_H

#include <tinyxml.h>
#include "iresourceloader.h"
#include "resourcecache.h"
#include <memory>

class XmlResourceLoader : public IResourceLoader
{
public:
    virtual bool vUseRawFile() override {return false;}
    virtual bool vDiscardRawBufferAfterLoad() override {return true;}
    virtual unsigned int vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override {return rawSize;}
    virtual bool vLoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle) override;
    virtual std::string vGetPattern() override {return "*.xml";}
    static TiXmlElement* loadAndReturnRootXmlElement(const char* resourceString); //ResCache pointer added by me
};

std::shared_ptr<IResourceLoader> createXmlResourceLoader();

class XmlResourceExtraData : public IResourceExtraData
{
    TiXmlDocument m_xmlDocument;

public:
    virtual std::string vToString() { return "XmlResourceExtraData"; }
    void parseXml(char* pRawBuffer);
    TiXmlElement* getRoot(void) { return m_xmlDocument.RootElement(); }
};






































#endif // XMLRESOURCE_H
