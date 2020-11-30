// This is a separate file so C99 designated initializers can be used. See
// the corresponding .c file for how this simplifies initialization.
//
// Inspired by or borrowed from the evtest program.

#pragma once
#include <stdlib.h>
#include <linux/version.h>
#include <linux/input.h>

#define NAME_ELEMENT(element) [element] = #element

static const char *events[EV_MAX + 1];
static int maxval[EV_MAX + 1];
