#pragma once
class RockText
{
private:
	class Impl;
	friend class Impl;
	Impl *impl;
public:
	RockText();
	~RockText();
};

