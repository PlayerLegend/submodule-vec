#include "shape3.h"
#include <math.h>
#include "vec4.h"
#include "../log/log.h"

static fvec triangle_four_times_squared_area (const fvec3 * center, const fvec3 * a, const fvec3 * b)
{
    fvec3 ac = { vec3_subtract_init(*a, *center) };
    fvec3 bc = { vec3_subtract_init(*b, *center) };
    fvec3 cross = { vec3_cross_init(ac,bc) };
    return vec3_dot(cross, cross);
}

bool fvec3_triangle_contains_point (const fvec3_triangle * tri, fvec3 * point)
{
    fvec full_area = triangle_four_times_squared_area(&tri->vertex[0], &tri->vertex[1], &tri->vertex[2]);
    
    fvec alpha_squared = triangle_four_times_squared_area(point, &tri->vertex[1], &tri->vertex[2]) / full_area;

    assert (alpha_squared >= 0);

    if (alpha_squared > 1)
    {
	return false;
    }
    
    fvec beta_squared = triangle_four_times_squared_area(point, &tri->vertex[2], &tri->vertex[0]) / full_area;

    assert (beta_squared >= 0);

    if (beta_squared > 1)
    {
	return false;
    }

    if (sqrt(alpha_squared) + sqrt(beta_squared) > 1)
    {
	return false;
    }
    
    return true;
}

void fvec3_ray_triangle_intersect_distance (fvec3_ray_intersect * intersect, const fvec3_triangle * world)
{
    // Moller-Trumbore algorithm - backface culling version

    const fvec3_ray * path = intersect->path;
    
    fvec3 edge1 = { vec3_subtract_init (world->vertex[1], world->vertex[0]) };
    fvec3 edge2 = { vec3_subtract_init (world->vertex[2], world->vertex[0]) };
    
    fvec3 p = { vec3_cross_init(path->direction, edge2) };
    fvec determinate = vec3_dot (edge1, p);

    if (determinate < FVEC_EPSILON)
    {
	goto miss;
    }
    
    fvec3 tvec = { vec3_subtract_init(path->begin, world->vertex[0]) };

    fvec u = vec3_dot (tvec, p);

    if (u < 0.0 || u > determinate)
    {
	goto miss;
    }
    
    fvec3 q = { vec3_cross_init (tvec, edge1) };

    fvec v = vec3_dot (path->direction, q);

    if (v < 0.0 || u + v > determinate)
    {
	goto miss;
    }

    intersect->distance = vec3_dot (edge2, q) / determinate;

    if (intersect->distance < 0)
    {
	goto miss;
    }
    //assert (intersect->distance >= -FVEC_EPSILON);
    
    intersect->hit = true;
    
    return;

miss:
    intersect->hit = false;
    return;
}

/*
void fvec3_alligned_ellipsoid_unit_sphere_transform_point (fvec3 * output, const fvec3_aligned_ellipsoid * world, const fvec3 * input)
{
    *output = *input;

    vec3_subtract (*output, *world->center);

    output->x /= world->radius.x;
    output->y /= world->radius.y;
    output->z /= world->radius.z;

    vec3_add (*output, *world->center);
}

void fvec3_aligned_ellipsoid_unit_sphere_transform_triangle (fvec3_triangle * result, fvec3_aligned_ellipsoid * world, fvec3_triangle * input)
{
    fvec3_alligned_ellipsoid_unit_sphere_transform_point(result->vertex, world, input->vertex);
    fvec3_alligned_ellipsoid_unit_sphere_transform_point(result->vertex + 1, world, input->vertex + 1);
    fvec3_alligned_ellipsoid_unit_sphere_transform_point(result->vertex + 2, world, input->vertex + 2);

    fvec3 a = { vec3_subtract_init(result->vertex[0], result->vertex[1]) };
    fvec3 b = { vec3_subtract_init(result->vertex[0], result->vertex[2]) };

    result->normal = (fvec3) { vec3_cross_init (a, b) };

    fvec cross_scale = 1.0 / fvec3_vlen(&result->normal);

    vec3_scale (result->normal, cross_scale);
    }*/

