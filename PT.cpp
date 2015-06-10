#include "PT.h"


float distPT(pt a, pt b)
{
	return sqrt( (a.a - b.a)*(a.a - b.a) + (a.b - b.b)*(a.b - b.b) + (a.c - b.c)*(a.c - b.c) );
}

float dist2( pt a, Vec3b b)
{
	return (a.a - b[0])*( a.a - b[0]) + ( a.b - b[1])*( a.b - b[1]) + (a.c - b[2])*(a.c - b[2]);
}