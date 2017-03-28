#include "../inc/rndpool_inc.h"
#include <iostream>

int main()
{
	auto pool = get_rndpool();
	//
	auto a = pool->random();
	std::cout << a << std::endl;

    return 0;
}
