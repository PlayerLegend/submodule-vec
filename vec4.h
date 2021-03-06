#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#endif

#define vec4(type) union { struct { type x; type y; type z; type w; }; type index[4]; }
#define vec4_add(a,b) { (a).x += (b).x; (a).y += (b).y; (a).z += (b).z; (a).w += (b).w; }
#define vec4_subtract(a,b) { (a).x -= (b).x; (a).y -= (b).y; (a).z -= (b).z; (a).w -= (b).w; }
#define vec4_scale(a,s) { (a).x *= s; (a).y *= s; (a).z *= s; (a).w *= s; }
#define vec4_scale_init(a,s) .x = (a).x * (s), .y = (a).y * (s), .z = (a).z * (s), .w = (a).w * (s)
#define vec4_quaternion_invert_init(a) .x = -(a).x, .y = -(a).y, .z = -(a).z, .w = (a).w

typedef union { vec4(ivec); ivec3 alias_vec3; } ivec4;
typedef union { vec4(fvec); fvec3 alias_vec3; } fvec4;

void fvec4_setup_rotation_quaternion (fvec4 * q, const fvec3 * axis);
void fvec4_apply_rotation_quaternion (fvec4 * target, const fvec4 * apply);

void fvec4_apply_rotation_axis (fvec4 * target, const fvec3 * axis);
void fvec4_apply_rotation_axis_angle (fvec4 * target, const fvec3 * axis, fvec angle);

float vec4_quaternion_yaw (const fvec4 * q);
float vec4_quaternion_pitch (const fvec4 * q);
float vec4_quaternion_roll (const fvec4 * q);
fvec vec4_quaternion_roll_mod (const fvec4 * q);

float vec4_vlen (const fvec4 * v);

void fvec4_hamiltonian_product (fvec4 * result, const fvec4 * a, const fvec4 * b);
void fvec4_quaternion_rotate (fvec3 * output, const fvec4 * q_quaternion, const fvec3 * input);
//void fvec4_quaternion_unrotate (fvec3 * output, const fvec4 * q, const fvec3 * input);
inline static void fvec4_quaternion_unrotate (fvec3 * output, const fvec4 * q_quaternion, const fvec3 * input)
{
    fvec4_quaternion_rotate (output, &(fvec4){ vec4_quaternion_invert_init(*q_quaternion) }, input);
}
void fvec4_normalize (fvec4 * result, const fvec4 * input);

void fvec4_quaternion_angle_delta (fvec4 * result, const fvec3 * begin, const fvec3 * end);