void fvec3_unit_sphere_point_transform_triangle (fvec3_triangle * result, fvec3_triangle * input)
{
    *result = *input;

    vec3_add (result->vertex[0], result->normal);
    vec3_add (result->vertex[1], result->normal);
    vec3_add (result->vertex[2], result->normal);
}

/*
static fvec paralellogram_area (const fvec3 * center, const fvec3 * a, const fvec3 * b)
{
    fvec3 ac = vec3_subtract_init(*a, *center);
    fvec3 bc = vec3_subtract_init(*b, *center);
    fvec3 cross = vec3_cross(ac,bc);
    return fvec3_vlen (&cross);
}

static bool fvec3_triangle_contains_point (const fvec3_triangle * tri, fvec3 * point)
{
    fvec twice_triangle_area = paralellogram_area(&tri->edge[0].vertex, &tri->edge[1].vertex, &tri->edge[2].vertex);
    
    fvec alpha = paralellogram_area(point, &tri->edge[1].vertex, &tri->edge[2].vertex) / twice_triangle_area;
    if (alpha < 0 || alpha > 1)
    {
	return false;
    }

    fvec beta = paralellogram_area(point, &tri->edge[2].vertex, &tri->edge[0].vertex) / twice_triangle_area;

    if (beta < 0 || beta > 1)
    {
	return false;
    }

    fvec gamma = 1 - alpha - beta;

    if (gamma < 0 || gamma > 1)
    {
	return false;
    }

    return true;
}
*/

/*
void fvec3_ray_transform (fvec3_ray * result, const fvec3_ray * world, const fvec3_ray * input)
{
    fvec3 position_delta = (fvec3){ vec3_subtract_init(input->vertex, world->vertex) };
    fvec dot = vec3_dot_unit_z(world->direction);
    
    if (dot > -1 + FVEC_EPSILON)
    {
	fvec4 rotation = { vec3_cross_init_unit_z(world->direction), .w = 1 + dot };
	fvec4_normalize (&rotation, &rotation);
	vec4_quaternion_rotate(&result->direction, &rotation, &input->direction);
	vec4_quaternion_rotate(&result->vertex, &rotation, &position_delta);
    }
    else // world direction is { 0, 0, -1 }
    {
	result->direction = (fvec3){ vec3_negate_init(input->direction) };
	result->vertex = (fvec3){ vec3_negate_init(position_delta) };
    }
    }*/

/*
void fvec3_line_from_points (fvec3_line * result, const fvec3 * begin, const fvec3 * end)
{
    const fvec3 delta = { vec3_subtract_init (*end, *begin) };

    result->distance = fvec3_vlen (&delta);

    const fvec invlength = 1.0 / result->distance;

    result->ray.direction = (fvec3){ vec3_scale_init(delta, invlength) };

    result->ray.vertex = *begin;
    }*/

void fvec3_combined_line_from_point_line (fvec3_combined_line * result, const fvec3_point_line * input)
{
    result->begin = input->begin;
    result->end = input->end;
    
    fvec3 delta = { vec3_subtract_init(result->end, result->begin) };
    
    result->distance = fvec3_vlen(&delta);

    fvec invdistance = 1.0 / result->distance;
    result->ray.direction = (fvec3) { vec3_scale_init(delta, invdistance) };
}

