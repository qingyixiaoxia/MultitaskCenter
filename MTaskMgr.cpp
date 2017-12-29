/* Copyright Wenjing Li. and other Node contributors. All rights reserved.
* Author: Wenjing Li, GitHub Address: http://www.github.com/qingyixiaoxia
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/
#include "MTaskMgr.h"
#include "MP_MutexFactory.h"
using namespace MultiPlatformWrapper;

namespace MultiTaskCenter
{
MTaskMgr::MTaskMgr(MPBaseLibraryType type)
{
    m_pMutex = MP_MutexFactory::createMutex(type, "MTaskMgr");
	m_baseLibType = type;
}

MTaskMgr::~MTaskMgr()
{
    m_pMutex->lock();
    for (int i = 0; i < m_taskList.size(); i++)
    {
        MTask* pCur = m_taskList.at(i);
		pCur->stop();
		delete pCur;
    }
	m_taskList.clear();
	m_pMutex->unlock();

	delete m_pMutex;
}
	
bool MTaskMgr::createTask(const char *pTaskName, ActionRspListener* pRspListener, ActionReqListener* pReqListener, bool isHandleRspFirst)
{
    MTask* pNewTask = new MTask(pTaskName, isHandleRspFirst, m_baseLibType);
	if (pNewTask == NULL)
	{
	    return false;
	}
	pNewTask->start(pRspListener, pReqListener);

	m_pMutex->lock();
	m_taskList.push_back(pNewTask);
	m_pMutex->unlock();
	
    return true;
}

bool MTaskMgr::destroyTask(const char *pTaskName)
{
    bool result = false;
	
    m_pMutex->lock();
	std::vector<MTask*>::iterator iter = m_taskList.begin();
	while (iter != m_taskList.end())
	{
        MTask* pCur = *iter;
		if (pCur->name().compare(pTaskName) == 0)
		{
		    pCur->stop();
		    m_taskList.erase(iter);
			result = true;
			break;
		}
	    iter++;
	}
	m_pMutex->unlock();
	
    return result;
}

bool MTaskMgr::sendRequest(const char *pSenderName, const char *pReceiverName, unsigned int msgID, void *msgBody)
{
    bool result = false;
	
    m_pMutex->lock();
	std::vector<MTask*>::iterator iter = m_taskList.begin();
	while (iter != m_taskList.end())
	{
        MTask* pCur = *iter;
		if (pCur->name().compare(pReceiverName) == 0)
		{
		    pCur->request(pSenderName, msgID, msgBody);
			result = true;
			break;
		}
	    iter++;
	}
	m_pMutex->unlock();
	
    return result;
}

bool MTaskMgr::sendResponse(const char *pSenderName, const char *pReceiverName, unsigned int msgID, void *msgBody)
{
    bool result = false;
	
    m_pMutex->lock();
	std::vector<MTask*>::iterator iter = m_taskList.begin();
	while (iter != m_taskList.end())
	{
        MTask* pCur = *iter;
		if (pCur->name().compare(pReceiverName) == 0)
		{
		    pCur->response(pSenderName, msgID, msgBody);
			result = true;
			break;
		}
	    iter++;
	}
	m_pMutex->unlock();
	
    return result;
}

}