#include <string.h>
#include <math.h>
#define FLAT_INCLUDES
#include "vec.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "../keyargs/keyargs.h"
#include "mat4.h"
#include "../log/log.h"

void apply_translation (mat4 input, fvec3 * tl)
{
#define tl_matrix_unwrap(row) input[12 + row] = input[0 + row]*tl->x + input[4 + row]*tl->y + input[8 + row]*tl->z + input[12 + row];
    tl_matrix_unwrap(0);
    tl_matrix_unwrap(1);
    tl_matrix_unwrap(2);
    tl_matrix_unwrap(3);
}

keyargs_define(mat4_setup_rotation_matrix)
{
    // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
    
    
    float s = 1 / (+ args.quaternion->x * args.quaternion->x
		   + args.quaternion->y * args.quaternion->y
		   + args.quaternion->z * args.quaternion->z
		   + args.quaternion->w * args.quaternion->w);

    s = 1;
    
    float s2 = 2 * s;

    (*args.result)[0]  = 1 - s2 * (args.quaternion->y * args.quaternion->y + args.quaternion->z * args.quaternion->z);
    (*args.result)[1]  = s2 * (args.quaternion->x * args.quaternion->y + args.quaternion->z * args.quaternion->w);
    (*args.result)[2]  = s2 * (args.quaternion->x * args.quaternion->z - args.quaternion->y * args.quaternion->w);
    (*args.result)[3]  = 0;

    (*args.result)[4]  = s2 * (args.quaternion->x * args.quaternion->y - args.quaternion->z * args.quaternion->w);
    (*args.result)[5]  = 1 - s2 * (args.quaternion->x * args.quaternion->x + args.quaternion->z * args.quaternion->z);
    (*args.result)[6]  = s2 * (args.quaternion->y * args.quaternion->z + args.quaternion->x * args.quaternion->w);
    (*args.result)[7]  = 0;
    
    (*args.result)[8]  = s2 * (args.quaternion->x * args.quaternion->z + args.quaternion->y * args.quaternion->w);
    (*args.result)[9]  = s2 * (args.quaternion->y * args.quaternion->z - args.quaternion->x * args.quaternion->w);
    (*args.result)[10] = 1 - s2 * (args.quaternion->x * args.quaternion->x + args.quaternion->y * args.quaternion->y);
    (*args.result)[11] = 0;

    (*args.result)[12] = 0;
    (*args.result)[13] = 0;
    (*args.result)[14] = 0;
    (*args.result)[15] = 1;

    /*
    mat4 a;
    a[0] = args.quaternion->w;
    a[1] = -args.quaternion->z;
    a[2] = args.quaternion->y;
    a[3] = -args.quaternion->x;

    a[4] = args.quaternion->z;
    a[5] = args.quaternion->w;
    a[6] = -args.quaternion->x;
    a[7] = -args.quaternion->y;

    a[8] = -args.quaternion->y;
    a[9] = args.quaternion->x;
    a[10] = args.quaternion->w;
    a[11] = -args.quaternion->z;

    a[12] = args.quaternion->x;
    a[13] = args.quaternion->y;
    a[14] = args.quaternion->z;
    a[15] = args.quaternion->w;

    mat4 b;
    b[0] = args.quaternion->w;
    b[1] = -args.quaternion->z;
    b[2] = args.quaternion->y;
    b[3] = args.quaternion->x;

    b[4] = args.quaternion->z;
    b[5] = args.quaternion->w;
    b[6] = -args.quaternion->x;
    b[7] = args.quaternion->y;

    b[8] = -args.quaternion->y;
    b[9] = args.quaternion->x;
    b[10] = args.quaternion->w;
    b[11] = args.quaternion->z;
    
    b[12] = -args.quaternion->x;
    b[13] = -args.quaternion->y;
    b[14] = -args.quaternion->z;
    b[15] = args.quaternion->w;

    mat4_multiply(args.result, &a, &b);*/

    /*(*args.result)[0] = 2 * (q.w * q.w + q.x * q.x) - 1;
    (*args.result)[1] = 2 * (q.w * q.y + q.w * q.z);
    (*args.result)[2] = 2 * (q.x * q.z - q.w * q.y);
    (*args.result)[3] = 0;
    
    (*args.result)[0] = 2 * (q.x * q.y - q.w * q.z);
    (*args.result)[0] = 2 * (q.w * q.w + q.y * q.y) - 1;
    (*args.result)[0] = 2 * (q.y * q.z + q.w * q.x);
    (*args.result)[0] = 0;
    
    (*args.result)[0] = 2 * (q.x * q.z + q.w * q.y);
    (*args.result)[0] = 2 * (q.y * q.z - q.w * q.x);
    (*args.result)[0] = 2 * (q.w * q.w + q.z * q.z) - 1;
    (*args.result)[0] = 0;
    
    (*args.result)[12] = 0;
    (*args.result)[13] = 0;
    (*args.result)[14] = 0;
    (*args.result)[15] = 1;*/
}

