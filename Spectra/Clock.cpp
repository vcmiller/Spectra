// System includes
#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

#include "Clock.h"

namespace spectra {
	namespace internal {

		Clock::Clock() {
			lastReset = time();
		}

		ulong Clock::reset() {
			ulong t = time();
			ulong d = t - lastReset;
			lastReset = t;
			return d;
		}

		ulong Clock::elapsed() {
			return time() - lastReset;
		}

		ulong Clock::time() {
#ifndef _WIN32
			struct timespec in;
			clock_gettime(CLOCK_REALTIME, &in);
			return (in.tv_sec * 1000000 + in.tv_nsec / 1000);
#else
			FILETIME ftime;
			GetSystemTimeAsFileTime(&ftime);
			LONGLONG t = (LONGLONG)ftime.dwLowDateTime + ((LONGLONG)ftime.dwHighDateTime << 32LL);
			return t / 10;
#endif
		}

		void Clock::sleep(ulong microseconds) {
#ifdef _WIN32
			Sleep(microseconds / 1000);
#else
			usleep(microseconds);
#endif
		}

		bool Clock::init() {
#ifdef _WIN32
			return timeBeginPeriod(1) == TIMERR_NOERROR;
#else
			return true;
#endif
		}

		bool Clock::stop() {
#ifdef _WIN32
			return timeEndPeriod(1) == TIMERR_NOERROR;
#else
			return true;
#endif
		}
	}
}