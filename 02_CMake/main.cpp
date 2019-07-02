#include "json.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <exception>

// for convenience
using json = nlohmann::json;

void rundJsonInt(const std::string _fileName, int _size)
{
	json j;
	std::ofstream o(_fileName);
	srand(time(NULL));
	for(int i = 0; i < _size; i++)
	{
		j.push_back(rand() % 50);
	}	
	
	o << j;
	o.close();
}

void rundJsonChar(const std::string _fileName, int _size)
{
	std::ofstream o(_fileName);
	
	std::vector<std::string> v
	{"lol", "cat", "bla", "123", "string","a"};
	json j(v);
	
	o << j;
	o.close();
}

template <typename T>
void maxEl(json & _j, T & _result)
{
	if(_j.is_array())
	{
		json::iterator it = _j.begin();
		T max = *it;
		for(; it != _j.end(); it++)
			if(max < *it)
				max = *it;
		_result = max;	
	}
	else
		throw std::logic_error("json has no array");
}

template <typename T>
void minEl(json & _j, T & _result)
{
	if(_j.is_array())
	{
		json::iterator it = _j.begin();
		T max = *it;
		for(; it != _j.end(); it++)
			if(max > *it)
				max = *it;
		_result = max;	
	}
	else
		throw std::logic_error("json has no array");
}

template <typename T>
int posFind(json & _j, T & _find)
{
	if(_j.is_array())
	{
		int pos = 0;
		for(json::iterator it = _j.begin(); it != _j.end(); it++, pos++)
			if(_find == *it)
				return pos;
		return -1;
	}
	else
		throw std::logic_error("json has no array");
}

template <typename T>
void deleteEl(const std::string _fileName, json & _j ,T & _del)
{
	if(_j.is_array())
	{
		int pos = 0;
		for(json::iterator it = _j.begin(); it != _j.end(); it++, pos++)
			if(_del == *it)
			{
				_j.erase(pos);
				
				std::ofstream o(_fileName);
				o << _j;
				o.close();
				
				break;
			}
		
	}
	else
		throw std::logic_error("json has no array");
}

template <typename T>
void pushEl(const std::string _fileName,json & _j ,T & _el)
{
	if(_j.is_array())
	{
		std::ofstream o(_fileName);
		_j.push_back(_el);
		
		o << _j;
		o.close();
	}
	else
		throw std::logic_error("json has no array");
}



int main()
{
	json j, j2;
	rundJsonInt("test.json",20);
	rundJsonChar("test_S.json", 20);
	
	std::ifstream i("test.json");
	i >> j;
	i.close();
	
	std::ifstream i2("test_S.json");
	i2 >> j2;
	i2.close();
	
	int max, min, pos, find = 5, push = 10 ,del = 10;
	std::string maxS, minS, findS = "lol", pushS = "test" ,delS = "test"; 
	try
	{
		std::cout << j << '\n';
		
		maxEl(j,max);
		minEl(j,min);
		pos = posFind(j, find);
		std::cout << "int : " << '\n';
		std::cout <<"max : " << max <<" min : "<< min <<'\n';
		std::cout << "pos of " << find << " : ";
		if(pos >= 0)
		{
			std::cout << pos;
		}
		else 
			std::cout << "this element doesn`t exist";
		
		std::cout << '\n' << "push " << push << '\n';
		pushEl("test.json", j, push);
		std::cout << j;
		
		std::cout << '\n' << "del " << del << '\n';
		deleteEl("test.json", j, del);
		std::cout << j;
		
		maxEl(j2,maxS);
		minEl(j2,minS);
		pos = posFind(j2, findS);
		
		std::cout << '\n' << '\n' << "string : " << '\n';
		std::cout << j2 << '\n';
		std::cout <<"max : " << maxS <<" min : "<< minS <<'\n';
		std::cout << "pos of " << findS << " : ";
		if(pos >= 0)
		{
			std::cout << pos;
		}
		else 
			std::cout << "this element doesn`t exist";
		
		std::cout << '\n' << "push " << pushS << '\n';
		pushEl("test_S.json", j2, pushS);
		std::cout << j2;
		
		std::cout << '\n' << "del " << delS << '\n';
		deleteEl("test_S.json", j2, delS);
		std::cout << j2;
	}
	catch(std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	std::cin.get();
	return 0;
}