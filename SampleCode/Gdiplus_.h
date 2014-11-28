#pragma once
class Gdiplus_
{
private:
	class Impl;
	friend class Impl;
	Impl*		impl;

public:
	Gdiplus_();
	~Gdiplus_();

	void doSomething();

};

void countu();