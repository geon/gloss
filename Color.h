#ifndef COLOR_H
#define COLOR_H

typedef struct {
	float red, green, blue;
} Color;

Color makeColorWhite();
Color makeColorBlack();
Color makeColorLightness(float l);
Color makeColor(float red, float green, float blue);

Color  cAdd(Color a, Color b);
Color  cMul(Color a, Color b);
Color csMul(Color c, float s);

unsigned char linearFloatToGammaEncodedUint8(float channel, float gamma);

#endif // COLOR_H
