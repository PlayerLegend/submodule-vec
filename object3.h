#ifndef FLAT_INCLUDES
#include "shape3.h"
#include "vec4.h"
#endif

#include <stdio.h>

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    fvec3 scale;
}
    vec_object3;

#define VEC_OBJECT3_INITIALIZER (vec_object3){ .scale = { .x = 1.0, .y = 1.0, .z = 1.0 }, .quaternion = { .w = 1.0 } }

//inline static void object3_transform_point (fvec3 * result, const vec_object3 * world, const fvec3 * input)
inline static void fvec3_point_to_object_space (fvec3 * result, const vec_object3 * world, const fvec3 * input)
{
    *result = *input;
    vec3_subtract(*result, world->position);
    fvec4_quaternion_unrotate(result, &world->quaternion, result);
    vec3_unscale3(*result, world->scale);
}

inline static void fvec3_point_to_world_space (fvec3 * result, const vec_object3 * world, const fvec3 * input)
{
    *result = *input;
    vec3_scale3(*result, world->scale);
    fvec4_quaternion_rotate(result, &world->quaternion, result);
    vec3_add(*result, world->position);
}

inline static void fvec3_direction_to_object_space (fvec3 * result, const vec_object3 * world, const fvec3 * input)
{
    fvec4_quaternion_unrotate(result, &world->quaternion, input);
    vec3_unscale3(*result, world->scale);
    fvec3_normalize(result, result);
}

inline static void fvec3_direction_to_world_space (fvec3 * result, const vec_object3 * world, const fvec3 * input)
{
    vec3_scale3(*result, world->scale);
    fvec4_quaternion_rotate(result, &world->quaternion, input);
    fvec3_normalize(result, result);
}

void fvec3_combined_line_to_object_space (fvec3_combined_line * result, const vec_object3 * world, const fvec3_combined_line * input);
void fvec3_combined_line_to_world_space (fvec3_combined_line * result, const vec_object3 * world, const fvec3_combined_line * input);

void fvec3_triangle_transfer_object_space (fvec3_triangle * result, const vec_object3 * dst, const vec_object3 * src, const fvec3_triangle * input);

void fvec3_triangle_to_world_space (fvec3_triangle * result, const vec_object3 * world, const fvec3_triangle * input);

void fvec3_triangle_to_world_space (fvec3_triangle * result, const vec_object3 * world, const fvec3_triangle * input);
void fvec3_triangle_to_object_space (fvec3_triangle * result, const vec_object3 * world, const fvec3_triangle * input);

/*
inline static void fvec3_distance_to_object_space (fvec * result, const vec_object3 * world, fvec input)
{
    *result = input / world->scale;
}

inline static void fvec3_distance_to_world_space (fvec * result, const vec_object3 * world, fvec input)
{
    *result = input * world->scale;
}

inline static void fvec3_line_to_object_space (fvec3_line * result, const vec_object3 * object, const fvec3_line * input)
{
    fvec3_point_to_object_space (&result->ray.vertex, object, &input->ray.vertex);
    fvec3_direction_to_object_space (&result->ray.direction, object, &input->ray.direction);
    fvec3_distance_to_object_space (&result->distance, object, input->distance);
}

inline static void fvec3_line_to_world_space (fvec3_line * result, const vec_object3 * object, const fvec3_line * input)
{
    object3_untransform_point (&result->ray.vertex, object, &input->ray.vertex);
    object3_untransform_direction (&result->ray.direction, object, &input->ray.direction);
    object3_untransform_distance (&result->distance, object, input->distance);
}
*/

inline static void fvec3_point_transfer_object_space (fvec3 * result, const vec_object3 * dst, const vec_object3 * src, const fvec3 * input)
{
    fvec3_point_to_world_space(result, src, input);
    fvec3_point_to_object_space(result, dst, result);
}
