#pragma once
#include "1D_Point.h"
class TwoD_Point:public OneD_Point
{
private:
	double y;
public:
	TwoD_Point(double x, double y);
	~TwoD_Point();
	double dosomething();
};
