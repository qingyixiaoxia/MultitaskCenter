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
#include "MMsgQueue.h"
#include "MP_MutexFactory.h"
using namespace MultiPlatformWrapper;

namespace MultiTaskCenter
{
MMsgQueue::MMsgQueue(MPBaseLibraryType type)
{
    m_pMutex = MP_MutexFactory::createMutex(type, "MMsgQueue");
}

MMsgQueue::~MMsgQueue()
{
    m_pMutex->lock();
	while (!m_queue.empty())
	{
		m_queue.pop();
	}
	m_pMutex->unlock();
	
    delete m_pMutex;
}

bool MMsgQueue::isEmpty()
{
    bool result = false;
	m_pMutex->lock();
    result = m_queue.empty();
	m_pMutex->unlock();

	return result;
}

bool MMsgQueue::push(std::string Sender, unsigned int ID, void* pDataPointer)
{
    m_pMutex->lock();
    MQueueNode node = {Sender, ID, pDataPointer};
    m_queue.push(node);
	m_pMutex->unlock();
	return true;
}

bool MMsgQueue::pop(std::string &Sender, unsigned int &ID, void* &pDataPointer)
{
    m_pMutex->lock();
	if (m_queue.empty())
	{
	    m_pMutex->unlock();
	    return false;
	}
    MQueueNode node = m_queue.front();
	m_queue.pop();
	m_pMutex->unlock();

	ID = node.ID;
	pDataPointer = node.data;
	Sender = node.Sender;
	return true;
}

bool MMsgQueue::top(std::string &Sender, unsigned int &ID, void* &pDataPointer)
{
    m_pMutex->lock();
	if (m_queue.empty())
	{
	    m_pMutex->unlock();
	    return false;
	}
    MQueueNode node = m_queue.front();
	m_pMutex->unlock();

	ID = node.ID;
	pDataPointer = node.data;
	Sender = node.Sender;
	return true;
}

bool MMsgQueue::clear()
{
    m_pMutex->lock();
	while (!m_queue.empty())
	{
	    m_queue.pop();
	}
    m_pMutex->unlock();
	return true;
}

}