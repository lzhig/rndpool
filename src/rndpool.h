#pragma once

#include "../inc/rndpool_inc.h"

#include <vector>


#ifndef WIN32

#include <stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <iostream>

class linux_urandom_pool
{
public:
	linux_urandom_pool();
	~linux_urandom_pool();

	void initialize();
	void finalize();

	void shuffle(int m);

	int pop();

private:
	void _read();
	void _trace();

private:
	std::vector<unsigned char> m_data;
	FILE* m_file = nullptr;

	int m_read_pos = 0;
};

template<class T, int size>
class produce_consume
{
public:
	produce_consume()
		: m_data(size)
	{
	}

	const T pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_write_pos == m_read_pos)
		{
			std::random_shuffle(m_data.begin(), m_data.end());
			return m_data[m_read_pos];
		}

		auto ret = m_data[m_read_pos++];
		if (m_read_pos >= size)
		{
			m_read_pos = 0;
		}

		m_not_full.notify_all();
		lock.unlock();

		return ret;
	}

	void push(T& t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (((m_write_pos + 1) % size) == m_read_pos)
		{
			//std::cout << "wait" << std::endl;
			m_not_full.wait(lock);
		}
		//std::cout << "push " << int(t) << std::endl;
		m_data[m_write_pos++] = t;
		if (m_write_pos == size)
		{
			m_write_pos = 0;
		}
		lock.unlock();
	}

private:
	std::vector<T> m_data;
	int m_read_pos = 0;
	int m_write_pos = 0;
	std::mutex m_mutex;
	std::condition_variable m_not_full; // 条件变量, 指示产品缓冲区不为满.
};

class linux_random_pool
{
public:
	linux_random_pool();
	void initialize();
	void finalize();
	unsigned char pop();

private:
	static void _thread_func(linux_random_pool*);
	std::thread m_thread;

	produce_consume<unsigned char, 16> m_data;
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

