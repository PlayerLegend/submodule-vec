#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "vec.h"
#endif

#define vec3(type) struct { type x; type y; type z; }
#define vec3_add(a,b) { (a).x += (b).x; (a).y += (b).y; (a).z += (b).z; }
#define vec3_subtract(a,b) { (a).x -= (b).x; (a).y -= (b).y; (a).z -= (b).z; }
#define vec3_subtract_init(a,b) { (a).x - (b).x, (a).y - (b).y, (a).z - (b).z }
#define vec3_add_init(a,b) { (a).x + (b).x, (a).y + (b).y, (a).z + (b).z }
#define vec3_scale(a,s) { (a).x *= s; (a).y *= s; (a).z *= s; }
#define vec3_dot(a,b) ( (a).x * (b).x + (a).y * (b).y + (a).z * (b).z )
#define vec3_cross(a,b) { (a).y * (b).z - (a).z * (b).y, (a).z * (b).x - (a).x * (b).z, (a).x * (b).y - (a).y * (b).x }
#define vec3_line_point_init(position,direction,scale) { (position).x + (direction).x * (scale), (position).y + (direction).y * (scale), (position).z + (direction).z * (scale) }
#define vec3_initializer(a) { (a).x, (a).y, (a).z }
#define vec3_scale_init(a,s) { (a).x * (s), (a).y * (s), (a).z * (s) }

typedef vec3(ivec) ivec3;
typedef vec3(fvec) fvec3;

fvec fvec3_vlen (const fvec3 * v);