keyargs_define(mat4_setup_projection_matrix)
{
    /* https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml */

    float f = args.fovy / 2;
    f = cos (f) / sin (f);

    (*args.result)[0] = f / args.aspect;
    (*args.result)[1] = 0;
    (*args.result)[2] = 0;
    (*args.result)[3] = 0;

    (*args.result)[4] = 0;
    (*args.result)[5] = f;
    (*args.result)[6] = 0;
    (*args.result)[7] = 0;

    (*args.result)[8] = 0;
    (*args.result)[9] = 0;
    (*args.result)[10] = (args.far + args.near) / (args.near - args.far);
    (*args.result)[11] = -1;

    (*args.result)[12] = 0;
    (*args.result)[13] = 0;
    (*args.result)[14] = (2 * args.far * args.near) / (args.near - args.far);
    (*args.result)[15] = 0;
}

keyargs_define(mat4_setup_translation_matrix)
{
    (*args.result)[0] = 1;
    (*args.result)[1] = 0;
    (*args.result)[2] = 0;
    (*args.result)[3] = 0;

    (*args.result)[4] = 0;
    (*args.result)[5] = 1;
    (*args.result)[6] = 0;
    (*args.result)[7] = 0;

    (*args.result)[8] = 0;
    (*args.result)[9] = 0;
    (*args.result)[10] = 1;
    (*args.result)[11] = 0;

    (*args.result)[12] = args.translation.x;
    (*args.result)[13] = args.translation.y;
    (*args.result)[14] = args.translation.z;
    (*args.result)[15] = 1;
}

keyargs_define(mat4_setup_scale_matrix)
{
    (*args.result)[0] = args.scale.x;
    (*args.result)[1] = 0;
    (*args.result)[2] = 0;
    (*args.result)[3] = 0;

    (*args.result)[4] = 0;
    (*args.result)[5] = args.scale.y;
    (*args.result)[6] = 0;
    (*args.result)[7] = 0;

    (*args.result)[8] = 0;
    (*args.result)[9] = 0;
    (*args.result)[10] = args.scale.z;
    (*args.result)[11] = 0;

    (*args.result)[12] = 0;
    (*args.result)[13] = 0;
    (*args.result)[14] = 0;
    (*args.result)[15] = 1;
}

/*keyargs_define(mat4_setup_transform_matrix)
{
    mat4 a, b, c;

    mat4_setup_rotation_matrix(.result = &b, .axis = args.axis);
    mat4_setup_scale_matrix (.result = &c, .scale = args.scale);
    
    mat4_multiply (&a, &b, &c);

    mat4_setup_translation_matrix (.result = &c, .translation = args.translation);

    mat4_multiply (&b, &c, &a);

    mat4_setup_projection_matrix(.result = &a, .fovy = args.fovy, .aspect = 1, .near = 0.01, .far = 1);

    mat4_multiply (args.result, &a, &b);
    }*/

