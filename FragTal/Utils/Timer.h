#pragma once

#include <chrono>

namespace Utils {

	struct Timer {

		std::chrono::steady_clock::time_point start, now;
		std::chrono::duration<double> duration;

		Timer() {
		}
		~Timer() {
		}

		void Start() {
			static bool initialized = false;
			if (!initialized) {
				start = std::chrono::high_resolution_clock::now();
				initialized = true;
			}
		}
		double GetTime() {
			now = std::chrono::high_resolution_clock::now();
			duration = now - start;

			return duration.count();
		}
		void Reset() {
			start = std::chrono::high_resolution_clock::now();
		}
	};
}
