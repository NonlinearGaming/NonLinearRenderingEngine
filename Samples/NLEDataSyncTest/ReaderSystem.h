#ifndef READER_SYSTEM_H_
#define READER_SYSTEM_H_

#include "NLE\NLCore\NL_System.h"
#include "NLE\NLCore\NL_SlaveContainer.h"
#include "NLE\NLCore\NL_SContainer.h"
#include <memory>

class SysTask;
class Scheduler;
class StateManager;


class ReaderSystem : public NLE::Core::System
{
public:
	ReaderSystem(NLE::Core::Priority priority);
	~ReaderSystem();

	bool initialize(
		uint_fast8_t sysId,
		std::unique_ptr<NLE::Core::StateManager> const& stateManager);

	void release();
	NLE::Core::ExecutionDesc getExecutionDesc();
	uint_fast8_t getID();

	std::function<void()> getExecutionProcedure();

private:
	uint_fast8_t _id;
	NLE::Core::Priority _priority;

	NLE::Core::Data::SlaveContainer* _slave;
	NLE::Core::Data::SContainer* _shared;
};

#endif