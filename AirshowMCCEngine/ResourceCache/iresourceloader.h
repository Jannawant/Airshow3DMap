#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H

#include <string>
#include <memory>

class Resource;
class ResHandle;

class IResourceLoader
{
public:
    virtual std::string vGetPattern() = 0;
    virtual bool vUseRawFile()=0;
    virtual bool vDiscardRawBufferAfterLoad()=0;
    virtual bool vAddNullZero(){return false;}
    virtual unsigned int vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
    virtual bool vLoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle) = 0;
};

class IResourceFile
{
public:
    virtual bool vOpen() = 0;
    virtual int vGetRawResourceSize(const Resource &r)=0;
    virtual int vGetRawResource(const Resource &r, char *buffer) = 0;
    virtual int vGetNumResources() const = 0;
    virtual std::string vGetResourceName(int num) const = 0;
    virtual bool vIsUsingDevelopmentDirectories(void) const = 0;
    virtual ~IResourceFile(){}
};


class IResourceExtraData
{
public:
    virtual std::string vToString()=0;
    ~IResourceExtraData(){}
};




































#endif // IRESOURCELOADER_H
