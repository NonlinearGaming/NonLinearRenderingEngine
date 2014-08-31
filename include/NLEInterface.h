/*
-----------------------------------------------------------------------------
This source file is part of NLE
(NonLinear Engine)
For the latest info, see https://github.com/AlexandrSachkov/NonLinearEngine

Copyright (c) 2014 Alexandr Sachkov & NonLinear Engine Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef NLE_INTERFACE_
#define NLE_INTERFACE_

#include "NLEWindowReference.h"
#include <memory>

class NLELogInterface;
class NLEInputProcessorInterface;
class NLREInterface;
class NLEGuiManager;

class NLEInterface
{
public:
	virtual ~NLEInterface(){}
	virtual bool initialize() = 0;
	virtual void release() = 0;

	virtual bool isInitialized() = 0;
	virtual bool isRunning()=0;

	virtual void run() =0;
	virtual void stop() =0;
	virtual void onTick()=0;

	virtual std::shared_ptr<NLELogInterface> getLog()=0;
	virtual std::shared_ptr<NLREInterface> getRenderingEngine() =0;
	virtual std::shared_ptr<NLEInputProcessorInterface> getInputProcessor()=0;
	virtual std::shared_ptr<NLEGuiManager> getGuiManager()=0;
};

#endif