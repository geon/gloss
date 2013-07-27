#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

typedef struct {
    Color reflectivity;
    Color radience;
    bool isPerfectBlack;
} Material;

#endif // MATERIAL_H
