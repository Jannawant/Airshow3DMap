#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

#include <string>
#include <memory>
#include <list>
#include <map>
#include <vector>
#include "iresourceloader.h"
#include "codestd.h"
#include "zipfile.h"

using std::list;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

class ResHandle;
class ResCache;

class Resource
{
public:
    string m_name;
    Resource(const string &name);
    Resource();
};

class ResHandle
{
    friend class ResCache;
protected:
    Resource m_resource;
    char *m_buffer;
    unsigned int m_size;
    shared_ptr<IResourceExtraData> m_extra;
    ResCache *m_pResCache;
public:
    ResHandle(Resource &resource, char *buffer, unsigned int size, ResCache *pResCache);
    virtual ~ResHandle();
    const string getName(){return m_resource.m_name;}
    unsigned int size() const {return m_size;}
    char *buffer() const {return m_buffer;}
    char *writableBuffer(){return m_buffer;}
    shared_ptr<IResourceExtraData> getExtra(){return m_extra;}
    void setExtra(shared_ptr<IResourceExtraData> extra){m_extra = extra;}
};

typedef list<shared_ptr<ResHandle> > ResHandleList;					// lru list
typedef map<std::string, shared_ptr<ResHandle> > ResHandleMap;		// maps indentifiers to resource data
typedef list<shared_ptr<IResourceLoader> > ResourceLoaders;

class ResCache
{
    friend class ResHandle;
    ResHandleList m_lru;
    ResHandleMap m_resources;
    ResourceLoaders m_resourceLoaders;
    //Main zip file for resource cache
    IResourceFile *m_file;
    unsigned int m_cacheSize;   //total memory size
    unsigned int m_allocated;    //total memory allocated
protected:
    bool makeRoom(unsigned int size);
    char *allocate(unsigned int size);
    void free(shared_ptr<ResHandle> gonner);

    shared_ptr<ResHandle> load(Resource *r);
    shared_ptr<ResHandle> find(Resource *r);
    void update(shared_ptr<ResHandle> handle);

    void freeOneResource();
    void memoryHasBeenFreed(unsigned int size);
public:
    ResCache(const unsigned int sizeInMb, IResourceFile *resFile);
    virtual ~ResCache();
    bool init();
    void registerLoader(shared_ptr<IResourceLoader> loader);
    shared_ptr<ResHandle> getHandle(Resource *r);
    int preload(const std::string pattern, void(*progressCallback)(int,bool &));
    std::vector<std::string> match(const string pattern);
    void flush(void);
    bool isUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_file); return m_file->vIsUsingDevelopmentDirectories();}
};

class DefaultResourceLoader : public IResourceLoader
{
public:
    virtual bool vUseRawFile() override { return true; }
    virtual bool vDiscardRawBufferAfterLoad() override { return true; }
    virtual unsigned int vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override { return rawSize; }
    virtual bool vLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) override { return true; }
    virtual std::string vGetPattern() override { return "*"; }

};


class ResourceZipFile : public IResourceFile
{
    ZipFile *m_pZipFile;
    std::wstring m_resFileName;
public:
    ResourceZipFile(const std::wstring resFileName): m_pZipFile{NULL},m_resFileName{resFileName}{}
    virtual ~ResourceZipFile() override;
    virtual bool vOpen() override;
    virtual int vGetRawResourceSize(const Resource &r) override;
    virtual int vGetRawResource(const Resource &r, char *buffer) override;
    virtual int vGetNumResources()  const override;
    virtual std::string vGetResourceName(int num) const override;
    virtual bool vIsUsingDevelopmentDirectories(void) const override {return false;}
};














































#endif // RESOURCECACHE_H
