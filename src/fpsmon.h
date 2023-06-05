#ifndef SIMPLE_RASTERIZER_FPSMON_H
#define SIMPLE_RASTERIZER_FPSMON_H

#define FPS_MON_INIT \
    int frames = 0; \
    int64_t ttf_accum = 0;

#define FPS_MON_FRAME_START \
    int64_t f_start = fenster_time();

#define FPS_MON_FRAME_END \
    ttf_accum += fenster_time() - f_start; \
    frames++; \
    if (ttf_accum >= 1000) \
    { \
        std::cout << "FPS: " << frames << " TTF: " << ttf_accum / frames << " ms." << '\n'; \
        ttf_accum = 0; \
        frames = 0; \
    } \

#endif //SIMPLE_RASTERIZER_FPSMON_H
