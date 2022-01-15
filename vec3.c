#include <math.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"

fvec fvec3_vlen (const fvec3 * a)
{
    return sqrt (a->x * a->x + a->y * a->y + a->z * a->z);
}
