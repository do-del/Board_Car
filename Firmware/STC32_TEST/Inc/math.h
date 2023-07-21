#ifndef MATH_H
#define MATH_H

#include "stc32g.h"
#include "intrins.h"

#define Abs(x) ((x) > 0 ? (x) : -(x))
#define Map(a, b, c, d, x) (((x) - (a)) * ((d) - (c)) / ((b) - (a)) + (c))
#define Limit(x, a) ((x) < (a) ? 0 : (x))

#endif
