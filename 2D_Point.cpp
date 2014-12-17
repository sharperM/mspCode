#include "TwoD_Point.h"
#include <cmath>
TwoD_Point::TwoD_Point(double x, double y)
:OneD_Point(x), y(y)
{
	cout << "TwoD_Point()" << endl;
}

TwoD_Point::~TwoD_Point()
{
	cout << "~TwoD_Point()" << endl;

}
double TwoD_Point::dosomething()
{
	return sqrt(x*x + y*y);
}