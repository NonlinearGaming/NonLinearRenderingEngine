#include "NL_Scene.h"
#include "NL_Uuid.h"
#include "NL_ThreadLocal.h"
#include "NL_GameObject.h"
#include "NL_GameManager.h"

namespace NLE
{
	namespace GAME
	{
		Scene::Scene() : Scene(nullptr) {}

		Scene::Scene(GameManager* gameManager) :
			_gameManager(gameManager),
			_scriptingContext(this)
		{
			_name = L"Scene " + std::to_wstring(UUID::generateUuid());
		}

		Scene::~Scene()
		{

		}

		GameManager* Scene::getGameManager()
		{
			return _gameManager;
		}

		void Scene::setGameManager(GameManager& gameManager)
		{
			_gameManager = &gameManager;
		}

		std::wstring Scene::getName()
		{
			return _name;
		}

		void Scene::setName(std::wstring name)
		{
			_name = name;
		}

		void Scene::addObject(GameObject* object)
		{
			_sceneGraph.addObject(object);
		}

		void Scene::addObject(GameObject* parent, GameObject* object)
		{
			_sceneGraph.addObject(parent, object);
		}

		void Scene::removeObject(std::wstring name)
		{
			_sceneGraph.removeObject(name);
		}

		void Scene::removeObject(GameObject* object)
		{
			_sceneGraph.removeObject(object);
		}

		GameObject* Scene::getObject(std::wstring name)
		{
			return _sceneGraph.getObject(name);
		}

		SCRIPT::ScriptingContext& Scene::getScriptingContext()
		{
			return _scriptingContext;
		}

		void Scene::bind(LuaIntf::CppBindModule<LuaIntf::LuaBinding>& binding)
		{
			binding.addVariableRef<Scene>("this", this);
		}
	}
}