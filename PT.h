//定义point 结构，含有rgb值，和距离计划方式

#ifndef PT_H_
#define PT_H_

#include <opencv2/core/core.hpp>

using namespace cv;

struct pt
{
	int a;
	int b; 
	int c;

	pt()
	{
		a = 0;
		b = 0;
		c = 0;
	}

	pt( int _a, int _b, int _c)
	{
		a = _a;
		b = _b;
		c = _c;
	}

	pt operator= ( pt x)
	{
		a = x.a;
		b = x.b;
		c = x.c;

		return pt(a,b,c);
	}


};

struct data
{
	pt a;
	bool isvisited;

	data()
	{
		a = pt(0,0,0);
		isvisited = false;
	}

	data( pt x )
	{
		a = x;
		isvisited = false;
	}

};

float distPT(pt a, pt b);

float dist2( pt a, Vec3b b);

#endif
