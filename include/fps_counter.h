 /**
 * @file fps_counter.h
 * @brief
 */

#pragma once

#include <cstdint>
#include <chrono>

class FPS_Counter final
{
private:
    FPS_Counter(const FPS_Counter&)                                 = delete;
    FPS_Counter& operator=(const FPS_Counter&)                      = delete;
private:
    uint8_t fps                                                     = 0U;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

    std::chrono::high_resolution_clock::time_point startUpdateTime;

    bool flagUpdate                                                 = true;
public:
    FPS_Counter()                                                   = default;
    ~FPS_Counter()                                                  = default;

    void Start()                                                    noexcept;
    void End()                                                      noexcept;

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
    uint8_t GetFps()                                                const noexcept;
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
    bool GetFlagUpdate()                                            const noexcept;
};
