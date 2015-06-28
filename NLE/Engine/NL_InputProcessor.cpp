#include "NL_InputProcessor.h"
#include "NL_InputEvents.h"

#include <assert.h>

namespace NLE
{
	InputProcessor::InputProcessor() :
		_initialized(false)
	{

	}

	InputProcessor::~InputProcessor()
	{

	}

	bool InputProcessor::initialize(Core::IEngine& engine)
	{
		assert(!_initialized && _eventPoller);

		_procedure = [&](){
			_eventPoller();
		};

		_initialized = true;
		return _initialized;
	}

	void InputProcessor::release()
	{

	}

	bool InputProcessor::initialized()
	{
		return _initialized;
	}

	std::function<void()> const& InputProcessor::getExecutionProcedure()
	{
		return _procedure;
	}

	Core::ISystem& InputProcessor::getInterface()
	{
		return *this;
	}

	void InputProcessor::attachEventPollingOperation(std::function<void()> operation)
	{
		_eventPoller = operation;
	}

	void InputProcessor::processEvent(INPUT::Event& event)
	{
		printf("Processing event\n");
	}
}