#include "nlohmann/json.hpp"
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
		std::ofstream o(_fileName);
		_j.push_back(_el);
		o << _j;
		o.close();
}

void ClearFile(const std::string _fileName)
{
	json j;
	std::ofstream o(_fileName);
	o << j;
	o.close();
}
