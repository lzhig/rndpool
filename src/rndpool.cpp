#include "rndpool.h"

#ifndef WIN32
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
	m_file = fopen("/dev/urandom");
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

linux_random_pool::linux_random_pool()
	:m_data(RANDOM_POOL_SIZE)
{

}

void linux_random_pool::initialize()
{

}

#endif
