#pragma once
#include "types.hpp"

namespace KEvents
{
	class CallBackBase
	{
	public:

		CallBackBase();
		// This is the function that gets invoked by the executing thread.
		virtual void operator()(Event e) = 0;
		// if the operator is not used, this is the function that can be used
		virtual void execute(Event e) = 0;
	};
}