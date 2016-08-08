#ifndef NL_D3D11_RenderingEngine_
#define NL_D3D11_RenderingEngine_

#include "NL_IRenderingEngine.h"
#include "NL_EngineServices.h"


struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11BlendState;
struct ID3D11DepthStencilView;
struct ID3D11InputLayout;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace NLE
{
	class IWindowManager;
	namespace GRAPHICS
	{
		class D3D11RenderingEngine : public IRenderingEngine
		{
		public:
			D3D11RenderingEngine(EngineServices& eServices);
			~D3D11RenderingEngine();

			bool initialize(Size2D screenResolution, bool fullscreen, bool decorated, std::wstring title);
			void update(SystemServices* sServices, DataManager* data, double deltaT);

		private:
			EngineServices& _eServices;
			IWindowManager* _windowManager;

			ID3D11Device* _d3dDevice;
			IDXGISwapChain* _swapChain;
			ID3D11DeviceContext* _deviceContext;

			ID3D11RenderTargetView* _backBufferRenderTargetView;
			ID3D11BlendState* _noBlendState;
			ID3D11DepthStencilView* _depthStencilView;
			ID3D11VertexShader*	_vertexShader;
			ID3D11InputLayout* _inputLayout;
			ID3D11RasterizerState* _backFaceCull;
			ID3D11RasterizerState* _frontFaceCull;
			ID3D11SamplerState*	_textureSamplerState;
			ID3D11PixelShader* _pixelShader;
		};
	}
}


#endif