void fvec3_ray_cylinder_intersect_distance (fvec3_ray_intersect * intersect, const fvec3_ray * cylinder_axis, fvec cylinder_radius)
{
    const fvec3_ray * path = intersect->path;
    
    fvec3 point_delta = { vec3_subtract_init(path->begin, cylinder_axis->begin) };

    fvec point_delta_dot_cylinder_normal = vec3_dot(point_delta, cylinder_axis->direction);
    fvec line_normal_dot_cylinder_normal = vec3_dot(path->direction, cylinder_axis->direction);

    fvec quad_a = vec3_dot(path->direction, path->direction) - line_normal_dot_cylinder_normal * line_normal_dot_cylinder_normal;

    if (quad_a * quad_a < FVEC_EPSILON)
    {
	goto miss;
    }
    
    fvec quad_b = 2 * vec3_dot(path->direction, point_delta) - 2 * point_delta_dot_cylinder_normal * line_normal_dot_cylinder_normal;
    fvec quad_c = vec3_dot(point_delta, point_delta) - point_delta_dot_cylinder_normal * point_delta_dot_cylinder_normal - cylinder_radius * cylinder_radius;
    
    fvec quad_discriminate = quad_b * quad_b - 4 * quad_a * quad_c;

    if (quad_discriminate < 0)
    {
	goto miss;
    }

    fvec sqrt_quad_discriminate = sqrt(quad_discriminate);

    fvec inv_2a = 1.0 / (2.0 * quad_a);

    fvec x1 = (-quad_b + sqrt_quad_discriminate) * inv_2a;

    if (x1 < 0)
    {
	goto miss;
    }
    
    fvec x2 = (-quad_b - sqrt_quad_discriminate) * inv_2a;

    if (x2 < 0)
    {
	goto miss;
    }

    intersect->distance = x1 < x2 ? x1 : x2;

    intersect->hit = true;

    return;
    
miss:
    intersect->hit = false;
    return;
    
    /*
    fvec3 endpoint = { vec3_ray_point(sweep->path.ray, distance) };

    fvec3 endpoint_delta = { vec3_subtract_init(endpoint, cylinder_axis->begin) };

    fvec hit_height = vec3_dot(endpoint_delta, cylinder_axis->ray.direction);

    if (hit_height < 0 || hit_height > cylinder_axis->distance)
    {
	return;
    }

    // todo: add normal

    if (fvec3_sweep_distance_collides(*sweep, distance))
    {
	fvec3_sweep_update_without_normal(sweep, distance);
	}*/
}

void fvec3_ray_sphere_intersect_distance (fvec3_ray_intersect * intersect, const fvec3 * sphere_position, fvec sphere_radius)
{
    const fvec3_ray * path = intersect->path;
    
    fvec3 position_transform = { vec3_subtract_init(path->begin, *sphere_position) };

    fvec quad_a = vec3_dot(path->direction, path->direction);
    fvec quad_b = 2 * vec3_dot(path->direction, position_transform);
    fvec quad_c = vec3_dot(position_transform, position_transform) - sphere_radius * sphere_radius;

    fvec quad_discriminate = quad_b * quad_b - 4 * quad_a * quad_c;

    if (quad_discriminate < 0)
    {
	goto miss;
    }

    fvec sqrt_quad_discriminate = sqrt(quad_discriminate);

    fvec inv_2a = 1.0 / (2.0 * quad_a);

    fvec x1 = (-quad_b + sqrt_quad_discriminate) * inv_2a;

    if (x1 < 0)
    {
	goto miss;
    }
    
    fvec x2 = (-quad_b - sqrt_quad_discriminate) * inv_2a;

    if (x2 < 0)
    {
	goto miss;
    }

    intersect->distance = x1 < x2 ? x1 : x2;

    intersect->hit = true;

    return;

miss:
    intersect->hit = false;
    return;
    
    /*
    if (!fvec3_sweep_distance_collides(*sweep, distance))
    {
	return;
    }

    fvec3_sweep_update_without_normal(sweep, distance);
        
    fvec3 endpoint = { vec3_ray_point(sweep->path.ray, distance) };

    fvec3 endpoint_delta = { vec3_subtract_init(endpoint, *sphere_position) };

    fvec3_normalize(&sweep->hit_normal, &endpoint_delta);*/
}

void fvec3_triangle_set_normal (fvec3_triangle * target)
{
    fvec3 a = { vec3_subtract_init(target->vertex[0], target->vertex[1]) };
    fvec3 b = { vec3_subtract_init(target->vertex[0], target->vertex[2]) };

    fvec3 cross = { vec3_cross_init (a, b) };

    fvec cross_scale = 1.0 / fvec3_vlen(&cross);

    vec3_scale (cross, cross_scale);

    target->normal = cross;
}
