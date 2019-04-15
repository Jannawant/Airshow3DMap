 #include "processmanager.h"

ProcessManager::~ProcessManager()
{
    clearAllProcesses();
}

//---------------------------------------------------------------------------------------------------------------------
// The process update tick.  Called every logic tick.  This function returns the number of process chains that
// succeeded in the upper 32 bits and the number of process chains that failed or were aborted in the lower 32 bits.
//---------------------------------------------------------------------------------------------------------------------
unsigned int ProcessManager::updateProcesses(float deltaMs)
{
    unsigned short int successCount = 0;
    unsigned short int failCount = 0;
    ProcessList::iterator it = m_processList.begin();
    while(it!=m_processList.end())
    {
        StrongProcessPtr pCurrProcess = (*it);
        // save the iterator and increment the old one in case we need to remove this process from the list
        ProcessList::iterator thisIt = it;
        ++it;
        //process is uninitialized , so initialize it
        if(pCurrProcess->getState() == Process::UNINTIALIZED)
            pCurrProcess->vOnInit();
        //give  the process an update tick if its running
        if(pCurrProcess->getState() == Process::RUNNING)
            pCurrProcess->vOnUpdate(deltaMs);
        //run the appropriate exit function
        if(pCurrProcess->isDead())
        {
            switch(pCurrProcess->getState())
            {
                case Process::SUCCEEDED:
                {
                    pCurrProcess->vOnSuccess();
                    StrongProcessPtr pChild = pCurrProcess->removeChild();
                    if(pChild)
                        attachProcess(pChild);
                    else
                        ++successCount; // only counts if the whole chain completed
                    break;
                }
                case Process::FAILED:
                {
                    pCurrProcess->vOnFail();
                    ++failCount;
                    break;
                }
                case Process::ABORTED:
                {
                    pCurrProcess->vOnAbort();
                    ++failCount;
                    break;
                }
            }
            //removes the process and destroy it
            m_processList.erase(thisIt);
        }
    }
    return ((successCount << 16) | failCount);
}

//---------------------------------------------------------------------------------------------------------------------
// Attaches the process to the process list so it can be run on the next update.
//---------------------------------------------------------------------------------------------------------------------
WeakProcessPtr ProcessManager::attachProcess(StrongProcessPtr pProcess)
{
    m_processList.push_front(pProcess);
    return WeakProcessPtr(pProcess);
}

//---------------------------------------------------------------------------------------------------------------------
// Aborts all processes.  If immediate == true, it immediately calls each ones OnAbort() function and destroys all
// the processes.
//---------------------------------------------------------------------------------------------------------------------
void ProcessManager::abortAllProcesses(bool immediate)
{
    ProcessList::iterator it = m_processList.begin();
    while(it!=m_processList.end())
    {
        ProcessList::iterator tempIt = it;
        ++it;
        StrongProcessPtr pProcess = *tempIt;
        if(pProcess->isAlive())
        {
            pProcess->setState(Process::ABORTED);
            if(immediate)
            {
                pProcess->vOnAbort();
                m_processList.erase(tempIt);
            }
        }
    }
}

void ProcessManager::clearAllProcesses()
{
    m_processList.clear();
}






























