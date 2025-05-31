#include "timer.h"

void Timer::Start(long long duration)
{
    std::chrono::steady_clock::duration d = std::chrono::milliseconds(duration);

    cnt = std::chrono::steady_clock::now();

    cmp = cnt + d;

    isRunningFlag = true;
}

void Timer::Stop()
{
    isRunningFlag = false;
}

void Timer::Reset()
{
    isRunningFlag = false;
}

bool Timer::isFinished() const
{
    if (isRunningFlag == false) { return false; }

    return std::chrono::steady_clock::now() >= cmp;
}

bool Timer::isRunning() const
{
    return isRunningFlag;
}
