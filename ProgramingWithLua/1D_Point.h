#pragma once
class OneD_Point
{
public:
	double x;
public:
	OneD_Point(double x);
	~OneD_Point();
	OneD_Point operator+(const OneD_Point& p);
	virtual double dosomething();
};
