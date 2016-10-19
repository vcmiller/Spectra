#pragma once

#include "Clock.h"

namespace spectra {
	class Spectra;

	class Time {
	public:
		static float delta();
		static float time();
		static float unscaledDelta();
		static float unscaledTime();
		
		static float scale;

	private:
		friend class Spectra;
		static void tick();
		static void init(float spf);

		static float last;
		static float unscaledLast;
		static float elapsed;
		static float unscaledElapsed;
		static internal::Clock clock;
	};
}