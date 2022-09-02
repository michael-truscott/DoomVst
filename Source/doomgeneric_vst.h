/*
  ==============================================================================

    doomgeneric_vst.h
    Created: 31 Aug 2022 11:31:48pm
    Author:  Michael

  ==============================================================================
*/

#pragma once

#include "doomgeneric.h"

#define FRAMES_PER_SEC 35
#define FRAME_PERIOD_MS (uint32_t)((1.0 / FRAMES_PER_SEC) * 1000)

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        int pressed;
        unsigned char key;
    } keyEvent_t;

    extern uint32_t* mainFrameBuffer;
    uint32_t getElapsedMillis();
    int getNextKeyEvent(keyEvent_t* event);

#ifdef __cplusplus
}

#endif