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
#ifndef MULTI_MESSAGE_QUEUE_H
#define MULTI_MESSAGE_QUEUE_H
#include "MP_Mutex.h"
#include <queue>
#include "MPCommon.h"
using namespace MultiPlatformWrapper;

namespace MultiTaskCenter
{
typedef struct
{
    std::string  Sender;
    unsigned int ID;
	void*        data;
} MQueueNode;

class MMsgQueue
{
public:
	MMsgQueue(MPBaseLibraryType type);
	~MMsgQueue();

public:
	bool isEmpty();
	bool push(std::string Sender, unsigned int ID, void* pDataPointer);
	bool pop(std::string &Sender, unsigned int &ID, void* &pDataPointer);
	bool top(std::string &Sender, unsigned int &ID, void* &pDataPointer);
	bool clear();
	
private:
	MP_Mutex* m_pMutex;
	std::queue<MQueueNode> m_queue;
};
}
#endif
