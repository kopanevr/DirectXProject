#include "fps_counter.h"

#include <cassert>
#include <algorithm>

#define MAX_FPS                                                             (  60.0  )

#define DELAY_UPADATE_S                                                     (   1.0  )

/**
 * @brief
 */
void FPS_Counter::Start() noexcept
{
    startTime = std::chrono::high_resolution_clock::now();

    if (flagUpdate == true)
    {
        startUpdateTime = startTime;

        flagUpdate = false;
    }
}

/**
 * @brief
 */
void FPS_Counter::End() noexcept
{
    endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = endTime - startTime;
    std::chrono::duration<double> durationUpdate = endTime - startUpdateTime;

    static_assert(MAX_FPS > 0.0);
    static_assert(DELAY_UPADATE_S > 0.0);

    if (durationUpdate.count() >= DELAY_UPADATE_S)
    {
        fps = static_cast<uint8_t>(1.0 / duration.count());

        fps = std::min<double>(fps, MAX_FPS);

        flagUpdate = true;
    }
}

/**
 * @brief
 */
uint8_t FPS_Counter::GetFps() const noexcept
{
    return fps;
}

/**
 * @brief
 */
bool FPS_Counter::GetFlagUpdate() const noexcept
{
    return flagUpdate;
}
