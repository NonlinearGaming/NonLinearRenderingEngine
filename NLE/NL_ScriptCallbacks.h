#ifndef NL_LUA_CALLBACKS_H_
#define NL_LUA_CALLBACKS_H_

#include "tbb\concurrent_unordered_map.h"

struct lua_State;
namespace NLE
{
	namespace SCRIPT
	{
		class Callback
		{
		public:
			static int importScene(lua_State* state);

			static int printConsole(lua_State* state);
		};		
	}
}

#endif