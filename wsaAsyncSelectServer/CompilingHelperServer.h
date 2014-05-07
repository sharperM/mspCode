#pragma once
class CompilingHelperServer
{
private:
	friend class Impl;
	class Impl;
	Impl *impl;

private:
	CompilingHelperServer();
public:
	~CompilingHelperServer();
	static  CompilingHelperServer& instance();
};

