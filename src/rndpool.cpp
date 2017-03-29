#include "rndpool.h"

#ifndef WIN32
#include <algorithm>
#include <iostream>
#include <map>

#define URANDOM_POOL_SIZE 512
#define RANDOM_POOL_SIZE 16
#endif

rndpoolImp pool;

rndpool* get_rndpool() {
	// todo: 准备完成后再返回
	pool.initialize();
	return &pool;
}

rndpoolImp::rndpoolImp()
{
}

void rndpoolImp::initialize()
{
#ifndef WIN32
	m_urandom_pool.initialize();
	m_random_pool.initialize();
#else
#endif
}

void rndpoolImp::finalize()
{

}

int rndpoolImp::random()
{
	return 0;
}

int rndpoolImp::random(int min, int max)
{
	if (min + 1 == max)
		return min;

	if (min + 1 > max || min < 0 || max < 0)
		throw "bad parameters";

	return (random() % (max - min)) + min;
}

#ifndef WIN32

linux_urandom_pool::linux_urandom_pool()
	:m_data(URANDOM_POOL_SIZE)
{

}

linux_urandom_pool::~linux_urandom_pool()
{
	finalize();
}

void linux_urandom_pool::initialize()
{
	m_file = fopen("/dev/urandom", "rb");
	std::cout << "open: " << m_file << std::endl;
	_read();
#define NUMB 1000
	int a[NUMB] = { 0 };
	for (int i = 0; i < 10000000; i++)
	{
		auto n = pop() % NUMB;
		a[n]++;
	}

	std::map<int,int> c;

	for (int i = 0; i < NUMB; i++)
	{
		std::cout << a[i] << " ";
		c[a[i]]++;
	}

	for (auto it = c.begin(); it != c.end(); ++it)
	{
		std::cout << "key:" << it->first << ", value: " << it->second << std::endl;
	}
	std::cout << std::endl;
	
	//std::cout << std::hex << n << " " << std::oct << n << std::endl;
}

void linux_urandom_pool::finalize()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
	else{
		srand(time(nullptr));
	}
}

void linux_urandom_pool::shuffle(int m)
{
	std::random_shuffle(m_data.begin(), m_data.end());
	return;
	for (int i = 0; i < URANDOM_POOL_SIZE; i++)
	{
		auto tmp = m_data[m];
		std::swap(m_data[m], m_data[m + tmp]);
		m = tmp;
	}
}

int linux_urandom_pool::pop()
{
	if (m_read_pos == m_data.size())
	{
		_read();
		m_read_pos = 0;
	}
	unsigned int n = 0;
	int k = sizeof(int);
	for (int i = 0; i < k; i++)
	{
		n += m_data[m_read_pos++] << (8 * (k - i - 1));
	}
	
	return n & 0x7FFFFFF;
}

void linux_urandom_pool::_read()
{
	if (m_file == nullptr)
	{
		// 打开失败, 从标准库读取
		std::generate(m_data.begin(), m_data.end(), []()->unsigned char {
			return rand() % 255;
		});
	}
	else
	{
		fread(m_data.data(), URANDOM_POOL_SIZE, 1, m_file);
		
		// random shuffle
	}
//	_trace();
	shuffle(0);
//	_trace();
}

void linux_urandom_pool::_trace()
{
	std::cout << std::hex;
	for (int i = 0; i < URANDOM_POOL_SIZE; i++)
	{
		std::cout<< std::hex << int(m_data[i]) << " ";
	}
	std::cout << std::endl;
}

linux_random_pool::linux_random_pool()
	:m_data(RANDOM_POOL_SIZE)
{

}

void linux_random_pool::initialize()
{

}

#endif
