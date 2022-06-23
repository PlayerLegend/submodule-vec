#include <math.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "../log/log.h"

fvec fvec3_vlen (const fvec3 * a)
{
    return sqrt (a->x * a->x + a->y * a->y + a->z * a->z);
}

void fvec3_normalize (fvec3 * result, const fvec3 * input)
{
    float length = fvec3_vlen (input);

    if (length < FVEC_EPSILON)
    {
	*result = (fvec3){0};
    }
    else
    {
	float inv_length = 1.0 / length;

	*result = (fvec3){ vec3_scale_init (*input, inv_length) };
    }
}
