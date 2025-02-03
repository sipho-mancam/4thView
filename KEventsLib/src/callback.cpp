#include "callback.hpp"


namespace KEvents
{
	void CallBackBase::setEventProducer(EventProducerPtr eProducer)
	{
		eventProducerPtr = eProducer;
	}
}