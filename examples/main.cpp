#include "..\inc\rndpool_inc.h"
#include <iostream>

void main()
{
	auto pool = get_rndpool();
	//
	auto a = pool->random();
	std::cout << a << std::endl;
}