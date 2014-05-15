/*
-----------------------------------------------------------------------------
This source file is part of NLRE
(NonLinear Rendering Engine)
For the latest info, see https://github.com/AlexandrSachkov/NonLinearRenderingEngine

Copyright (c) 2014 NonLinear Rendering Engine Team

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

#ifndef NLRE_FORWARD_RT_
#define NLRE_FORWARD_RT_

#include "RenderingDevice\NLRERenderingDevice.h"
#include "RenderingDevice\NLRERenderingTechnique.h"

class NLREForwardRT : public virtual NLRERenderingTechnique
{
public:

	NLREForwardRT(NLRERenderingDevice* renderingDevice);
	NLREForwardRT(const NLREForwardRT& other);
	~NLREForwardRT();

	void render(std::vector<NLRE_RenderableAsset*>& assets);
private:
	bool initialize();
	
	NLRERenderingDevice* _renderingDevice;

	//Resources and States

	NLRE_APIRenderTargetView* _backBufferRenderTargetView;
	NLRE_APIBlendState* _noBlendState;
	NLRE_APIBlendState* _blendState;
	NLRE_APIDepthStencilView* _depthStencilView;
	NLRE_VertexShader	_vertexShader;
	NLRE_APIInputLayout* _inputLayout;
	NLRE_APIRasterizerState* _backFaceCull;
	NLRE_APIRasterizerState* _frontFaceCull;
	NLRE_APISamplerState*	_textureSamplerState;
	NLRE_PixelShader _pixelShader;
	
	
};

#endif