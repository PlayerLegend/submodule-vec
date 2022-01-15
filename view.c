#include <math.h>
#include <assert.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "vec4.h"
#include "view.h"
#include "../keyargs/keyargs.h"
#include "mat4.h"

void view_normals_setup (view_normals * target, const fvec4 * quaternion)
{
    mat4 rotation_matrix = {0};

    fvec4 quaternion_fix = { .x = -quaternion->x, .y = -quaternion->y, .z = -quaternion->z, .w = quaternion->w };
    
    mat4_setup_rotation_matrix(.result = &rotation_matrix, .quaternion = &quaternion_fix);

    fvec4 in;
    fvec4 out;

    in = (fvec4) { .x = 1, .w = 1 };
    mat4_multiply_right_vec(&out, &rotation_matrix, &in);
    target->right = (fvec3){ .x = out.x, .y = out.y, .z = out.z };
    
    in = (fvec4) { .y = 1, .w = 1 };
    mat4_multiply_right_vec(&out, &rotation_matrix, &in);
    target->up = (fvec3){ .x = out.x, .y = out.y, .z = out.z };
    
    in = (fvec4) { .z = 1, .w = 1 };
    mat4_multiply_right_vec(&out, &rotation_matrix, &in);
    target->forward = (fvec3){ .x = out.x, .y = out.y, .z = out.z };
}

void view_axis_add (fvec3 * target, const fvec3 * add)
{
    vec3_add (*target, *add);
    
    /*float axis_length = sqrt (target->y * target->y + target->x * target->x + target->z * target->z);
    float pi = 3.14159;

    if (axis_length > 2 * pi)
    {
	fvec take_scale = 2 * pi / axis_length;
	fvec3 axis_take = vec3_scale_init(*target, take_scale);
	vec3_subtract(*target, axis_take);
	}*/
}
