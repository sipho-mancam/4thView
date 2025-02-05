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

	using CallbackBasePtr = std::shared_ptr<CallBackBase>;
	/*template<typename T, typename ...Args>
	std::shared_ptr<T> createCallback(Args&& ... args);*/
	

	template<typename T, typename ...Args>
	std::shared_ptr<T> createCallback(Args && ...args)
	{
		return std::make_shared<T>(args...);
	}
}