void mat4_init_identity (mat4 mat)
{
    mat[0] = 1;
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;
    
    mat[4] = 0;
    mat[5] = 1;
    mat[6] = 0;
    mat[7] = 0;
    
    mat[8] = 0;
    mat[9] = 0;
    mat[10] = 1;
    mat[11] = 0;
    
    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
    
}

inline static float matrix_multiply_dot (int row_index, int col_index, const mat4 * a, const mat4 * b)
{
    const fvec * row = (*a) + row_index;
    const fvec * col = (*b) + 4 * col_index;

    /*log_normal ("(%d,%d) [%f, %f, %f, %f] * [%f, %f, %f, %f]",
		row_index,
		col_index,
		row[0],
		row[4],
		row[8],
		row[12],
		col[0],
		col[1],
		col[2],
		col[3]);*/
    
    return
	+ row[0] * col[0]
	+ row[4] * col[1]
	+ row[8] * col[2]
	+ row[12] * col[3];
}

void mat4_multiply (mat4 * result, const mat4 * a, const mat4 * b)
{
#define mat4_multiply_column(n)					\
    (*result)[0 + 4 * n] = matrix_multiply_dot(n, 0, a, b);	\
    (*result)[1 + 4 * n] = matrix_multiply_dot(n, 1, a, b);	\
    (*result)[2 + 4 * n] = matrix_multiply_dot(n, 2, a, b);	\
    (*result)[3 + 4 * n] = matrix_multiply_dot(n, 3, a, b);

    mat4_multiply_column(0);
    mat4_multiply_column(1);
    mat4_multiply_column(2);
    mat4_multiply_column(3);

    mat4_transpose(result); // why
}

void mat4_transpose (mat4 * a)
{
    float tmp;

    float * xp;
    float * yp;
    
    for (int x = 0; x < 4; x++)
    {
	for (int y = 0; y < x; y++)
	{
	    xp = (*a) + 4 * x + y;
	    yp = (*a) + 4 * y + x;

	    //log_debug("1 %f %f", *xp, *yp);
	    tmp = *xp;
	    *xp = *yp;
	    *yp = tmp;
	    //log_debug("2 %f %f", *xp, *yp);
	}
    }
}

/*void mat4_multiply (mat4 * out, const mat4 * a, const mat4 * b)
  {
  int row, col;

  for (row = 0; row < 4; row += 1)
  {
  for (col = 0; col < 16; col += 4)
  {
  (*out)[col + row]
  = (*a)[row] * (*b)[col]
  + (*a)[row + 4] * (*b)[col + 1]
  + (*a)[row + 8] * (*b)[col + 2]
  + (*a)[row + 12] * (*b)[col + 3];
  }
  }
  }
*/

void mat4_setup_identity_matrix (mat4 * a)
{
    (*a)[0] = 1;
    (*a)[1] = 0;
    (*a)[2] = 0;
    (*a)[3] = 0;
    
    (*a)[4] = 0;
    (*a)[5] = 1;
    (*a)[6] = 0;
    (*a)[7] = 0;
    
    (*a)[8] = 0;
    (*a)[9] = 0;
    (*a)[10] = 1;
    (*a)[11] = 0;
    
    (*a)[12] = 0;
    (*a)[13] = 0;
    (*a)[14] = 0;
    (*a)[15] = 1;
}

void mat4_multiply_right_vec (fvec4 * result, const mat4 * a, const fvec4 * b)
{
#define right_vec_dot(i)						\
    (*a)[0 + i] * b->x + (*a)[4 + i] * b->y + (*a)[8 + i] * b->z + (*a)[12 + i] * b->w

    result->x = right_vec_dot(0);
    result->y = right_vec_dot(1);
    result->z = right_vec_dot(2);
    result->w = right_vec_dot(3);
}
