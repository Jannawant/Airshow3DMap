#include "resourcecache.h"
#include <algorithm>
#include <cctype>
#include "codestd.h"
#include "Utilities/stringUtility.h"

using std::transform;

Resource::Resource(const std::string &name) : m_name(name)
{
    //toDO strToLower(m_name);
    transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int))std::tolower); //?
}

bool ResCache::makeRoom(unsigned int size)
{
    if(size > m_cacheSize)
    {
        return false;
    }
    // return null if there's no possible way to allocate the memory
    while(size > (m_cacheSize - m_allocated))
    {
        //The cache is empty , and there's still not enough room
        if(m_lru.empty())
            return false;
        freeOneResource();
    }
    return true;
}

char *ResCache::allocate(unsigned int size)
{
    if(!makeRoom(size))
    {
        GCC_ASSERT("Not room for size " && false);
        return NULL;
    }
    char *mem = GCC_NEW char[size];
    if(mem)
    {
        std::cout<<"Allocated "<<size<<" bytes"<<std::endl;
        m_allocated += size;
    }
    return mem;
}

void ResCache::free(shared_ptr<ResHandle> gonner)
{
    m_lru.remove(gonner);
    m_resources.erase(gonner->m_resource.m_name);
//     Note - the resource might still be in use by something,
//     so the cache can't actually count the memory freed until the
//     ResHandle pointing to it is destroyed.

    //m_allocated -= gonner->m_resource.m_size;
    //delete gonner;
}

shared_ptr<ResHandle> ResCache::load(Resource *r)
{
    std::cout<<"ResCache load resource "<<r->m_name<<std::endl;
    shared_ptr<IResourceLoader> loader;
    shared_ptr<ResHandle> handle;
    // check if there is a loader to load resource, example if Resource *r.m_name =  earth.xml
    // it should be Register loader with pattern *.xml
    for(auto it = m_resourceLoaders.begin(); it!=m_resourceLoaders.end(); ++it)
    {
        shared_ptr<IResourceLoader> testLoader = *it;
        if(WildcardMatch(testLoader->vGetPattern().c_str(), r->m_name.c_str()))
        {
            loader = testLoader;
            break;
        }
    }
    if(!loader)
    {
        GCC_ASSERT(loader && ("Default resource loader not found"));
        return handle;  //resource not loaded!
    }
    std::cout<<"ResCache load: find loader "<<loader->vGetPattern()<<std::endl;
    //Get size of Resource file(for example earth.xml)
    int rawSize = m_file->vGetRawResourceSize(*r);
    if(rawSize < 0)
    {
        GCC_ASSERT(rawSize > 0 && "Resource size returned -1 -Resource not found");
        return shared_ptr<ResHandle>();
    }

    int allocSize = rawSize + ((loader->vAddNullZero()) ? (1) : (0)); // now allocSize == rawSize
    char *rawBuffer = loader->vUseRawFile() ? allocate(allocSize) : GCC_NEW char[allocSize]; //xml loader not use raw file
    memset(rawBuffer,0,allocSize);
    //Load Resource file from zipfile to rawBuffer
    if(rawBuffer == NULL || m_file->vGetRawResource(*r,rawBuffer)==0)
    {
        //resource cache out of memory
        return shared_ptr<ResHandle>();
    }

    char *buffer = NULL;
    unsigned int size = 0;
    if(loader->vUseRawFile()) // not use raw file
    {
        buffer = rawBuffer;
        handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r,buffer,rawSize,this));
    }
    else
    {
        size = loader->vGetLoadedResourceSize(rawBuffer,rawSize); // size == rawSize now
        //if it enough room in cache alloc memory
        // remove last resources by one and check if it is enough memory
        //buffer allocated by size but not initialize
        buffer = allocate(size);
        if(rawBuffer==NULL || buffer==NULL)
        {
            //resource cache out of memory
            return shared_ptr<ResHandle>();
        }
        handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r,buffer,size,this));
        //parse xml and set it as extra data to handle of resource
        bool success = loader->vLoadResource(rawBuffer,rawSize,handle);
        if(loader->vDiscardRawBufferAfterLoad()) //true , delete rawBuffer after use
        {
            SAFE_DELETE_ARRAY(rawBuffer);
        }
        if(!success)
        {
            //resource cache out of memory
            return shared_ptr<ResHandle>();
        }
    }
    if(handle)
    {
        m_lru.push_front(handle);
        m_resources[r->m_name] = handle;
    }
    GCC_ASSERT(loader && "Default resource loade not foune");
    return handle;  //ResCache is out of memory
}

shared_ptr<ResHandle> ResCache::find(Resource *r)
{
    ResHandleMap::iterator i = m_resources.find(r->m_name);
    if(i==m_resources.end())
        return shared_ptr<ResHandle>();
    return i->second;
}

void ResCache::update(shared_ptr<ResHandle> handle)
{
    m_lru.remove(handle);
    m_lru.push_front(handle);
}

void ResCache::freeOneResource()
{
    ResHandleList::iterator gonner = m_lru.end();
    gonner--;
    shared_ptr<ResHandle> handle = *gonner;
    m_lru.pop_back();
    m_resources.erase(handle->m_resource.m_name);

    // Note - you can't change the resource cache size yet - the resource bits could still actually be
    // used by some sybsystem holding onto the ResHandle. Only when it goes out of scope can the memory
    // be actually free again.
}

void ResCache::memoryHasBeenFreed(unsigned int size)
{
    m_allocated -= size;
}

