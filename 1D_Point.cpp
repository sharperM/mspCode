#include "stdafx.h"
#include "1D_Point.h"
#include <iostream>
using namespace std;
OneD_Point::OneD_Point(int x)
:x(x)
{
	cout << "One_Dimensional_Point()" << endl;
}

OneD_Point::~OneD_Point()
{
	cout << "~One_Dimensional_Point()" << endl;
}

double OneD_Point::dosomething()
{
	return x;
}