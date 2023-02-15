//
//  timer.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/19/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_TIMER_H_
#define YCSB_C_TIMER_H_

#include <chrono>

namespace utils {
class Timer
{
public:
	Timer() : tp_(std::chrono::high_resolution_clock::now()) {}
	void Reset()
	{
		tp_ = std::chrono::high_resolution_clock::now();
	}
	double GetDurationSec()
	{
		auto tt = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double>>(tt - tp_).count();
	}
	double GetDurationMs()
	{
		auto tt = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(tt - tp_).count();
	}
	double GetDurationUs()
	{
		auto tt = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(tt - tp_).count();
	}
	double GetDurationNs()
	{
		auto tt = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(tt - tp_).count();
	}

private:
	std::chrono::high_resolution_clock::time_point tp_;
};

} // utils

#endif // YCSB_C_TIMER_H_

