#pragma once

#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	float FrameDuration();
private:
	std::chrono::steady_clock::time_point current;
};