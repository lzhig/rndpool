#include "../inc/rndpool_inc.h"
#include <iostream>
#include <algorithm>

int main()
{
	auto pool = get_rndpool();
#define LOOP_SIZE 100
#define SIZE 20
	int a[SIZE] = { 0 };
	//
	for (int i = 0; i < LOOP_SIZE; i++)
	{
		a[pool->random(0, SIZE)]++;
	}
	std::sort(std::begin(a), std::end(a));
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << i << ":" << ((float)a[i])/(LOOP_SIZE/SIZE) << std::endl;
	}
	std::cout << std::endl;
	while (1) {}

    return 0;
}
