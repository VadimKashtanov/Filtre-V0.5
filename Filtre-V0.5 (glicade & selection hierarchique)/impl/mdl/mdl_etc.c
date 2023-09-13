#include "mdl.h"

static float p_cond[] = {-1, -.75, -0.5, -.25, 0, .25, +0.5, .75, +1}; 
float poid_cond_rnd() {
    return p_cond[rand() % 9];
};


float poid_cond_le_plus_proche(float x) {
    float plus_proche = p_cond[0];
    for (uint i=1; i < 8; i++) {
        if (fabs(p_cond[i]-x) < fabs(plus_proche-x)) plus_proche = p_cond[i];
    }
    return plus_proche;
};