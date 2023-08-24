
#include "timeManager.h"


timer_TimeManager::timer_TimeManager()
{
    timeNow = 0;
    timeStart = 0;
    timeEndPause = 0;
    timeStartPause = 0;
    timePause = 0;
    timerStarted = false;
    timerPause = false;
}


void timer_TimeManager::timer_start()
{
    if (!timerStarted) {
        sceRtcGetCurrentTick(&timeStart);
        timerStarted = true;
    }
}

void timer_TimeManager::timer_stop()
{
    if (timerStarted) {
        timeNow = 0;
        timeStart = 0;
        timeEndPause = 0;
        timeStartPause = 0;
        timePause = 0;
        timerStarted = false;
        timerPause = false;
    }
}

void timer_TimeManager::timer_reset()
{
    if (timerStarted)
    {
        timerStarted = false;
    }
}

void timer_TimeManager::timer_startPause()
{
    if (!timerPause && timerStarted) {
        sceRtcGetCurrentTick(&timeStartPause);
        timerPause = true;
    }
}

void timer_TimeManager::timer_endPause()
{
    if (timerPause && timerStarted)
    {
        sceRtcGetCurrentTick(&timeEndPause);
        timePause += timeEndPause - timeStartPause;
        timerPause = false;
    }
}


u64 timer_TimeManager::timer_getTimeMicroSec()
{
	if(!timerStarted) return 0;

	if (timerPause) return (timeStartPause - timeStart - timePause);
	else {
        sceRtcGetCurrentTick(&timeNow);
        return (timeNow - timeStart - timePause);
	}
}

u64 timer_TimeManager::timer_getTimeMilliSec()
{
	if(!timerStarted) return 0;

	if (timerPause) return ((timeStartPause - timeStart- timePause)/1000);
	else {
        sceRtcGetCurrentTick(&timeNow);
        return ((timeNow - timeStart - timePause)/1000);
	}
}

u64 timer_TimeManager::timer_getTimeSec()
{
	if(!timerStarted) return 0;

	if (timerPause) return ((timeStartPause - timeStart - timePause)/1000000);
	else {
        sceRtcGetCurrentTick(&timeNow);
        return ((timeNow - timeStart - timePause)/1000000);
	}
}

u64 timer_TimeManager::timer_getTimeMin()
{
	if(!timerStarted) return 0;

	if (timerPause) return (((timeStartPause - timeStart - timePause)/1000000)/60);
	else {
        sceRtcGetCurrentTick(&timeNow);
        return (((timeNow - timeStart - timePause)/1000000)/60);
	}
}

u64 timer_TimeManager::timer_getTimeHours()
{
	if(!timerStarted) return 0;

	if (timerPause) return ((((timeStartPause - timeStart - timePause)/1000000)/60)/60);
	else {
        sceRtcGetCurrentTick(&timeNow);
        return ((((timeNow - timeStart - timePause)/1000000)/60)/60);
	}
}
