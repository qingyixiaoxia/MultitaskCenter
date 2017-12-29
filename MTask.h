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
#ifndef MULTI_TASK_H
#define MULTI_TASK_H
#include <string>
#include <vector>
#include "MMsgQueue.h"
#include "MP_Thread.h"
#include "MP_Mutex.h"
#include "MTaskListener.h"
using namespace MultiPlatformWrapper;

namespace MultiTaskCenter
{
class MTask : public MPThreadLoopBase
{
public:
	MTask(const char* pTaskName, bool isHandleRspFirst, MPBaseLibraryType type);
	~MTask();

public:
	bool start(ActionRspListener* pRspListener, ActionReqListener* pReqListener);
	bool stop();
	std::string name();
	
	bool request(const char *pSenderName, unsigned int msgID, void *msgBody);
	bool response(const char *pSenderName, unsigned int msgID, void *msgBody);

private:
	int onMPThreadLoop(void *pUser1, void *pUser2);
	
private:
	bool m_isHandleRspFirst;
	std::string m_taskName;
	MP_Thread* m_thread;
	MP_Mutex* m_pMutex;
	bool m_isRuning;

    MMsgQueue m_reqQueue;
	MMsgQueue m_rspQueue;
	
	ActionRspListener* m_pRspListener;
	ActionReqListener* m_pReqListener;
};
}
#endif
