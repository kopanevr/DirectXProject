#include "fps_counter.h"

#include <cassert>
#include <algorithm>

#define MAX_FPS                                                             ( 480    )

#define DELAY_UPADATE_S                                                     (   1.0  )

/**
 * @brief
 */
void FPS_Counter::Start() noexcept
{
    if (flagUpdate == true)
    {
        startTime = std::chrono::high_resolution_clock::now();

        flagUpdate = false;
    }
}

/**
 * @brief
 */
void FPS_Counter::End() noexcept
{
    frameCount++;

    endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = endTime - startTime;

    static_assert(MAX_FPS > 0);
    static_assert(DELAY_UPADATE_S > 0.0);

    if (duration.count() >= DELAY_UPADATE_S)
    {
        fps = static_cast<uint16_t>(frameCount / duration.count());

        fps = std::min<uint16_t>(fps, MAX_FPS);

        flagUpdate = true;

        frameCount = 0U;
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
