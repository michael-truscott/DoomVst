#include "doomgeneric.h"
#include "doomgeneric_vst.h"
#include "doomkeys.h"
#include <string.h>

uint32_t ticks = 0;
uint32_t* mainFrameBuffer;

void DG_Init() {
	// TODO:
	mainFrameBuffer = malloc(sizeof(uint32_t) * DOOMGENERIC_RESX * DOOMGENERIC_RESY);
}

void DG_DrawFrame() {
	// TODO:
	memcpy(mainFrameBuffer, DG_ScreenBuffer, sizeof(uint32_t) * DOOMGENERIC_RESX * DOOMGENERIC_RESY);
}

void DG_SleepMs(uint32_t ms) {
	// TODO:
}

uint32_t DG_GetTicksMs() {
	return getElapsedMillis();
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
	keyEvent_t event;
	if (getNextKeyEvent(&event)) {
		*pressed = event.pressed;
		*doomKey = event.key;
		return 1;
	}
	return 0;
}

void DG_SetWindowTitle(const char* title) {

}