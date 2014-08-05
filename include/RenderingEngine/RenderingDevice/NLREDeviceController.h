/*
-----------------------------------------------------------------------------
This source file is part of NLRE
(NonLinear Rendering Engine)
For the latest info, see https://github.com/AlexandrSachkov/NonLinearRenderingEngine

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

#ifndef NLRE_DEVICE_CONTROLLER_
#define NLRE_DEVICE_CONTROLLER_

#include "NLESingleInstance.h"

class NLRERenderingDevice;
class NLRERenderingTechnique;

class NLREDeviceController : private NLESingleInstance<NLREDeviceController>
{
public:
	NLREDeviceController(NLEWindowReference hwndVal, int widthVal, int heightVal, NLRE_RENDERING_TECHNIQUE_ID techniqueId);
	NLREDeviceController(const NLREDeviceController&);
	~NLREDeviceController();

	std::shared_ptr<NLRERenderingDevice> getRenderingDevice();
	bool setRenderingTechnique(NLRE_RENDERING_TECHNIQUE_ID techniqueId);
	NLRE_RENDERING_TECHNIQUE_ID getCurrentRenderingTechniqueId();
	void render(std::vector<std::shared_ptr<NLRE_RenderableAsset>>& assets);
	void setGuiRenderCallback(void(*guiRenderCallback)(void));

	std::shared_ptr<NLRERenderingTechnique> _renderingTechnique;
private:
	bool initialize();
	NLRE_RENDERING_TECHNIQUE_ID _renderingTechniqueId;
	std::shared_ptr<NLRERenderingDevice> _renderingDevice;

	void(*_guiRenderCallback)(void);
	
};
#endif