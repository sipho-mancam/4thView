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

		/**
		* @brief
		* The producer will be set by the router when you register the callback,
		* So expect the producer to be overriden if you implement your own, or call it something else.
		*/
		virtual void setEventProducer(EventProducerPtr eProducer);

	protected:
		EventProducerPtr eventProducerPtr;
	};
}