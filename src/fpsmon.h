#ifndef SIMPLE_RASTERIZER_FPSMON_H
#define SIMPLE_RASTERIZER_FPSMON_H

#include "fenster.h"

class FPSMon
{
private:
    int frames;
    int64_t ttf_accum;
    int64_t frame_start_time;
public:
    FPSMon(): frames(0), ttf_accum(0), frame_start_time(0) { }
    void frame_start() { frame_start_time = fenster_time(); }
    void frame_end()
    {
        ttf_accum += fenster_time() - frame_start_time;
        frames++;
        if (ttf_accum >= 1000)
        {
            std::cout << "FPS: " << frames << " TTF: " << ttf_accum / frames << " ms." << '\n';
            ttf_accum = 0;
            frames = 0;
        }
    }
};

#endif //SIMPLE_RASTERIZER_FPSMON_H
