```c
#include <math.h>
```

linkin while compiling:

```bash
cc main.c -o main -lm
```

### ProtoTypes

```c
double acos(double x); // arc consine
double asin(double x); // arc sine
double atan(double x); // arc tangent
double atan2(double y, double x); // arc tangent of y/x
double cos(double x); // cosine in randians
double cosh(double x); // hyperbolic cosine 
...

double ceil(double x); // smalest integral value that exceeds x
double floor(double x); // largest integral value less than x

typedef struct {
	int quot; /* quotient */
	int rem;  /* remainder */
} div_t;
div_t div(int num, int denom);
ldiv_t ldiv(long num, long denom);

double sqrt(double x); // square root of x
double exp(double x, );
double pow(double x, double y); // get x raised to the power y
double log(double x);   // log(x)
double log10(double x); // log_10(x)

double fmod(double x, double y); // x/y, return remainder
double frexp(double x, int *expptr); //
// break x into fractional and integer parts
double modf(double x, double *intptr); 

// get absolute value
abs(int x)
labs(long x)
double fabs(double x)
```

### Constants

```c
HUGE    // maximum of a single-precision floating-point number
M_E     // e, base of natural logarithms
M_LOG2E // base-2 logarithm of e
M_LOG10E
M_LN2   // natural logarithm of 2
M_LN10
M_PI    // pi
M_PI_2  // pi/2
M_PI_4  // pi/4
M_1_PI  // 1/pi
M_2_PI  // 2/pi
M_2_SQRTPI // 2/sqrt(pi)
HUGE_VAL // positive infinity
MAXFLOAT // maximum of a non-infinite single-precision floating point number
```

还有一部分常数定义在 `value.h`, 详情见 `man value`