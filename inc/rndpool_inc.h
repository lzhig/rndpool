#pragma once
#include <memory>

class rndpool
{
public:
	virtual int random() = 0;
	virtual int random(int min, int max) = 0;
};

rndpool* get_rndpool();