#ifndef NL_S_DISTRIBUTOR_H_
#define NL_S_DISTRIBUTOR_H_

#include "NL_Distributor.h"
#include "NL_SContainer.h"

#include <cstdint>
#include <unordered_map>
#include <memory>

namespace NLE
{
	namespace Core
	{
		namespace Data
		{
			template<typename T>
			class SDistributor : public Distributor
			{
			public:
				SDistributor(uint_fast32_t dataSize, uint_fast32_t queueSize);
				~SDistributor();

				SContainer<T>& buildEndpoint(uint_fast8_t sysId);
				void distributeFrom(uint_fast8_t sysId);
				void distributeTo(uint_fast8_t sysId);

			private:
				uint_fast32_t _queueSize;

				std::vector<T, tbb::scalable_allocator<T>> _data;
				std::unordered_map<uint_fast8_t, SContainer<T>*> _containers;
			};
		}
	}
}

#include "NL_SDistributor.inl"

#endif