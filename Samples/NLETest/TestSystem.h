#ifndef TEST_SYSTEM_H_
#define TEST_SYSTEM_H_

#include "NLE\NLCore\NL_System.h"

class SysTask;
class Scheduler;
class TestSystem : public NLE::Core::System
{
public:
	TestSystem();
	~TestSystem();

	bool initialize(uint_fast8_t id);
	void release();

	uint_fast8_t getID();
	NLE::Core::SysTask* getTask(NLE::Core::Scheduler* scheduler);

private:
	uint_fast8_t _id;
};

#endif