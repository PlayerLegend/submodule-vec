#ifndef FLAT_INCLUDES
#include "vec3.h"
#include <stdbool.h>
#include <assert.h>
#endif

typedef struct {
    fvec3 begin;
    fvec3 direction;
}
    fvec3_ray;

typedef struct {
    union {
	fvec3 begin;
	fvec3_ray ray;
    };
    fvec distance;
}
    fvec3_ray_line;

typedef struct {
    fvec3 begin;
    fvec3 end;
}
    fvec3_point_line;

typedef struct {
    union {
	fvec3 begin;
	fvec3_ray ray;
    };
    fvec3 end;
    fvec distance;
}
    fvec3_combined_line;

typedef struct {
    union {
	fvec3 vertex[3];
	fvec3 point;
    };
    fvec3 normal;
}
    fvec3_triangle;

typedef struct {
    bool is_hit;
    fvec3 hit_normal;
    fvec full_distance;
    fvec3_combined_line path;
}
    fvec3_sweep;

#define vec3_ray_point(ray,distance) .x = (ray).begin.x + (ray).direction.x * distance, .y = (ray).begin.y + (ray).direction.y * distance, .z = (ray).begin.z + (ray).direction.z * distance
#define vec3_plane_distance(plane,test) ( (plane).normal.x * ( (test).x - (plane).point.x ) + (plane).normal.y * ( (test).y - (plane).point.y ) + (plane).normal.z * ( (test).z - (plane).point.z ) )
#define vec3_project_distance(ray,test) ( (ray).direction.x * ( (test).x - (ray).begin.x ) + (ray).direction.y * ( (test).y - (ray).begin.y ) + (ray).direction.z * ( (test).z - (ray).begin.z ) )

inline static void fvec3_ray_line_from_points (fvec3_ray_line * result, const fvec3 * begin, const fvec3 * end)
{
    fvec3 delta = { vec3_subtract_init(*end, *begin) };
    result->distance = fvec3_vlen(&delta);
    fvec invdistance = 1.0 / result->distance;
    result->ray.direction = (fvec3) { vec3_scale_init(delta, invdistance) };
    result->ray.begin = *begin;
}

inline static void fvec3_ray_line_from_point_line (fvec3_ray_line * result, const fvec3_point_line * input)
{
    fvec3_ray_line_from_points(result, &input->begin, &input->end);
}

inline static void fvec3_point_line_from_ray_line (fvec3_point_line * result, const fvec3_ray_line * input)
{
    result->begin = input->begin;
    result->end = (fvec3){ vec3_ray_point(input->ray, input->distance ) };
}

void fvec3_combined_line_from_point_line (fvec3_combined_line * result, const fvec3_point_line * input);

inline static void fvec3_combined_line_from_ray_line (fvec3_combined_line * result, const fvec3_ray_line * input)
{
    result->begin = input->begin;
    result->ray.direction = input->ray.direction;
    result->distance = input->distance;
    result->end = (fvec3){ vec3_ray_point(input->ray, input->distance) };
}

inline static void fvec3_combined_line_update_distance (fvec3_combined_line * target, fvec distance)
{
    target->distance = distance;
    target->end = (fvec3){ vec3_ray_point(target->ray, distance) };
}

inline static void fvec3_combined_line_update_end (fvec3_combined_line * target, const fvec3 * end)
{
    target->end = *end;

    fvec3 delta = { vec3_subtract_init(target->end, target->begin) };

    target->distance = fvec3_vlen(&delta);
}

inline static void fvec3_sweep_ray_start (fvec3_sweep * target, fvec3_ray_line * path)
{
    fvec3_combined_line_from_ray_line(&target->path, path);
    target->is_hit = false;
    target->hit_normal = (fvec3){0};
}

inline static void fvec3_sweep_point_start (fvec3_sweep * target, fvec3_point_line * path)
{
    fvec3_combined_line_from_point_line(&target->path, path);
    target->full_distance = target->path.distance;
    target->is_hit = false;
    target->hit_normal = (fvec3){0};
}

inline static void fvec3_sweep_invert (fvec3_sweep * target)
{
    target->path.begin = (fvec3){ vec3_ray_point(target->path.ray, target->full_distance) };
    target->path.ray.direction = (fvec3){ vec3_negate_init(target->path.ray.direction) };
    target->path.end = (fvec3){ vec3_ray_point(target->path.ray, target->path.distance) };
}

inline static void fvec3_sweep_update_without_normal (fvec3_sweep * sweep, fvec distance)
{
    fvec3_combined_line_update_distance(&sweep->path, distance);
    sweep->is_hit = true;
}

inline static void fvec3_sweep_update (fvec3_sweep * sweep, const fvec3 * normal, fvec distance)
{
    fvec3_sweep_update_without_normal (sweep, distance);
    sweep->hit_normal = *normal;
}

#define fvec3_sweep_distance_collides(sweep, distance) ( 0 <= (distance) && (distance) < (sweep).path.distance )

typedef struct {
    bool hit;
    fvec distance;
    const fvec3_ray * path;
}
    fvec3_ray_intersect;

/*typedef struct {
    fvec3 * center;
    fvec3 radius;
}
fvec3_aligned_ellipsoid;*/

/*bool fvec3_triangle_contains_point (const fvec3_triangle * tri, fvec3 * point); // assumes the point is in the triangle's plane
bool fvec3_triangle_ray_intersect_distance (fvec * distance, const fvec3_triangle * world, const fvec3_ray * path);
//void fvec3_aligned_ellipsoid_unit_sphere_transform_triangle (fvec3_triangle * result, fvec3_aligned_ellipsoid * world, fvec3_triangle * input);
void fvec3_unit_sphere_point_transform_triangle (fvec3_triangle * result, fvec3_triangle * input);
void fvec3_ray_transform (fvec3_ray * result, const fvec3_ray * world, const fvec3_ray * input);
void fvec3_line_from_points (fvec3_line * result, const fvec3 * begin, const fvec3 * end);*/

//void fvec3_sweep_point_cylinder (fvec3_sweep * sweep, const fvec3 * cylinder_begin, const fvec3 * cylinder_end, fvec cylinder_radius);
//void fvec3_sweep_point_sphere (fvec3_sweep * sweep, const fvec3 * sphere_position, fvec sphere_radius);

void fvec3_ray_triangle_intersect_distance (fvec3_ray_intersect * intersect, const fvec3_triangle * world);
void fvec3_ray_cylinder_intersect_distance (fvec3_ray_intersect * intersect, const fvec3_ray * cylinder_axis, fvec cylinder_radius);
void fvec3_ray_sphere_intersect_distance (fvec3_ray_intersect * intersect, const fvec3 * sphere_position, fvec sphere_radius);
void fvec3_triangle_set_normal (fvec3_triangle * target);
