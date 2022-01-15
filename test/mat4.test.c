#include <assert.h>
#include <stdbool.h>
#define FLAT_INCLUDES
#include "../vec.h"
#include "../vec3.h"
#include "../../keyargs/keyargs.h"
#include "../../debug/debug.h"
#include "../vec4.h"
#include "../mat4.h"
#include "../../log/log.h"

void print_matrix (mat4 a)
{
    for(int i = 0; i < 4; i++)
    {
	log_normal("%3f %3f %3f %3f",
		   a[i],
		   a[i + 4],
		   a[i + 8],
		   a[i + 12]);
    }
}

void test_transpose()
{
    mat4 a =
	{
	    5, 7, 9, 10,
	    2, 3, 3, 8,
	    8, 10, 2, 3,
	    3, 3, 4, 8,
	};

    mat4 expect =
	{
	    5, 2, 8, 3,
	    7, 3, 10, 3,
	    9, 3, 2, 4,
	    10, 8, 3, 8,
	};

    log_debug("pre transpose");
    print_matrix (a);
    
    mat4_transpose (&a);

    log_debug("post transpose");
    print_matrix (a);

    log_debug("expect");
    print_matrix (expect);

    for (int i = 0; i < 16; i++)
    {
	if (a[i] != expect[i])
	{
	    log_fatal ("Transpose matrices differ at index %d, %f != %f", i, a[i], expect[i]);
	}
    }

    return;

fail:
    assert (false);
}

void test_multiply()
{
    
    mat4 a =
	{
	    5, 7, 9, 10,
	    2, 3, 3, 8,
	    8, 10, 2, 3,
	    3, 3, 4, 8,
	};

    mat4 b =
	{
	    3, 10, 12, 18,
	    12, 1, 4, 9,
	    9, 10, 12, 2,
	    3, 12, 4, 10,
	};

    mat4 expect =
	{
	    210, 267, 236, 271,
	    93, 149, 104, 149,
	    171, 146, 172, 268,
	    105, 169, 128, 169,
	};

    mat4 result = {0};

    mat4_transpose (&a);
    mat4_transpose (&b);
    
    mat4_multiply(&result, &a, &b);
    
    log_normal ("Result:");
    mat4_transpose(&result);
    print_matrix(result);
    mat4_transpose(&result);

    for (int i = 0; i < 16; i++)
    {
	if (result[i] != expect[i])
	{
	    log_fatal ("Matrices differ at index %d, %f != %f", i, result[i], expect[i]);
	}
    }

    return;
    
fail:
    assert(false);
}

void test_multiply_right_vec()
{
    fvec4 v = { 2, 3, 5, 7 };
    
    mat4 a =
	{
	    5, 7, 9, 10,
	    2, 3, 3, 8,
	    8, 10, 2, 3,
	    3, 3, 4, 8,
	};

    mat4_transpose(&a);
    
    fvec4 result = {0};

    mat4_multiply_right_vec(&result, &a, &v);

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
