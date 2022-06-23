#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "vec.h"
#endif

#define vec3(type) struct { type x; type y; type z; }
#define vec3_add(a,b) { (a).x += (b).x; (a).y += (b).y; (a).z += (b).z; }
#define vec3_subtract(a,b) { (a).x -= (b).x; (a).y -= (b).y; (a).z -= (b).z; }
#define vec3_subtract_init(a,b) .x = (a).x - (b).x, .y = (a).y - (b).y, .z = (a).z - (b).z
#define vec3_add_init(a,b) .x = (a).x + (b).x, .y = (a).y + (b).y, .z = (a).z + (b).z
#define vec3_scale(a,s) { (a).x *= s; (a).y *= s; (a).z *= s; }
#define vec3_dot(a,b) ( (a).x * (b).x + (a).y * (b).y + (a).z * (b).z )
#define vec3_cross_init(a,b) .x = (a).y * (b).z - (a).z * (b).y, .y = (a).z * (b).x - (a).x * (b).z, .z = (a).x * (b).y - (a).y * (b).x
#define vec3_cross_init_unit_x(a) .y = (a).z, .z = -(a).y
#define vec3_cross_init_unit_y(a) .x = -(a).z, .z = (a).x
#define vec3_cross_init_unit_z(a) .x = (a).y, .y = -(a).x
#define vec3_dot_unit_x(a) ( (a).x )
#define vec3_dot_unit_y(a) ( (a).y )
#define vec3_dot_unit_z(a) ( (a).z )
#define vec3_negate_init(a) .x = -(a).x, .y = -(a).y, .z = -(a).z
#define vec3_line_point_init(position,direction,scale) (position).x + (direction).x * (scale), (position).y + (direction).y * (scale), (position).z + (direction).z * (scale)
#define vec3_initializer(a) { (a).x, (a).y, (a).z }
#define vec3_scale_init(a,s) .x = (a).x * (s), .y = (a).y * (s), .z = (a).z * (s)
#define vec3_scale3_init(a,s) .x = (a).x * (s).x, .y = (a).y * (s).y, .z = (a).z * (s).z
#define vec3_scale3(a,s) { (a).x *= (s).x; (a).y *= (s).y; (a).z *= (s).z; }
#define vec3_unscale3(a,s) { (a).x /= (s).x; (a).y /= (s).y; (a).z /= (s).z; }

#define vec3_init_scale_add(origin, normal, scale)	\
    {							\
	.x = (origin).x + (normal).x * (scale),		\
	.y = (origin).y + (normal).y * (scale),		\
	.z = (origin).z + (normal).z * (scale)		\
    }

#define VEC3_FORMSPEC "%f %f %f"
#define VEC3_FORMSPEC_ARG(v) (v).x, (v).y, (v).z

typedef vec3(ivec) ivec3;
typedef vec3(fvec) fvec3;

fvec fvec3_vlen (const fvec3 * v);
void fvec3_normalize (fvec3 * result, const fvec3 * input);
