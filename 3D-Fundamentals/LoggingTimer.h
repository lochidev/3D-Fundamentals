#pragma once
#include <iostream>
#include <chrono>
struct LoggingTimer {
	std::chrono::steady_clock::time_point m_Start;
	long long result;
	LoggingTimer()
	{
		m_Start = std::chrono::high_resolution_clock::now();
		result = -1;
	}
	~LoggingTimer() {
		if (result == -1) {
			Stop();
			LogResult();
		}
	}
	const long long Stop() {
		auto endP = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endP).time_since_epoch().count();
		result = end - start;
		return result;
	}

	void LogResult() const {
		std::cout << "Timer ended in: " << result << " microseconds\n";
	}
	inline const double ResultInMs() const {
		return result * 0.001;
	}
	inline const double ResultInS() const {
		return result * 0.000001;
	}
};