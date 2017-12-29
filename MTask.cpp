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
#include "MTask.h"
#include "MP_ThreadFactory.h"
#include "MP_MutexFactory.h"
using namespace MultiPlatformWrapper;

namespace MultiTaskCenter
{
MTask::MTask(const char* pTaskName, bool isHandleRspFirst, MPBaseLibraryType type)
	: m_reqQueue(type), m_rspQueue(type)
{
    m_isHandleRspFirst = isHandleRspFirst;
	m_taskName = pTaskName;
	m_isRuning = false;
	m_pMutex = MP_MutexFactory::createMutex(type, "MTask");
	m_thread = MP_ThreadFactory::createThread(type);
}

MTask::~MTask()
{
    m_pMutex->lock();
	delete m_thread;
	m_pMutex->unlock();

	delete m_pMutex;
}

bool MTask::start(ActionRspListener* pRspListener, ActionReqListener* pReqListener)
{
    if (m_isRuning)
    {
        return false;
    }
	
	m_pMutex->lock();
    // Save the paramenter 
    m_pRspListener = pRspListener;
	m_pReqListener = pReqListener;

	// Start the thread 
	m_isRuning = true;
	if (!m_thread->start(this, NULL, NULL))
	{
	    m_pMutex->unlock();
	    return false;
	}
	m_pMutex->unlock();
	
	return true;
}

bool MTask::stop()
{
    m_pMutex->lock();
    m_isRuning = false;
    m_thread->stop();
	m_pMutex->unlock();

	return true;
}

std::string MTask::name()
{
    return m_taskName;
}

bool MTask::request(const char *pSenderName, unsigned int msgID, void *msgBody)
{
    m_pMutex->lock();
    m_reqQueue.push(pSenderName, msgID, msgBody);
	m_pMutex->unlock();

	return true;
}

bool MTask::response(const char *pSenderName, unsigned int msgID, void *msgBody)
{
    m_pMutex->lock();
    m_rspQueue.push(pSenderName, msgID, msgBody);
	m_pMutex->unlock();

	return true;
}

int MTask::onMPThreadLoop(void *pUser1, void *pUser2)
{
    while (m_isRuning)
    {
        /* Handle a response message */
        {
            std::string Sender = "";
			unsigned int ID = 0;
			void* pDataPointer = NULL;
			
			m_pMutex->lock();
            bool result = m_rspQueue.pop(Sender, ID, pDataPointer);
			m_pMutex->unlock();

			if (result)
			{
			    m_pRspListener->onReceiveResponse(Sender.c_str(), ID, pDataPointer);
			}
        }

		m_pMutex->lock();
		if (m_isHandleRspFirst && m_rspQueue.isEmpty())
		{
		    m_pMutex->unlock();
		    continue;
		}
		m_pMutex->unlock();

        /* Handle a request message */
        {
            std::string Sender = "";
			unsigned int ID = 0;
			void* pDataPointer = NULL;
			
			m_pMutex->lock();
            bool result = m_reqQueue.pop(Sender, ID, pDataPointer);
			m_pMutex->unlock();

			if (result)
			{
				m_pReqListener->onReceiveRequest(Sender.c_str(), ID, pDataPointer);
			}
        }
    }

	return 0;
}
	
}