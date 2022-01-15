#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "vec4.h"
#endif

typedef struct {
    fvec3 forward;
    fvec3 right;
    fvec3 up;
}
    view_normals;

void view_normals_setup (view_normals * target, const fvec4 * quaternion);
void view_axis_add (fvec3 * target, const fvec3 * add);
