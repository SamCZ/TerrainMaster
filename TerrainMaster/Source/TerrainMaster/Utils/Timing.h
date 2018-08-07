#pragma once
#include "Common.h"

namespace TM {

	struct SDLTiming
	{
		static double getTime();
		static void sleep(uint32 milliseconds);
	};

	typedef SDLTiming PlatformTiming;

	namespace Time {
		inline double getTime() {
			return PlatformTiming::getTime();
		}

		inline void sleep(uint32 milliseconds) {
			PlatformTiming::sleep(milliseconds);
		}
	};
}