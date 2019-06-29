#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"
#include <exception>
#include <ctime>
#include <iostream>
#include <map>
#include <string>

typedef std::map<std::string, std::string>  mapT;
typedef Cache<std::string, std::string, mapT *> cacheT;

void RandomMap(mapT & _m, int _count)
{
	std::string s = "109.210.53.";
	std::string name = "user: ";
	char buf[10];

	for (int i = 0; i < _count; i++)
	{
		_itoa(i, buf, 10);
		_m.insert(std::make_pair(s + buf, name + buf));
	}
}

void printMap(mapT & _m)
{
	mapT::iterator it;
	for (it = _m.begin(); it != _m.end(); it++)
		std::cout << it->first << ' ' << it->second << '\n';
}

std::string getNameByIp_help(std::string _s, mapT * _map)
{
	mapT::iterator ft = _map->find(_s);
	if (ft != _map->end())
		return ft->second;
	else
		throw std::exception("This ip doesn`t exist");
}
 
std::string getNameByIp(cacheT & _cache, mapT * _m,std::string _s)
{
	std::string result = _cache.find(_s, _m);
	return result;
}

void test()
{
	mapT m;
	RandomMap(m, 50);

	static cacheT cache(getNameByIp_help);

	std::string s = "109.210.53.";

	std::cout <<'\n' <<"Input first element: " << '\n';
	getNameByIp(cache, &m, s + "0");
	cache.Print(std::cout);

	std::cout <<'\n' << "Input the same element: " << '\n';
	getNameByIp(cache, &m, s + "0");
	cache.Print(std::cout);

	std::cout << '\n' << "Input 10 elements: " << '\n';
	char buf[5];
	for (int i = 1; i < 10; i++)
	{
		_itoa(i, buf, 10);
		getNameByIp(cache, &m, s + buf);
	}
	cache.Print(std::cout);

	std::cout << '\n' << "Input one more element: " << '\n';
	getNameByIp(cache, &m, s + "10");
	cache.Print(std::cout);

	std::cout << '\n' << "Input 10 elements again: " << '\n';
	for (int i = 11; i < 21; i++)
	{
		_itoa(i, buf, 10);
		getNameByIp(cache, &m, s + buf);
	}
	cache.Print(std::cout);

	std::cout << '\n' << "Input invalid data: " << '\n';
	getNameByIp(cache, &m, s + "error");

	m.clear();
}


int main()
{
	try
	{
		test();
	}
	catch (std::exception _e)
	{
		std::cout << _e.what() << '\n';
	}
	std::cin.get();
	return 0;
}