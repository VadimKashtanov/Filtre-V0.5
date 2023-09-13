#include "mdl.h"

static float p_cond[] = {-1, -.75, -0.5, -.25, 0, .25, +0.5, .75, +1}; 
float poid_cond_rnd() {
    return p_cond[rand() % 9];
};

