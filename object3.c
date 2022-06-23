#include "object3.h"
#include "../log/log.h"

/*
void object3_transform_line (fvec3_line * result, const vec_object3 * object, const fvec3_line * trace)
{
    result->vertex = (fvec3) vec3_subtract_init(trace->vertex, object->position);
    vec4_quaternion_unrotate(&result->vertex, &object->quaternion, &result->vertex);
    vec4_quaternion_unrotate(&result->direction, &object->quaternion, &trace->direction);
    result->distance = trace->distance / object->scale;
}

void object3_untransform_line (phys_trace_result * result, const vec_object3 * object, const phys_trace_result * input)
{
    result->distance = input->distance * object->scale;
    vec4_quaternion_rotate(&result->hit_normal, &object->quaternion, &input->hit_normal);
    vec4_quaternion_rotate(&result->end, &object->quaternion, &input->end);
    vec3_add(result->end, object->position);
}
*/

#define TESTING
static bool vec_equals(fvec a, fvec b)
{
    fvec delta = a - b;

    return delta > -0.1 && delta < 0.1;
}

static bool vec3_equals(const fvec3 * a, const fvec3 * b)
{
    return vec_equals(a->x, b->x) && vec_equals(a->y, b->y) && vec_equals(a->z, b->z);
}

void combined_line_assert(const fvec3_combined_line * a, const fvec3_combined_line * b)
{
    assert (vec3_equals(&a->begin, &b->begin));
    assert (vec3_equals(&a->end, &b->end));
    if (!vec3_equals(&a->ray.direction, &b->ray.direction))
    {
	log_debug("begin compare " VEC3_FORMSPEC " vs " VEC3_FORMSPEC, VEC3_FORMSPEC_ARG(a->begin), VEC3_FORMSPEC_ARG(b->begin));
	log_debug("end compare " VEC3_FORMSPEC " vs " VEC3_FORMSPEC, VEC3_FORMSPEC_ARG(a->end), VEC3_FORMSPEC_ARG(b->end));
	log_debug("direction compare " VEC3_FORMSPEC " vs " VEC3_FORMSPEC, VEC3_FORMSPEC_ARG(a->ray.direction), VEC3_FORMSPEC_ARG(b->ray.direction));
    }
    assert (vec3_equals(&a->ray.direction, &b->ray.direction));
}

//#define TEST_WORLD

#ifndef TEST_WORLD
#define TEST_OBJECT
#endif

void fvec3_combined_line_to_object_space (fvec3_combined_line * result, const vec_object3 * world, const fvec3_combined_line * input)
{
    #ifndef TEST_OBJECT
    fvec3_combined_line original = *input;
    #endif
    
    fvec3_point_line result_points;
    fvec3_point_to_object_space(&result_points.begin, world, &input->begin);
    fvec3_point_to_object_space(&result_points.end, world, &input->end);

    //log_debug("object space points (" VEC3_FORMSPEC ") (" VEC3_FORMSPEC ")", VEC3_FORMSPEC_ARG(result_points.begin), VEC3_FORMSPEC_ARG(result_points.end));

    fvec3_combined_line_from_point_line(result, &result_points);

    #ifndef TEST_OBJECT
    fvec3_combined_line revert;
    fvec3_combined_line_to_world_space(&revert, world, result);
    combined_line_assert(&revert, &original);
    #endif
}

static void fvec3_points_normal (fvec3 * result, const fvec3 * begin, const fvec3 * end)
{
    fvec3 delta = { vec3_subtract_init(*end, *begin) };
    fvec3_normalize(result, &delta);
}

static void verify_normal (const fvec3_combined_line * test)
{
    fvec3 normal;

    fvec3_points_normal(&normal, &test->begin, &test->end);

    assert (vec3_equals(&normal, &test->ray.direction));
}

void fvec3_combined_line_to_world_space (fvec3_combined_line * result, const vec_object3 * world, const fvec3_combined_line * input)
{
    verify_normal(input);
    #ifndef TEST_WORLD
    fvec3_combined_line original = *input;
    #endif
    
    fvec3_point_line result_points;
    fvec3_point_to_world_space(&result_points.begin, world, &input->begin);
    fvec3_point_to_world_space(&result_points.end, world, &input->end);

    fvec3_combined_line_from_point_line(result, &result_points);
    
    verify_normal(input);
    verify_normal(result);
    
    #ifndef TEST_WORLD
    fvec3_combined_line revert;
    assert (&revert.begin == &revert.ray.begin);
    assert (vec3_equals(&result_points.begin, &result->begin));
    assert (vec3_equals(&result_points.end, &result->end));
    fvec3_combined_line_to_object_space(&revert, world, result);
    verify_normal(&revert);
    assert (vec3_equals(&revert.begin, &original.begin));
    assert (vec3_equals(&revert.end, &original.end));
    fvec3 delta = { vec3_subtract_init(revert.end, revert.begin) };
    fvec3_normalize(&original.ray.direction, &delta);
    combined_line_assert(&revert, &original);
    #endif
}

void fvec3_triangle_transfer_object_space (fvec3_triangle * result, const vec_object3 * dst, const vec_object3 * src, const fvec3_triangle * input)
{
    fvec3_point_to_world_space(result->vertex, src, input->vertex);
    fvec3_point_to_world_space(result->vertex + 1, src, input->vertex + 1);
    fvec3_point_to_world_space(result->vertex + 2, src, input->vertex + 2);

    fvec3_point_to_object_space(result->vertex, dst, result->vertex);
    fvec3_point_to_object_space(result->vertex + 1, dst, result->vertex + 1);
    fvec3_point_to_object_space(result->vertex + 2, dst, result->vertex + 2);

    fvec3 edge[2] =
	{
	    { vec3_subtract_init(result->vertex[0], result->vertex[1]) },
	    { vec3_subtract_init(result->vertex[0], result->vertex[2]) },
	};

    result->normal = (fvec3){ vec3_cross_init(edge[0], edge[1]) };
    fvec3_normalize(&result->normal, &result->normal);
}

void fvec3_triangle_to_world_space (fvec3_triangle * result, const vec_object3 * world, const fvec3_triangle * input)
{
    fvec3_point_to_world_space(result->vertex, world, input->vertex);
    fvec3_point_to_world_space(result->vertex + 1, world, input->vertex + 1);
    fvec3_point_to_world_space(result->vertex + 2, world, input->vertex + 2);

    fvec3_triangle_set_normal (result);
}

void fvec3_triangle_to_object_space (fvec3_triangle * result, const vec_object3 * world, const fvec3_triangle * input)
{
    fvec3_point_to_object_space(result->vertex, world, input->vertex);
    fvec3_point_to_object_space(result->vertex + 1, world, input->vertex + 1);
    fvec3_point_to_object_space(result->vertex + 2, world, input->vertex + 2);

    fvec3_triangle_set_normal (result);
}
