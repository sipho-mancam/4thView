#pragma once
#include "callback.hpp"

namespace KEvents
{
	class TestCallback : public CallBackBase
	{
	public:
		virtual void execute(Event e)
		{
			eventProducerPtr->sendMessage("test",e);
		}
	};


	class TestCallback2 : public CallBackBase
	{
	public:
		virtual void execute(Event e)
		{
			eventProducerPtr->sendMessage("test", e);
		}
	};

	class TestCallback3 : public CallBackBase
	{
	public:
		TestCallback3(std::string callback_name)
		{

		}

		virtual void execute(Event e)
		{
			eventProducerPtr->sendMessage("test", e);
		}
	};

	class TestCallback4 : public CallBackBase
	{
	public:
		TestCallback4(std::string callback_name, std::string _some_other_argument)
		{

		}

		virtual void execute(Event e)
		{
			eventProducerPtr->sendMessage("test", e);
		}
	};

}

