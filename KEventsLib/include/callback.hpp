#pragma once
#include "types.hpp"

namespace KEvents
{
	class CallBackBase
	{
	public:

		CallBackBase() = default;
		// if the operator is not used, this is the function that can be used
		virtual void execute(Event e) = 0;
		virtual void setEventProducer(EventProducerPtr eProducer);

	protected:
		EventProducerPtr eventProducerPtr;
	};
}