ResCache::ResCache(const unsigned int sizeInMb, IResourceFile *resFile)
{
    m_cacheSize = sizeInMb * 1024 * 1024;   //total memory size
    m_allocated = 0;
    m_file = resFile;
}

ResCache::~ResCache()
{
    while(!m_lru.empty())
    {
        freeOneResource();
    }
    std::cout<<"Delete ResCache"<<std::endl;
    SAFE_DELETE(m_file);
}

bool ResCache::init()
{
    std::cout<<"Cache init"<<std::endl;
    bool retValue = false;
    if(m_file->vOpen())
    {
        registerLoader(shared_ptr<IResourceLoader>(GCC_NEW DefaultResourceLoader()));
        retValue = true;
    }
    return retValue;
}

void ResCache::registerLoader(shared_ptr<IResourceLoader> loader)
{
    std::cout<<"Register loader"<<m_resourceLoaders.size()<<std::endl;
    m_resourceLoaders.push_front(loader);
}

//Get handle of resource by find r->m_name
//if not find resource with that name , load resource:
//Example if resource->m_name ==  earth.xml, than find that resource in zip file, load memory for size
// of this xml file, parse this xml file and save in TiXmlElement object in ResourceExtraData in Handle
shared_ptr<ResHandle> ResCache::getHandle(Resource *r)
{
    std::cout<<"ResCache getHandle: "<<r->m_name<<std::endl;
    shared_ptr<ResHandle> handle(find(r));
    if(handle == NULL)
    {
        std::cout<<"Load: ";
        handle = load(r);
        GCC_ASSERT(handle);
    }
    else
    {
        update(handle);
    }
    std::cout<<"Return handle"<<std::endl;
    return handle;
}

int ResCache::preload(const std::string pattern, void (*progressCallback)(int, bool &))
{
    if (m_file==NULL)
    {
        GCC_ASSERT(m_file!=NULL && "file shouldnt be NULL");
        return 0;
    }

    int numFiles = m_file->vGetNumResources();
    int loaded = 0;
    bool cancel = false;
//	for (int i=0; i<numFiles; ++i)
//	{
//		Resource resource(m_file->vGetResourceName(i));

//		if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
//		{
//			shared_ptr<ResHandle> handle = g_pApp->m_ResCache->GetHandle(&resource);
//			++loaded;
//		}

//		if (progressCallback != NULL)
//		{
//			progressCallback(i * 100/numFiles, cancel);
//		}
//	}
    return loaded;
}

std::vector<std::string> ResCache::match(const std::string pattern)
{
    std::vector<std::string> matchingNames;
        if (m_file==NULL)
            return matchingNames;

        int numFiles = m_file->vGetNumResources();
        for (int i=0; i<numFiles; ++i)
        {
            std::string name = m_file->vGetResourceName(i);
            std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
            if (WildcardMatch(pattern.c_str(), name.c_str()))
            {
                matchingNames.push_back(name);
            }
        }
        return matchingNames;
}

void ResCache::flush()
{
    while(!m_lru.empty())
    {
        shared_ptr<ResHandle> handle = *(m_lru.begin());
        free(handle);
        m_lru.pop_front();
    }
}

ResHandle::ResHandle(Resource &resource, char *buffer, unsigned int size, ResCache *pResCache) : m_resource(resource)
{
    m_buffer = buffer;
    m_size = size;
    m_extra = nullptr;
    m_pResCache = pResCache;
}

ResHandle::~ResHandle()
{
    SAFE_DELETE_ARRAY(m_buffer);
    //m_pResCache->memoryHasBeenFreed(m_size);
}

ResourceZipFile::~ResourceZipFile()
{
    SAFE_DELETE(m_pZipFile);
}

bool ResourceZipFile::vOpen()
{
    std::cout<<"zip vOpen"<<std::endl;
    m_pZipFile = GCC_NEW ZipFile;
    if(m_pZipFile)
    {
        std::cout<<"if m_pZipFile"<<std::endl;
        return m_pZipFile->init(m_resFileName.c_str());
    }
    return false;
}

int ResourceZipFile::vGetRawResourceSize(const Resource &r)
{
    int resourceNum = m_pZipFile->find(r.m_name.c_str());
    if(resourceNum == -1)
    {
        std::cout<<"resource num == -1 "<<std::endl;
        return -1;
    }

    std::cout<<"file len "<<m_pZipFile->getFileLen(resourceNum)<<std::endl;
    GCC_ASSERT(m_pZipFile->getFileLen(resourceNum) >= 0 && "Filelen cant be less 0");
    return m_pZipFile->getFileLen(resourceNum);
}

int ResourceZipFile::vGetRawResource(const Resource &r, char *buffer)
{
    int size=0;
//    optional<int> resourceNum = m_pZipFile->find(r.m_name.c_str());
//    if(resourceNum.valid())
//    {
//        size = m_pZipFile->getFileLen(*resourceNum);
//        m_pZipFile->readFile(*resourceNum, buffer);
//    }
    int resourceNum = m_pZipFile->find(r.m_name.c_str());
    if(resourceNum >= 0 && resourceNum<m_pZipFile->getNumFiles())
    {
        size = m_pZipFile->getFileLen(resourceNum);
        m_pZipFile->readFile(resourceNum, buffer);
    }
    return size;
}

int ResourceZipFile::vGetNumResources() const
{
    return (m_pZipFile==NULL) ? 0 : m_pZipFile->getNumFiles();
}

std::string ResourceZipFile::vGetResourceName(int num) const
{
    string resName = "";
    if(m_pZipFile!=NULL && num>=0 && num<m_pZipFile->getNumFiles())
    {
        resName = m_pZipFile->getFilename(num);
    }
    return resName;
}









































