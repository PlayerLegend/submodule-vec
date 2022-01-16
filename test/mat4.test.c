#include <assert.h>
#include <stdbool.h>
#include <assert.h>
#define FLAT_INCLUDES
#include "../vec.h"
#include "../vec3.h"
#include "../../keyargs/keyargs.h"
#include "../vec4.h"
#include "../mat4.h"
#include "../../log/log.h"

void print_vec(fvec4 * v)
{
    log_normal("%3f %3f %3f %3f",
	       v->x,
	       v->y,
	       v->z,
	       v->w);
}

void print_row(mat4 * a, int i)
{
    log_normal("%3f %3f %3f %3f",
	       a->column[0].index[i],
	       a->column[1].index[i],
	       a->column[2].index[i],
	       a->column[3].index[i]);
}

void print_matrix (mat4 * a)
{
    print_row (a, 0);
    print_row (a, 1);
    print_row (a, 2);
    print_row (a, 3);
}

bool matrix_compare (mat4 * a, const char * a_title, mat4 * b, const char * b_title)
{
    log_normal("Comparing matrix %s:", a_title);
    print_matrix(a);
    log_normal("To matrix %s:", b_title);
    print_matrix(b);
    
    for (int col_i = 0; col_i < 4; col_i++)
    {
	for (int row_i = 0; row_i < 4; row_i++)
	{
	    if (a->column[col_i].index[row_i] != b->column[col_i].index[row_i])
	    {
		log_fatal ("Matrices differ at (column, row) (%d, %d) where %f != %f", col_i, row_i, a->column[col_i].index[row_i], b->column[col_i].index[row_i]);
	    }
	}
    }

    log_normal ("Matrices match!");
    log_normal("");
    
    return true;
    
fail:
    return false;
}

void test_transpose()
{
    log_normal ("Testing transpose");
    
    mat4 a =
	{
	    .column =
	    {
		{ .x =  5, .y = 2, .z =  8, .w = 3 },
		{ .x =  7, .y = 3, .z = 10, .w = 3 },
		{ .x =  9, .y = 3, .z =  2, .w = 4 },
		{ .x = 10, .y = 8, .z =  3, .w = 8 },
	    }
	};

    mat4 expect =
	{
	    .column =
	    {
		{ .x = 5, .y = 7,  .z = 9, .w = 10 },
		{ .x = 2, .y = 3,  .z = 3, .w = 8 },
		{ .x = 8, .y = 10, .z = 2, .w = 3 },
		{ .x = 3, .y = 3,  .z = 4, .w = 8 },
	    }
	};

    log_normal ("Before transpose");
    
    print_matrix (&a);
    
    mat4_transpose (&a);

    assert (matrix_compare(&a, "After transpose", &expect, "Expected matrix"));
    
    return;
}

void test_multiply()
{
    mat4 a =
	{
	    .column =
	    {
		{ .x = 5,  .y = 2, .z = 8,  .w = 3 },
		{ .x = 7,  .y = 3, .z = 10, .w = 3 },
		{ .x = 9,  .y = 3, .z = 2,  .w = 4, },
		{ .x = 10, .y = 8, .z = 3,  .w = 8 },
	    }
	};

    mat4 b =
	{
	    .column =
	    {
		{ .x = 3,  .y = 12, .z = 9,  .w = 3 },
		{ .x = 10, .y = 1,  .z = 10, .w = 12 },
		{ .x = 12, .y = 4,  .z = 12, .w = 4 },
		{ .x = 18, .y = 9, . z = 2,  .w = 10 },
	    }
	};

    log_normal ("Multiplying matrix:");
    print_matrix(&a);
    log_normal ("With matrix:");
    print_matrix(&b);

    mat4 expect =
	{
	    .column =
	    {
		{ .x = 210, .y = 93, .z = 171, .w = 105 },
		{ .x = 267, .y = 149, .z = 146, .w = 169 },
		{ .x = 236, .y = 104, .z = 172, .w = 128 },
		{ .x = 271, .y = 149, .z = 268, .w = 169 },
	    }
	};

    mat4 result = {0};

    mat4_multiply(&result, &a, &b);

    assert (matrix_compare(&result, "multiply result", &expect, "expectation"));
}

void test_multiply_right_vec()
{
    fvec4 v = { .x = 2, .y = 3, .z = 5, .w = 7 };
    
    mat4 a =
	{
	    .column =
	    {
		{ .x = 5, .y = 2, .z = 8, .w = 3 },
		{ .x = 7, .y = 3, .z = 10, .w = 3 },
		{ .x = 9, .y = 3, .z = 2, .w = 4 },
		{ .x = 10, .y = 8, .z = 3, .w = 8 },
	    }
	};

    log_normal("Multiplying matrix:");
    print_matrix(&a);
    log_normal("With column vector");
    print_vec(&v);
    
    fvec4 result = {0};

    mat4_multiply_column_vec(&result, &a, &v);

    log_normal ("Result:");
    print_vec(&v);

    //log_normal ("%f * %f + %f * %f + %f * %f + %f * %f", a[0], v.x, a[4] , v.y , a[8] , v.z , a[12] , v.w);
    //log_normal ("result: %f %f %f %f", result.x, result.y, result.z, result.w);
    
    assert (result.x == 146);
    assert (result.y == 84);
    assert (result.z == 77);
    assert (result.w == 91);
}

int main()
{
    test_transpose();
    test_multiply();
    test_multiply_right_vec();
    
    return 0;
}
