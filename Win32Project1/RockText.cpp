#include "stdafx.h"
#include "RockText.h"
#include <winsock.h>
#pragma comment (lib, "Ws2_32.lib")
class RockText::Impl
{
private:
	RockText * _this;
public:
	Impl(RockText *_this)
	{
		this->_this = _this;
	}
};
RockText::RockText()
{
	impl = new Impl(this);
}


RockText::~RockText()
{
	if (impl)
	{
		delete impl;
	}
}
