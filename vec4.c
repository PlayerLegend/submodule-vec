#include <math.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "vec4.h"
#include <assert.h>
#include "../log/log.h"

void fvec4_setup_rotation_quaternion (fvec4 * q, const fvec3 * axis)
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

void fvec4_normalize (fvec4 * result, const fvec4 * input)
{
    const fvec length = sqrt(input->x * input->x + input->y * input->y + input->z * input->z + input->w * input->w);
    assert (length > FVEC_EPSILON);
    const fvec invlength = 1.0 / length;
    *result = (fvec4){ vec4_scale_init(*input, invlength) };
}

void fvec4_quaternion_multiply (fvec4 * result, const fvec4 * a, const fvec4 * b)
{
    result->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    result->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    result->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    result->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
}

void fvec4_quaternion_angle_delta (fvec4 * result, const fvec3 * begin, const fvec3 * end)
{
    result->w = fvec3_vlen(begin) * fvec3_vlen(end) + vec3_dot(*begin, *end);

    assert (result->w > -FVEC_EPSILON);

    if (result->w < FVEC_EPSILON)
    {
	fvec3 perpendicular = { vec3_cross_init_unit_x(*begin) };
	fvec perpendicular_length = fvec3_vlen(&perpendicular);

	if (perpendicular_length < FVEC_EPSILON)
	{
	    perpendicular = (fvec3){ vec3_cross_init_unit_y(*begin) };
	    perpendicular_length = fvec3_vlen(&perpendicular);
	    assert (perpendicular_length >= FVEC_EPSILON);
	}

	fvec scale = 3.14159 / perpendicular_length;

	vec3_scale(perpendicular, scale);

	fvec4_setup_rotation_quaternion(result, &perpendicular);
    }
    else
    {
	result->alias_vec3 = (fvec3){ vec3_cross_init(*end, *begin) };
    }

    fvec4_normalize(result, result);
}

void fvec4_apply_rotation_quaternion (fvec4 * target, const fvec4 * apply)
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

    fvec4_quaternion_multiply (target, &tmp, apply);
    
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

void fvec4_apply_rotation_axis (fvec4 * target, const fvec3 * axis)
{
    fvec4 new_quaternion;
    fvec4_setup_rotation_quaternion(&new_quaternion, axis);
    fvec4_apply_rotation_quaternion(target, &new_quaternion);
}

void fvec4_apply_rotation_axis_angle (fvec4 * target, const fvec3 * axis, fvec angle)
{
    fvec axis_length = fvec3_vlen (axis);

    if (axis_length == 0)
    {
	return;
    }

    fvec axis_scale = angle / axis_length;

    fvec3 axis_fix = { vec3_scale_init(*axis, axis_scale) };

    fvec4_apply_rotation_axis(target, &axis_fix);
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

void fvec4_hamiltonian_product (fvec4 * result, const fvec4 * a, const fvec4 * b)
{
    *result = (fvec4)
	{
	    .w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z,
	    .x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y,
	    .y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x,
	    .z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w,
        };
}

void fvec4_quaternion_rotate (fvec3 * output, const fvec4 * q_quaternion, const fvec3 * input)
{
    fvec4 v_target = { .alias_vec3 = *input };
    
    fvec4 c_compliment =
	{
	    .w = q_quaternion->w,
	    .x = -q_quaternion->x,
	    .y = -q_quaternion->y,
	    .z = -q_quaternion->z,
	};

    fvec4 qv_hamiltonian;
    fvec4_hamiltonian_product(&qv_hamiltonian, q_quaternion, &v_target);

    fvec4 qvc_hamiltonian;
    fvec4_hamiltonian_product(&qvc_hamiltonian, &qv_hamiltonian, &c_compliment);

    *output = qvc_hamiltonian.alias_vec3;
}

/*void fvec4_quaternion_unrotate (fvec3 * output, const fvec4 * q_quaternion, const fvec3 * input)
{
    fvec4 v_target = { .alias_vec3 = *input };
    
    fvec4 c_compliment =
	{
	    .w = q_quaternion->w,
	    .x = -q_quaternion->x,
	    .y = -q_quaternion->y,
	    .z = -q_quaternion->z,
	};

    fvec4 cv_hamiltonian;
    vec4_hamiltonian_product(&cv_hamiltonian, &c_compliment, &v_target);

    fvec4 cvq_hamiltonian;
    vec4_hamiltonian_product(&cvq_hamiltonian, &cv_hamiltonian, q_quaternion);

    *output = cvq_hamiltonian.alias_vec3;
}
*/
