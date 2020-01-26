#include "FrameTimer.h"

using std::chrono::steady_clock;

FrameTimer::FrameTimer()
	:
	current(steady_clock::now())
{
}

float FrameTimer::FrameDuration()
{
	const auto last = current;
	current = steady_clock::now();
	std::chrono::duration<float> durration = current - last;
	return durration.count();
}
