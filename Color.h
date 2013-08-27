#ifndef COLOR_H
#define COLOR_H

#include "bool.h"

extern const float cEpsilon;

typedef struct {
	float red, green, blue;
} Color;

Color makeColorWhite();
Color makeColorBlack();
Color makeColorLightness(const float l);
Color makeColor(const float red, const float green, const float blue);

bool cEqual(const Color a, const Color b);

Color  cAdd(const Color a, const Color b);
Color  cMul(const Color a, const Color b);
Color csMul(const Color c, const float s);

unsigned char linearFloatToGammaEncodedUint8(const float channel, const float gamma);

#endif // COLOR_H
