#pragma once

#include <chrono>

class Timer final
{
private:
    std::chrono::steady_clock::time_point cnt;
    std::chrono::steady_clock::time_point cmp;

    bool isRunningFlag = false;
public:
    Timer() = default;
    ~Timer() = default;

    void Start(long long duration);
    void Stop();

    void Reset();

    bool isFinished()                               const;
    bool isRunning()                                const;
};
