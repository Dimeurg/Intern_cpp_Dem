#ifndef _HEADER_H_
#define _HEADER_H_

#include <functional>
#include <tuple>
#include <ostream>

template <typename Result, typename ... Args>
class Cache
{
public:
	Cache(std::function<Result(Args...)> _f) : m_f(_f), m_count(0), m_current_pos(0) {};
	std::ostream & Print(std::ostream & _o);
	Result find(Args... args);

private:
	int findInMas(std::tuple<Args...> & _t);
	void insertIntoMas(std::tuple<Args...> & _t, Result & _result);
	std::function <Result(Args...)> m_f;
	
	std::pair<std::tuple<Args...>, Result> m_mas[10];
	short int m_current_pos;
	short int m_count;
};

template <typename Result, typename ... Args>
Result Cache<Result, Args...>::find(Args... args)
{
	auto t = std::make_tuple(args...);
	int ft = findInMas(t);
	if (ft != -1)
	{
		return m_mas[ft].second;

	}
	else
	{
		Result r = m_f(std::forward<Args>(args)...);
		insertIntoMas(t, r);
		return r;
	}
}

template <typename Result, typename ... Args>
int Cache<Result, Args...>::findInMas( std::tuple<Args...> & _t)
{
	for (int i = 0; i < m_count;i++)
	{
		if (m_mas[i].first == _t)
			return i;
	}

	return -1;
}

template <typename Result, typename ... Args>
void Cache<Result, Args...>::insertIntoMas(std::tuple<Args...> & _t, Result & _result)
{
	if (m_count == 10)
	{
		m_mas[m_current_pos++] = std::make_pair(_t, _result);
		if (m_current_pos == 10)
			m_current_pos = 0;
	}
	else
	{
		m_mas[m_count++] = std::make_pair(_t, _result);
	}
}

template <typename Result, typename ... Args>
std::ostream & Cache<Result, Args...>::Print(std::ostream & _o)
{
	for (int i = 0; i < m_count; i++)
		 _o << "Cache " << i << ": "<< std::get<0>(m_mas[i].first) << ' ' <<m_mas[i].second << '\n';
	return _o;
}

#endif //_HEADER_H_
