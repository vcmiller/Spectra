#include "Time.h"

namespace spectra {

	float Time::delta() {
		return last;
	}

	float Time::time() {
		return elapsed;
	}

	float Time::unscaledDelta() {
		return unscaledLast;
	}

	float Time::unscaledTime() {
		return unscaledElapsed;
	}

	void Time::tick() {
		unscaledLast = clock.reset() / 1e6f;
		last = unscaledLast * scale;
		elapsed += last;
		unscaledElapsed += unscaledLast;
	}

	void Time::init(float spf) {
		clock.reset();

		scale = 1.0f;
		last = spf;
		unscaledLast = spf;
		elapsed = 0.0f;
		unscaledElapsed = 0.0f;
	}

	spectra::internal::Clock Time::clock;
	float Time::scale;
	float Time::last;
	float Time::unscaledLast;
	float Time::elapsed;
	float Time::unscaledElapsed;
}