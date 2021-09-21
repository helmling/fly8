/* --------------------------------- sdlstick.c ------------------------------ */

/* This is part of the flight simulator 'fly8'.
 * Author: chris
*/

/* SDL Joystick Handling
*/

#include <SDL.h>

#include "fly.h"
#include "stick.h"

#define SDLS_MAX_AXES 6
#define SDLS_MAX_BUTTONS 32
#define SDLS_MAX_HATS 1 

struct SdlStickState {
    SDL_Joystick *stickHandle;
    SDL_JoystickID stickId;
    int axisCount;
    int buttonCount;
    int hatCount;
    int axisValues[SDLS_MAX_AXES];
    int buttonValues[SDLS_MAX_BUTTONS];
	int hatValue[SDLS_MAX_HATS];

};

static int sdlJsCount;
static struct SdlStickState *sdlStickAllSticks = NULL;

static struct SdlStickState *
SDLStickFindState(SDL_JoystickID stickId) {
    int ctr;

    if (sdlStickAllSticks == NULL) {
        return NULL;
    }

    for (ctr = 0; ctr < sdlJsCount; ctr++) {
        if (sdlStickAllSticks[ctr].stickId == stickId) {
            return &sdlStickAllSticks[ctr];
        }
    }
    return NULL;
}

static int FAR
SDLStickInit(POINTER *p, char *options) {
    int ctr;

    sdlJsCount = SDL_NumJoysticks();

    sdlStickAllSticks = calloc(sdlJsCount, sizeof(*sdlStickAllSticks));
    if (NULL == sdlStickAllSticks) {
        return 1;
    }

    for (ctr = 0; ctr < sdlJsCount; ctr++) {
        struct SdlStickState *const thisStick = &sdlStickAllSticks[ctr];
        SDL_JoystickType jsType;
        int useStick = 0;

        LogPrintf("SDLStick: Joystick %d: %s\n", ctr, SDL_JoystickNameForIndex(ctr));
        jsType = SDL_JoystickGetDeviceType(ctr);
        switch (jsType) {
            case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
                LogPrintf("  - Is a Flight Stick\n");
                useStick = 1;
                break;
            case SDL_JOYSTICK_TYPE_THROTTLE:
                LogPrintf("  - Is a Throttle\n");
                useStick = 1;
                break;
            case SDL_JOYSTICK_TYPE_GAMECONTROLLER:
                LogPrintf("  - Is a Game Controller\n");
                useStick = 1;
                break;
            case SDL_JOYSTICK_TYPE_UNKNOWN:
                LogPrintf("  - Is an Unknown Type\n");
                useStick = 1;
                break;
            default:
                LogPrintf("  - Is not a useful type\n");
                break;
        }
        if (!useStick) {
            continue;
        }
        thisStick->stickHandle = SDL_JoystickOpen(ctr);
        if (NULL == thisStick->stickHandle) {
            LogPrintf("SDLStick: Failed to Open stick %d: %s\n", ctr, SDL_GetError());
            continue;
        }
        thisStick->stickId = SDL_JoystickInstanceID(thisStick->stickHandle);
        thisStick->axisCount = SDL_JoystickNumAxes(thisStick->stickHandle);
        thisStick->buttonCount = SDL_JoystickNumButtons(thisStick->stickHandle);
		thisStick->hatCount = SDL_JoystickNumHats(thisStick->stickHandle);

		LogPrintf("  - Has %d axis\n", thisStick->axisCount);
		LogPrintf("  - Has %d buttons\n", thisStick->buttonCount);
		LogPrintf("  - Has %d hats\n", thisStick->hatCount);
    }
    return 0;
}

