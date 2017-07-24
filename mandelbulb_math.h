#ifndef HEADER_MANDELBULB_MATH_DEFINED
#define HEADER_MANDELBULB_MATH_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct
    {
        double x; double y; double z;
    } Point;

    int equals(double, double);

#ifdef __cplusplus
}
#endif

#endif
