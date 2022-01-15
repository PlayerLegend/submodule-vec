#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "vec.h"
#include "vec3.h"
#include "vec4.h"
#include "../keyargs/keyargs.h"
#endif

typedef fvec mat4[16];
typedef struct mat4_swap mat4_swap;
struct mat4_swap {
    int result_index;
    mat4 contents[3];
};

void mat4_swap_init_identity (mat4_swap * swap);
void add_translation_matrix (mat4 input, fvec3 tl);

void mat4_multiply (mat4 * result, const mat4 * a, const mat4 * b);

void mat4_multiply_right_vec (fvec4 * result, const mat4 * a, const fvec4 * b);

// based on the information found here https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
#define mat4_setup_projection_matrix(...) keyargs_call(mat4_setup_projection_matrix, __VA_ARGS__)
keyargs_declare(void, mat4_setup_projection_matrix,
		mat4 * result;
		fvec fovy;
		fvec aspect;
		fvec near;
		fvec far;);

#define mat4_setup_translation_matrix(...) keyargs_call(mat4_setup_translation_matrix, __VA_ARGS__)
keyargs_declare(void, mat4_setup_translation_matrix,
		mat4 * result;
		fvec3 translation;);

#define mat4_setup_scale_matrix(...) keyargs_call(mat4_setup_scale_matrix, __VA_ARGS__)
keyargs_declare(void, mat4_setup_scale_matrix,
		mat4 * result;
		fvec3 scale;);

#define mat4_setup_rotation_matrix(...) keyargs_call(mat4_setup_rotation_matrix, __VA_ARGS__)
keyargs_declare(void, mat4_setup_rotation_matrix,
		mat4 * result;
		const fvec4 * quaternion;);

#define mat4_setup_transform_matrix(...) keyargs_call(mat4_setup_transform_matrix, __VA_ARGS__)
keyargs_declare(void, mat4_setup_transform_matrix,
		mat4 * result;
		fvec3 translation;
		fvec3 scale;
		fvec3 axis;
		fvec fovy;);

void mat4_setup_identity_matrix (mat4 * a);

void mat4_transpose (mat4 * a);
