#ifndef FLAT_INCLUDES
#include "vec4.h"
#endif

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    float scale;
}
    vec_object3;

#define VEC_OBJECT3_INITIALIZER (vec_object3){ .scale = 1, .quaternion.w = 1 }
