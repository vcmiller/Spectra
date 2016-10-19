#pragma once

namespace spectra {
	typedef unsigned long long ulong;
	namespace internal {
		class Clock {
		public:
			Clock();

			ulong reset();

			ulong elapsed();

			static ulong time();
			static void sleep(ulong microseconds);
			static bool init();
			static bool stop();

		private:
			ulong lastReset;
		};
	}
}