static void FAR
SDLStickTerm(POINTER *p) {
    int ctr;

    if (sdlStickAllSticks != NULL) {
        for (ctr = 0; ctr < sdlJsCount; ctr++) {
            struct SdlStickState *const thisStick = &sdlStickAllSticks[ctr];

            if (SDL_JoystickGetAttached(thisStick->stickHandle)) {
                SDL_JoystickClose(thisStick->stickHandle);
                thisStick->stickHandle = NULL;
            }
        }
        free(sdlStickAllSticks);
        sdlStickAllSticks = NULL;
        sdlJsCount = 0;
    }
}

void FAR
SDLStickHandleEvent(SDL_Event *jsEvent) {
    struct SdlStickState *thisJs;
    switch (jsEvent->type) {
        case SDL_JOYAXISMOTION:
            thisJs = SDLStickFindState(jsEvent->jaxis.which);
            if (NULL == thisJs || jsEvent->jaxis.axis >= SDLS_MAX_AXES) {
                break;
            }
            thisJs->axisValues[jsEvent->jaxis.axis] = jsEvent->jaxis.value;
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            thisJs = SDLStickFindState(jsEvent->jbutton.which);
            if (NULL == thisJs || jsEvent->jbutton.button >= SDLS_MAX_BUTTONS) {
                break;
            }
            thisJs->buttonValues[jsEvent->jbutton.button] = jsEvent->jbutton.state;
            break;
		case SDL_JOYHATMOTION:
            thisJs = SDLStickFindState(jsEvent->jhat.which);
            if (NULL == thisJs || jsEvent->jhat.hat >= SDLS_MAX_HATS) {
                break;
            }
			thisJs->hatValue[jsEvent->jhat.hat] = jsEvent->jhat.value;
			break;
        default:
            break;
    }
}

static int FAR
SDLStickRead(POINTER *p) {
    int ctr;
    int sctr;
	char	btn[NBTNS];

    for (ctr = 0; ctr < sdlJsCount; ctr++) {
        struct SdlStickState *const thisStick = &sdlStickAllSticks[ctr];
        if (thisStick->stickHandle == NULL) {
            continue;
        }
        for (sctr = 0; sctr < thisStick->axisCount; sctr++) {
            if (sctr  >= NANALOG) {
                break;
            }
			if (sctr == 0) // invert role
				p->a[sctr] = -((int)thisStick->axisValues[sctr]) * 100 / SDL_JOYSTICK_AXIS_MAX;
			else if (sctr == 3) // postive throttle
				p->a[sctr] = 50 + ((int)thisStick->axisValues[sctr]) * 50 / SDL_JOYSTICK_AXIS_MAX;
			else
				p->a[sctr] = ((int)thisStick->axisValues[sctr]) * 100 / SDL_JOYSTICK_AXIS_MAX;
        }
		/*
        for (sctr = 0; sctr < thisStick->hatCount; sctr++) {
			p->a[10]= 25 * thisStick->hatValue[sctr];
		}
		*/
		memset (btn, 0, sizeof (btn));
        for (sctr = 0; sctr < thisStick->buttonCount; sctr++) {
            if ((sctr) >= NBTNS) {
                break;
            }
		 	btn[sctr] = (char)(thisStick->buttonValues[sctr] == SDL_PRESSED);
        }
        for (sctr = 0; sctr < thisStick->hatCount; sctr++) {
			btn[18] = (char)(thisStick->hatValue[sctr] == SDL_HAT_UP);    // i
			btn[19] = (char)(thisStick->hatValue[sctr] == SDL_HAT_RIGHT); // j
			btn[20] = (char)(thisStick->hatValue[sctr] == SDL_HAT_DOWN);  // k
			btn[21] = (char)(thisStick->hatValue[sctr] == SDL_HAT_LEFT);  // l
		}
		// do_btns (p, btn, thisStick->buttonCount);
		do_btns (p, btn, 22);
    }
    return 0;
}

struct PtrDriver NEAR PtrSdlStick = {
        .name = "SDLStick",
        .Init = SDLStickInit,
        .Term = SDLStickTerm,
        .Read = SDLStickRead,
        .Key = std_key,
};
