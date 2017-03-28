#pragma once

#include "../inc/rndpool_inc.h"

#include <vector>


#ifndef WIN32

#include <stdio.h>

class linux_urandom_pool
{
public:
	linux_urandom_pool();
	~linux_urandom_pool();

	void initialize();
	void finalize();

private:
	void _read();

private:
	std::vector<unsigned char> m_data;
	FILE* m_file = nullptr;
};

class linux_random_pool
{
public:
	linux_random_pool();
	void initialize();

private:
	std::vector<unsigned char> m_data;
};

#endif

class rndpoolImp : public rndpool
{
public:
	rndpoolImp();
	void initialize();
	void finalize();

	virtual int random();
	virtual int random(int min, int max);

private:
#ifndef WIN32
	linux_urandom_pool m_urandom_pool;
	linux_random_pool m_random_pool;
#endif
};
