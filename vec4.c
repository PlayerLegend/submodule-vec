#include <math.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "vec4.h"

void vec4_setup_rotation_quaternion (fvec4 * q, const fvec3 * axis)
{
    fvec length = (axis->x || axis->y || axis->z) ? fvec3_vlen (axis) : 1;
    
    fvec3 axis_normal =
	{
	    .x = axis->x / length,
	    .y = axis->y / length,
	    .z = axis->z / length
	};

    float cos_angle_2 = cos(length / 2);
    float sin_angle_2 = sin(length / 2);

    q->x = sin_angle_2 * axis_normal.x;
    q->y = sin_angle_2 * axis_normal.y;
    q->z = sin_angle_2 * axis_normal.z;

    q->w = cos_angle_2;
    //q->w = 1;
}

void vec4_quaternion_multiply (fvec4 * result, const fvec4 * a, const fvec4 * b)
{
    result->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    result->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    result->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    result->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
}

void vec4_apply_rotation_quaternion (fvec4 * target, const fvec4 * apply)
{
    /*fvec4 apply_inverse =
	{
	    .w = target->w,
	    .x = -target->x,
	    .y = -target->y,
	    .z = -target->z,
	    };*/
    
    fvec4 tmp = *target;

    //vec4_quaternion_multiply(&tmp, apply, target);

    vec4_quaternion_multiply (target, &tmp, apply);
    
    float scale = 1.0 / vec4_vlen(target);
	
    vec4_scale (*target, scale);
}

#define qx q->z
#define qy q->y
#define qz q->x
#define qw q->w
//float vec4_quaternion_pitch (const fvec4 * q)
float vec4_quaternion_yaw (const fvec4 * q)
{
    return atan2 (2 * (qy * qw - qx * qz), 1 - 2 * (qy * qy + qz * qz));
}

//float vec4_quaternion_yaw (const fvec4 * q)
float vec4_quaternion_pitch (const fvec4 * q)
{
    return asin (2 * (qx * qy + qz * qw));
}

//float vec4_quaternion_roll (const fvec4 * q)
float vec4_quaternion_roll (const fvec4 * q)
{
    return atan2 (2 * (qx * qw - qy * qz), 1 - 2 * (qx * qx + qz * qz));
}

float vec4_vlen (const fvec4 * v)
{
    return sqrt (v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

void vec4_apply_rotation_axis (fvec4 * target, const fvec3 * axis)
{
    fvec4 new_quaternion;
    vec4_setup_rotation_quaternion(&new_quaternion, axis);
    vec4_apply_rotation_quaternion(target, &new_quaternion);
}

void vec4_apply_rotation_axis_angle (fvec4 * target, const fvec3 * axis, fvec angle)
{
    fvec axis_length = fvec3_vlen (axis);

    if (axis_length == 0)
    {
	return;
    }

    fvec axis_scale = angle / axis_length;

    fvec3 axis_fix = vec3_scale_init(*axis, axis_scale);

    vec4_apply_rotation_axis(target, &axis_fix);
}

fvec vec4_quaternion_roll_mod (const fvec4 * q)
{
    fvec roll = vec4_quaternion_roll (q);
    fvec pi = 3.14159;

    if (roll < -0.5)
    {
	return roll + pi;
    }
    else if (roll > 0.5)
    {
	return roll - pi;
    }
    else
    {
	return roll;
    }
}
