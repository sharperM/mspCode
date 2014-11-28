#pragma once
class Webcam
{
protected:
	class Impl;
	friend class Impl;
	Impl*	impl;

private:
	Webcam();

public:
	~Webcam();


};

