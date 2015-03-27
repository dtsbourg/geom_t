#include "bezier.h"
#include "stdio.h"


void lerp(point_t *dest, const point_t a, const point_t b, const float t)
{
	dest->x = a.x + (b.x-a.x)*t;
	dest->y = a.y + (b.y-a.y)*t;
}

void cubic_bezier(point_t *dest, point_t a, point_t b, point_t c, point_t d, float t)
{
	//1 Interpolate lines ab bc and cd
	point_t ab, bc, cd;
	lerp(&ab, a, b, t);
	lerp(&bc, b, c, t);
	lerp(&cd, c, d, t);

	//2 Interpolate lines abbc and bccd
	point_t abbc, bccd;
	lerp(&abbc, ab, bc, t);
	lerp(&bccd, bc, cd, t);

	//3 Interpolate dest
	lerp(dest, abbc, bccd, t);
}

void quad_bezier(point_t *dest, point_t a, point_t b, point_t c, float t)
{
	//1 Interpolate lines ab bc
	point_t ab, bc;
	lerp(&ab, a, b, t);
	lerp(&bc, b, c, t);

	//2 Interpolate dest
	lerp(dest, ab, bc, t);
}

void eval_quad_curve(curve_t curve, point_t a, point_t b, point_t c)
{
	point_t de;

	for (float t=0;t<1;t+=GRID_LARGE)
	{
		quad_bezier(&de, a, b, c, t);
		curve[(int)(t*PT_LEN)] = de;
	}
}

int main ()
{
	point_t a = { .x =   0 , .y =   0 };
	point_t b = { .x = 200 , .y = 200 };
	point_t c = { .x = 200 , .y = 200 };
	point_t d = { .x = 300 , .y = 300 };

	point_t aq = { .x =   0 , .y =   0 };
	point_t bq = { .x =   0 , .y = 100 };
	point_t cq = { .x = 100 , .y = 100 };

	FILE * fp;
	curve_t q_curve;

	fp = fopen("../data/bezier_quad2.dat","w");

	eval_quad_curve(q_curve, aq, bq, cq);
	
	for (int i=0;i<PT_LEN;i++)
		fprintf(fp, "%d %d\n", q_curve[i].x, q_curve[i].y);
	
	fclose(fp);
	return 0;
}