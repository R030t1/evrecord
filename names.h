// This is a separate file so C99 designated initializers can be used. See
// the corresponding .c file for how this simplifies initialization.
//
// Inspired by or borrowed from the evtest program.

#pragma once
#include <stdlib.h>
#include <linux/version.h>
#include <linux/input.h>

#define NAME_ELEMENT(element) [element] = #element

extern const char *events[EV_MAX + 1];
extern int maxval[EV_MAX + 1];

// TODO: Remove guards?
#ifdef INPUT_PROP_SEMI_MT
extern const char *props[INPUT_PROP_MAX + 1];
#endif

extern const char *keys[KEY_MAX + 1];
extern const char *absval[6];
extern const char *relatives[REL_MAX + 1];
extern const char *absolutes[ABS_MAX + 1];
extern const char *misc[MSC_MAX + 1];
extern const char *leds[LED_MAX + 1];
extern const char *repeats[REP_MAX + 1];
extern const char *sounds[SND_MAX + 1];
extern const char *syns[SYN_MAX + 1];
extern const char *switches[SW_MAX + 1];
extern const char *force[FF_MAX + 1];
extern const char *forcestatus[FF_STATUS_MAX + 1];

extern const char **names[EV_MAX + 1];
