#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <memory>
#include <map>
#include <iostream>

using std::map;
using std::shared_ptr;
using std::weak_ptr;
using std::cout;
using std::endl;

template<class Type>
shared_ptr<Type> makeStrongPtr(weak_ptr<Type> pWeakPtr)
{
    if(!pWeakPtr.expired())
        return shared_ptr<Type>(pWeakPtr);
    else
        return shared_ptr<Type>();
}

template <class BaseType, class SubType>
BaseType* genericObjectCreationFunction(void) { return new SubType;}

template<class BaseClass, class IdType>
class GenericObjectFactory
{
    typedef BaseClass* (*ObjectCreationFunction)(void);
    std::map<IdType, ObjectCreationFunction> m_creationFunctions;
public:
    template<class SubClass>
    bool Register(IdType id)
    {
        auto findIt = m_creationFunctions.find(id);
        if(findIt == m_creationFunctions.end())
        {
            m_creationFunctions[id] = &genericObjectCreationFunction<BaseClass,SubClass>; // insert is giving me compiler errors
            return true;
        }
        cout<<"Not Register idType. Return false"<<endl;
        return false;
    }

    BaseClass* create(IdType id)
    {
        auto findIt = m_creationFunctions.find(id);
        if(findIt != m_creationFunctions.end())
        {
            ObjectCreationFunction pFunc = findIt->second;
            return pFunc();
        }
        cout<<"Not create component.Return nullptr "<<endl;
        return nullptr;
    }

};


































#endif // TEMPLATES_H
