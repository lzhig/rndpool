#include "rndpool.h"

#ifndef WIN32
#include <algorithm>
#include <iostream>

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

	m_urandom_pool.shuffle(m_random_pool.pop());
#else
#endif
}

void rndpoolImp::finalize()
{

}

int rndpoolImp::random()
{
#ifndef WIN32
	return m_urandom_pool.pop();
#else
	return 0;
#endif
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
	_read();
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
	for (int i = 0; i < m; i++)
		std::random_shuffle(m_data.begin(), m_data.end());
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
	}
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
{
}

void linux_random_pool::initialize()
{
	m_thread = std::thread(_thread_func, this);
}

void linux_random_pool::finalize()
{
	
}

unsigned char linux_random_pool::pop()
{
	return m_data.pop();
}

void linux_random_pool::_thread_func(linux_random_pool* p)
{
	auto file = fopen("/dev/random", "rb");
	if (file == nullptr)
		return;
	
	while (true)
	{
		unsigned char tmp;
		fread(&tmp, 1, 1, file);
		p->m_data.push(tmp);
	}
	
	fclose(file);
}

#endif
