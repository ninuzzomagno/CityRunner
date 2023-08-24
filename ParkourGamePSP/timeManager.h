#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pspkernel.h>
#include <psprtc.h>

class timer_TimeManager {

protected:
    u64 timeNow;
    u64 timeStart;
    u64 timeEndPause;
    u64 timeStartPause;
    u64 timePause;
    bool timerStarted;
    bool timerPause;

public:
    timer_TimeManager();
    void timer_start();
    void timer_stop();
    void timer_reset();
    void timer_startPause();
    void timer_endPause();
    u64 timer_getTimeMicroSec();
    u64 timer_getTimeMilliSec();
    u64 timer_getTimeSec();
    u64 timer_getTimeMin();
    u64 timer_getTimeHours();
};
