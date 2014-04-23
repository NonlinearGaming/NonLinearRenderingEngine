#ifndef NLRE_DX11_RENDERING_DEVICE_
#define NLRE_DX11_RENDERING_DEVICE_

#include "stdafx.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DxErr.h>

#include "NLREDX11ShaderID.h"
//#include "StreamSet.h"

struct ShaderBlob
{
	void* data;
	size_t size;
};

struct cbPerObject
{
	NLE_FLOAT4X4 MVP;
};

class NLREDX11RenderingDevice
{
public:
	NLREDX11RenderingDevice(HWND hwndVal, int widthVal, int heightVal);
	NLREDX11RenderingDevice(const NLREDX11RenderingDevice&);
	~NLREDX11RenderingDevice();

	bool initialize();
	void release();
	//====================================== Getter Functions ==========================================
	ID3D11Device* getD3DDevice();

private:
	
	//====================================== Initialization Functions ==========================================
	bool initializeDirect3d11();
	bool createDeviceAndSwapChain();
	bool createRenderTargetView();
	bool createDepthStencilView();
	bool setRenderTargets();
	bool createDeviceContexts();

	bool createAllResources();
	bool loadVShader(NLRE_ShaderID::VS vsId, std::wstring path);
	bool loadPShader(NLRE_ShaderID::PS psId, std::wstring path);
	void setVShader(NLRE_ShaderID::VS vsId);
	void setPShader(NLRE_ShaderID::PS vsId);
	bool createIndexBuffer();
	bool createStreamBuffers();
	bool createInputLayouts();
	void setViewPort();
	bool createPerFrameCBuffer();
	bool createPerObjectCBuffer();
	bool createTextureSamplerStates();
	bool createBlendStates();
	bool createRasterizerStates();

	bool loadBlobFromFile(std::wstring path, ShaderBlob& blob);
	//====================================== Pipeline Modification Functions ==========================================
	//void setShader(ID3D11DeviceContext* context);
	//void setInputLayout(ID3D11DeviceContext* context);


	
	
	HWND _hwnd;
	int _screenWidth;
	int _screenHeight;

	int numRenderingThreads;
	int numDeferredContexts;
	int maxIndices;
	int maxVertices;
	
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11DeviceContext* _deviceContext;

	ID3D11Buffer* indexStreamBuff;
	ID3D11Buffer* geomStreamBuff;
	ID3D11Buffer* textCoordStreamBuff;

	ID3D11InputLayout* defaultInputLayout;
	ID3D11Buffer* perFrameCBuffer;
	ID3D11Buffer* perObjectCBuffer;
	ID3D11SamplerState* defaultTextSamplerState;
	ID3D11RasterizerState* backFaceCullState;

	static D3D11_INPUT_ELEMENT_DESC defaultLayout[];
	int defaultInputLayoutNumElements;

	std::unordered_map<NLRE_ShaderID::VS, ID3D11VertexShader*> _vertexShaderMap;
	std::unordered_map<NLRE_ShaderID::VS, ShaderBlob> _vertexShaderBlobMap;

	std::unordered_map<NLRE_ShaderID::PS, ID3D11PixelShader*> _pixelShaderMap;
	std::unordered_map<NLRE_ShaderID::PS, ShaderBlob> _pixelShaderBlobMap;
};


#endif