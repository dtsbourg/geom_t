#ifndef BEZIER_H
#define BEZIER_H

#define PT_LEN 		20
#define GRID_LARGE  0.05
#define GRID_MEDIUM 0.01
#define GRID_FINE   0.005

typedef struct {
    int x;
    int y;
} point_t;

//Curve is an array of PT_LEN points
typedef point_t curve_t[PT_LEN];

//Linear interpolation
// t ∈ [0,1]
void lerp(point_t *dest, const point_t a, const point_t b, const float t);

//Evaluate a point on the curve
void cubic_bezier(point_t *dest, point_t a, point_t b, point_t c, point_t d, float t);
void quad_bezier(point_t *dest, point_t a, point_t b, point_t c, float t);

//Computes quadratic Bezier curve and stores it in curve
void eval_quad_curve(curve_t curve, point_t a, point_t b, point_t c);

#endif

