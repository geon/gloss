#include "Color.h"
#include <math.h>


const float cEpsilon = 1/10000.0;

Color makeColorWhite() {
	
	return makeColorLightness(1);
}

Color makeColorBlack() {
	
	return makeColorLightness(0);
}

Color makeColorLightness(const float l) {
	
	return (Color) {l, l, l};
}

Color makeColor(const float red, const float green, const float blue) {
	
	return (Color) {red, green, blue};
}

bool cEqual(const Color a, const Color b) {
	
	return
		a.red   + cEpsilon > b.red   && b.red   + cEpsilon > a.red   &&
		a.green + cEpsilon > b.green && b.green + cEpsilon > a.green &&
		a.blue  + cEpsilon > b.blue  && b.blue  + cEpsilon > a.blue
	;
}

Color cAdd(const Color a, const Color b) {

	return (Color) {
		a.red   + b.red,
		a.green + b.green,
		a.blue  + b.blue
	};
}

Color cMul(const Color a, const Color b) {

	return (Color) {
		a.red   * b.red,
		a.green * b.green,
		a.blue  * b.blue
	};
}

Color csMul(const Color c, const float s) {
	
	return (Color) {
		c.red   * s,
		c.green * s,
		c.blue  * s
	};
}

unsigned char linearFloatToGammaEncodedUint8(const float channel, const float gamma) {

	float gammaEncoded = powf(channel, 1/gamma);
	float clipped = fmin(fmax(gammaEncoded, 0), 1);

	return clipped * 255 + 0.5;
}
