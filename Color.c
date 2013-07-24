#include "Color.h"
#include <cmath>


Color makeColor(float red, float green, float blue) {

    return (Color) {red, green, blue};
}

Color cAdd(Color a, Color b) {

    return (Color) {
        a.red   + b.red,
        a.green + b.green,
        a.blue  + b.blue
    };
}

Color cMul(Color a, Color b) {

    return (Color) {
        a.red   * b.red,
        a.green * b.green,
        a.blue  * b.blue
    };
}

unsigned char linearFloatToGammaEncodedUint8(float channel, float gamma) {

    float gammaEncoded = powf(channel, 1/gamma);
    float clipped = fmin(fmax(gammaEncoded, 0), 1);

    return clipped * 255 + 0.5;
}
