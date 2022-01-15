#include <math.h>
#define FLAT_INCLUDES
#include "../vec.h"
#include "../vec2.h"
#include "../vec3.h"
#include "../../debug/debug.h"

/*fvec fabs(fvec f)
{
    return f < 0 ? -f : f;
    }*/

void float_equals(fvec a, fvec b)
{
    fvec small_value = 0.0001;
    assert (fabs(a - b) < small_value);
}

void test_fvec2_add()
{
    fvec2 a = { 1, 2 }, b = { 3, 4 }, expect = { 4, 6 };

    vec2_add(a,b);

    float_equals(a.x, expect.x);
    float_equals(a.y, expect.y);
}

void test_fvec2_subtract()
{
    fvec2 a = { 1, 2 }, b = { 3, 4 }, expect = { -2, -2 };

    vec2_subtract(a,b);

    float_equals(a.x, expect.x);
    float_equals(a.y, expect.y);
}

void test_fvec2_scale()
{
    fvec s = 2.5;
    fvec2 v = { 1, 2 }, expect = { 2.5, 5 };

    vec2_scale(v,s);

    float_equals(v.x, expect.x);
    float_equals(v.y, expect.y);
}

int main()
{
    test_fvec2_add();
    test_fvec2_subtract();
    test_fvec2_scale();

    return 0;
}
