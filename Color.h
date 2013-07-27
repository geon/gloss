#ifndef COLOR_H
#define COLOR_H

typedef struct {
    float red, green, blue;
} Color;

Color makeColor(float red, float green, float blue);

Color cAdd(Color a, Color b);
Color cMul(Color a, Color b);

unsigned char linearFloatToGammaEncodedUint8(float channel, float gamma);

#endif // COLOR_H
