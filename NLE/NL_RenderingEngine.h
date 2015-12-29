#ifndef NL_RENDERING_ENGINE_H_
#define NL_RENDERING_ENGINE_H_

#include "NL_IRenderingEngine.h"
#include "NL_System.h"
#include "NL_RenderingResources.h"
#include "NL_Timer.h"
#include "NL_Thread.h"
#include "NL_SysInitializer.h"
#include "NL_CommonTypes.h"

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "tbb/atomic.h"

#include <memory>
#include <cstdint>
#include <chrono>
#include <string>
#include <vector>

#include "gl\glew.h"

struct lua_State;
namespace NLE
{
	namespace Core
	{
		class IEngine;
		class ISystem;
		struct SysInitializer;
	}
	namespace IMPORTER
	{
		class AssetImporter;
	}
	namespace WINDOW
	{
		class IWindowManager;
	}
	class WindowManager;
	namespace GRAPHICS
	{
		struct Initializer : public Core::SysInitializer
		{
			Initializer() :
				screenSize(Size2D(0, 0)),
				fullscreen(false),
				decorated(true),
				title(L""),
				openglMajorVersion(0),
				openglMinorVersion(0)
			{}
			Size2D screenSize;
			bool fullscreen;
			bool decorated;
			std::wstring title;
			int openglMajorVersion;
			int openglMinorVersion;
		};

		class Scene;
		class RenderingEngine : public Core::System, public IRenderingEngine
		{
		public:
			RenderingEngine();
			~RenderingEngine();

			bool initialize(Core::IEngine& engine, std::unique_ptr<Core::SysInitializer> const& initializer);
			void start();
			void stop();
			void release();
			bool initialized();
			std::function<void()> const& getExecutionProcedure();
			Core::ISystem& getInterface();

			ID3D11Device* getDevice();

		private:
			void initOpengl();
			void releaseOpengl();
			void render();		

			bool _initialized;
			std::function<void()> _procedure;

			WindowManager* _windowManager;
			Initializer* _init;
			Timer _timer;
			tbb::atomic<bool> _firstRun;
			Core::Thread* _renderingThread;

			GLuint _renderProgram;
			GLuint _vertexArray;
		};
	}
}

#endif