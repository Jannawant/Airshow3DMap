#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "process.h"
#include <list>

class ProcessManager
{
    typedef std::list<StrongProcessPtr> ProcessList;
    ProcessList m_processList;
public:
    ~ProcessManager();
    unsigned int updateProcesses(float deltaMs);
    WeakProcessPtr attachProcess(StrongProcessPtr pProcess);
    void abortAllProcesses(bool immediate);
    unsigned int getProcessCount() const { return m_processList.size();}
private:
    void clearAllProcesses(); // should only be called by the destructor
};

#endif // PROCESSMANAGER_H
