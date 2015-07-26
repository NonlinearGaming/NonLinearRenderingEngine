/*
-----------------------------------------------------------------------------
This source file is part of NLRE
(NonLinear Rendering Engine)
For the latest info, see https://github.com/AlexandrSachkov/NonLinearRenderingEngine

Copyright (c) 2015 Alexandr Sachkov

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

#ifndef NL_D3D11_DEVICE_H_
#define NL_D3D11_DEVICE_H_

#include "NL_RenderingResources.h"


#include <string>
#include <cstdint>

namespace NLE
{
	namespace GRAPHICS
	{
		class D3D11Device
		{
		public:
			D3D11Device();
			~D3D11Device();

			bool createDeviceAndSwapChain(
				HWND windowHandle, 
				uint_fast32_t screenWidth, 
				uint_fast32_t screenHeight,
				bool fullScreen,
				ID3D11Device*& device,
				IDXGISwapChain*& swapChain,
				ID3D11DeviceContext*& deviceContext
				);
			
			bool createBackBufferRenderTargetView(
				ID3D11Device* device, 
				IDXGISwapChain* swapChain, 
				ID3D11RenderTargetView*& renderTargetView
				);

			bool createBlendStates(
				ID3D11Device* device,
				bool enableBlend, 
				bool enableIndependentBlending, 
				uint_fast32_t numRenderTargets,
				bool enableAlphaToCoverage, 
				ID3D11BlendState*& blendState
				);

			template<class T>
			bool createBuffer(
				ID3D11Device* device,
				D3D11_BIND_FLAG bindFlag,
				D3D11_USAGE usage,
				T data[],
				size_t dataLength,
				RESOURCES::Buffer& buffer
				);

			bool createRenderTargetViews(
				ID3D11Device* device,
				uint_fast32_t numViews,
				uint_fast32_t width,
				uint_fast32_t height,
				ID3D11RenderTargetView*& renderTargetViews
				);

			bool createDepthStencilView(
				ID3D11Device* device,
				uint_fast32_t width,
				uint_fast32_t height,
				ID3D11DepthStencilView*& depthStencilView
				);
			
			bool createShaderResourceViewFromTexture2D(
				ID3D11Device* device, 
				ID3D11Texture2D* texture, 
				ID3D11ShaderResourceView*& resourceView
				);
			
			bool createTexture2D(
				ID3D11Device* device,
				uint_fast32_t width,
				uint_fast32_t height,
				bool generateMipMaps,
				uint_fast32_t MSAALevel,
				D3D11_USAGE usage,
				D3D11_BIND_FLAG bindFlag,
				void* data,
				uint_fast32_t memPitch,
				ID3D11Texture2D*& texture
				);

			bool createInputLayout(
				ID3D11Device* device, 
				D3D11_INPUT_ELEMENT_DESC ilDesc[],
				uint_fast32_t size,
				RESOURCES::VertexShader& vShader, 
				ID3D11InputLayout*& inputLayout
				);

			bool createTextureSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);

			bool createRasterizerState(
				ID3D11Device* device, 
				D3D11_CULL_MODE cullMode, 
				D3D11_FILL_MODE fillMode, 
				ID3D11RasterizerState*& rasterizerState
				);
				
			bool loadVertexShader(ID3D11Device* device, std::wstring path, RESOURCES::VertexShader& vertexShader);
			bool loadPixelShader(ID3D11Device* device, std::wstring path, RESOURCES::PixelShader& pixelShader);
			
			void setFullscreen(IDXGISwapChain*& swapChain, bool fullScreen);

			void setViewPort(
				ID3D11DeviceContext*& deviceContext, 
				uint_fast32_t numViewports, 
				float x, 
				float y, 
				float width, 
				float height, 
				float minDepth, 
				float maxDepth
				);

			void setViewPort(
				ID3D11DeviceContext*& deviceContext, 
				float screenWidth,
				float screenHeight
				);

			void updateBuffer(
				ID3D11DeviceContext*& deviceContext, 
				RESOURCES::Buffer& buffer, 
				void* data, 
				size_t dataSize
				);

			/*void setRenderTargets(
				ID3D11DeviceContext*& deviceContext,
				unsigned int numRenderTargets,
				ID3D11RenderTargetView* renderTargerViews,
				ID3D11DepthStencilView* depthStencilView
				);

			void VSSetShaderResources(unsigned int startSlot, unsigned int numViews, ID3D11ShaderResourceView* resourceViewArr);
			void PSSetShaderResources(unsigned int startSlot, unsigned int numViews, ID3D11ShaderResourceView* resourceViewArr);
			void GSSetShaderResources(unsigned int startSlot, unsigned int numViews, ID3D11ShaderResourceView* resourceViewArr);

			void setVertexShader(const RESOURCES::VertexShader& vertexShader);
			void setPixelShader(const RESOURCES::PixelShader& pixelShader);

			void VSSetConstantBuffer(unsigned int startSlot, unsigned int numBuffers, const RESOURCES::Buffer& buffer);
			void PSSetConstantBuffer(unsigned int startSlot, unsigned int numBuffers, const RESOURCES::Buffer& buffer);
			void GSSetConstantBuffer(unsigned int startSlot, unsigned int numBuffers, const RESOURCES::Buffer& buffer);

			void setVertexBuffer(const RESOURCES::Buffer& buffer, unsigned int slotNum);
			void setIndexBuffer(const RESOURCES::Buffer& buffer);

			

			void setBlendState(ID3D11BlendState* blendState, float blendFactor[]);
			void setInputLayout(ID3D11InputLayout* inputLayout);
			void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology);
			void setRasterizerState(ID3D11RasterizerState* rasterizerState);
			

			void VSSetTextureSamplerState(unsigned int startSlot, unsigned int numStates, ID3D11SamplerState* samplerState);
			void PSSetTextureSamplerState(unsigned int startSlot, unsigned int numStates, ID3D11SamplerState* samplerState);
			void GSSetTextureSamplerState(unsigned int startSlot, unsigned int numStates, ID3D11SamplerState* samplerState);

			void clearRenderTargetView(ID3D11RenderTargetView* renderTargetView, float colorRGBA[]);
			void clearDepthStencilView(ID3D11DepthStencilView* depthStencilView);
			void drawIndexed(const RESOURCES::Buffer& indexBuffer);
			void draw(const RESOURCES::Buffer& vertexBuffer);
			void display();*/

			private:
				bool compileBlobFromFile(
					std::wstring path,
					LPCSTR entryPoint,
					LPCSTR profile,
					ID3DBlob*& blob
					);

				bool loadBlobFromFile(std::wstring path, ID3DBlob*& blob);
		};
	}
}

#include "NL_D3D11Device.inl"

#endif