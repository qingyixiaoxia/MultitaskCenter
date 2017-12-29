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
#include "MTaskCenter.h"
#include "MTaskMgr.h"

namespace MultiTaskCenter
{
void*  MTaskCenter::m_handler = NULL;

bool MTaskCenter::InitTaskMgr(MPBaseLibraryType type)
{
    m_handler = new MTaskMgr(type);
	return true;
}

bool MTaskCenter::TerminateTaskMgr()
{
    if (m_handler == NULL)
    {
        return false;
    }

	MTaskMgr* handler = (MTaskMgr*)m_handler;
	delete handler;
	m_handler = NULL;
	return true;
}

bool MTaskCenter::CreateTask(const char *pTaskName, ActionRspListener* pRspListener, ActionReqListener* pReqListener, bool isHandleRspFirst)
{
    if (m_handler == NULL)
    {
        return false;
    }

    MTaskMgr* handler = (MTaskMgr*)m_handler;
	return handler->createTask(pTaskName, pRspListener, pReqListener, isHandleRspFirst);
}

bool MTaskCenter::DestroyTask(const char *pTaskName)
{
    if (m_handler == NULL)
    {
        return false;
    }

    MTaskMgr* handler = (MTaskMgr*)m_handler;
	return handler->destroyTask(pTaskName);
}

bool MTaskCenter::SendRequest(const char *pSenderName, const char *pReceiverName, unsigned int msgID, void *msgBody)
{
    if (m_handler == NULL)
    {
        return false;
    }

    MTaskMgr* handler = (MTaskMgr*)m_handler;
	return handler->sendRequest(pSenderName, pReceiverName, msgID, msgBody);
}

bool MTaskCenter::SendResponse(const char *pSenderName, const char *pReceiverName, unsigned int msgID, void *msgBody)
{
    if (m_handler == NULL)
    {
        return false;
    }

    MTaskMgr* handler = (MTaskMgr*)m_handler;
	return handler->sendResponse(pSenderName, pReceiverName, msgID, msgBody);
}

}