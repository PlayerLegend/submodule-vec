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

/*void apply_translation (mat4 input, fvec3 * tl)
{
#define tl_matrix_unwrap(row) input[12 + row] = input[0 + row]*tl->x + input[4 + row]*tl->y + input[8 + row]*tl->z + input[12 + row];
    tl_matrix_unwrap(0);
    tl_matrix_unwrap(1);
    tl_matrix_unwrap(2);
    tl_matrix_unwrap(3);
    }*/

keyargs_define(mat4_setup_rotation_matrix)
{
    // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
     
    float s = 1 / (+ args.quaternion->x * args.quaternion->x
		   + args.quaternion->y * args.quaternion->y
		   + args.quaternion->z * args.quaternion->z
		   + args.quaternion->w * args.quaternion->w);

    s = 1;
    
    float s2 = 2 * s;

    *args.result = (mat4){
	.column = {
	    {
		.x  = 1 - s2 * (args.quaternion->y * args.quaternion->y + args.quaternion->z * args.quaternion->z),
		.y  = s2 * (args.quaternion->x * args.quaternion->y + args.quaternion->z * args.quaternion->w),
		.z  = s2 * (args.quaternion->x * args.quaternion->z - args.quaternion->y * args.quaternion->w),
		.w  = 0,
	    },
	    {
		.x  = s2 * (args.quaternion->x * args.quaternion->y - args.quaternion->z * args.quaternion->w),
		.y  = 1 - s2 * (args.quaternion->x * args.quaternion->x + args.quaternion->z * args.quaternion->z),
		.z  = s2 * (args.quaternion->y * args.quaternion->z + args.quaternion->x * args.quaternion->w),
		.w  = 0,
	    },
	    {
		.x  = s2 * (args.quaternion->x * args.quaternion->z + args.quaternion->y * args.quaternion->w),
		.y  = s2 * (args.quaternion->y * args.quaternion->z - args.quaternion->x * args.quaternion->w),
		.z = 1 - s2 * (args.quaternion->x * args.quaternion->x + args.quaternion->y * args.quaternion->y),
		.w = 0,
	    },
	    {
		.x = 0,
		.y = 0,
		.z = 0,
		.w = 1,
	    },
	},
    };
    
    

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

    *args.result = (mat4)
	{
	    .column =
	    {
		{
		    .x = f / args.aspect,
		},
		{
		    .y = f,
		},
		{
		    .z = (args.far + args.near) / (args.near - args.far),
		    .w = -1,
		},
		{
		    .z = (2 * args.far * args.near) / (args.near - args.far),
		},
	    }
	};
}

keyargs_define(mat4_setup_translation_matrix)
{
    *args.result = (mat4)
    {
	.column =
	{
	    { .x = 1 },
	    { .y = 1 },
	    { .z = 1 },
	    {
		.x = args.translation.x,
		.y = args.translation.y,
		.z = args.translation.z,
		.w = 1,
	    }
	}
    };
}

keyargs_define(mat4_setup_scale_matrix)
{
    *args.result = (mat4)
    {
	.column =
	{
	    { .x = args.scale.x },
	    { .y = args.scale.y },
	    { .z = args.scale.z },
	    { .w = 1 },
	},
    };
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

/*inline static float matrix_multiply_dot (int row_index, int col_index, const mat4 * a, const mat4 * b)
{
    return
	+ a->column[0].index[row_index] * b->column[col_index].x
	+ a->column[1].index[row_index] * b->column[col_index].y
	+ a->column[2].index[row_index] * b->column[col_index].z
	+ a->column[3].index[row_index] * b->column[col_index].w;
    
    const fvec * row = (*a) + row_index;
    const fvec * col = (*b) + 4 * col_index;

    
    return
	+ row[0] * col[0]
	+ row[4] * col[1]
	+ row[8] * col[2]
	+ row[12] * col[3];
}*/

#define mat4_dot_column_vec(a, member, v)			\
    + (a).column[0].member * (v).x				\
    + (a).column[1].member * (v).y				\
    + (a).column[2].member * (v).z				\
    + (a).column[3].member * (v).w

#define mat4_dot_column(a, v)		\
	.x = mat4_dot_column_vec(a, x, v),	\
	    .y = mat4_dot_column_vec(a, y, v),	\
	    .z = mat4_dot_column_vec(a, z, v),	\
	    .w = mat4_dot_column_vec(a, w, v)

void mat4_multiply_column_vec (fvec4 * result, const mat4 * a, const fvec4 * b)
{
    *result = (fvec4){ mat4_dot_column(*a, *b) };
}

void mat4_multiply (mat4 * result, const mat4 * a, const mat4 * b)
{
    *result = (mat4)
	{
	    .column =
	    {
		{ mat4_dot_column(*a, b->column[0]) },
		{ mat4_dot_column(*a, b->column[1]) },
		{ mat4_dot_column(*a, b->column[2]) },
		{ mat4_dot_column(*a, b->column[3]) },
	    }
	};
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
	    xp = a->column[x].index + y;
	    yp = a->column[y].index + x;

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

void mat4_setup_identity_matrix (mat4 * target)
{
    *target = (mat4)
    {
	.column =
	{
	    { .x = 1 },
	    { .y = 1 },
	    { .z = 1 },
	    { .w = 1 },
	}
    };
}
