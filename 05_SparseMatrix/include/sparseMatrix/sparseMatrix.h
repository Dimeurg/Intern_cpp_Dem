#ifndef _SPARSEMATRIX_H_
#define _SPARSEMATRIX_H_
#include <iostream>
#include "Point.h"
#include <map>
#include <exception>

template <typename T1, typename T2>
class myIterator : public std::iterator<std::input_iterator_tag, T1>
{
public:
	typedef typename std::map<T1, T2>::iterator mapIT;

	myIterator(mapIT _p);
	myIterator(const myIterator & _it);

	bool operator ==(const myIterator & _it) const;
	bool operator !=(const myIterator & _it) const;

	std::pair<T1, T2> operator*();
	myIterator & operator ++();

private:
	mapIT m_it;
};


template <typename T1, typename T2>
myIterator<T1,T2>::myIterator(const myIterator & _it) : m_it(_it.m_it) {}

template <typename T1, typename T2>
myIterator<T1,T2>::myIterator(mapIT _p) : m_it(_p) {}

template <typename T1, typename T2>
bool myIterator<T1,T2>::operator==(const myIterator & _it) const {
	return m_it == _it.m_it;
}

template <typename T1, typename T2>
bool myIterator<T1,T2>::operator!=(const myIterator & _it) const {
	return m_it != _it.m_it;
}

template <typename T1, typename T2>
std::pair<T1, T2> myIterator<T1,T2>::operator*() {
	return make_pair(m_it->first, m_it->second);
}

template <typename T1, typename T2>
myIterator<T1, T2> & myIterator<T1,T2>::operator++() {
	++m_it;
	return *this;
}


template <typename T>
class sparseMatrix
{
public:
	sparseMatrix(int _x, int _y);
	sparseMatrix(const sparseMatrix & _m);
	~sparseMatrix();

	void insert(int _x, int _y, T _t);
	void remove(int _x, int _y);
	T at(int _x, int _y);
	int distance(Point _p1, Point _p2);

	myIterator<Point, T> begin();
	myIterator<Point, T> end();

private:
	std::map<Point, T> m_map;
	int m_x, m_y;
};

template <typename T>
sparseMatrix<T>::sparseMatrix(int _x, int _y) :m_y(_y), m_x(_x) {}
template <typename T>
sparseMatrix<T>::sparseMatrix(const sparseMatrix & _m) : m_y(_m.m_y), m_x(_m.m_x) {
	for (auto it = _m.m_map.cbegin(); it != _m.m_map.cend(); it++)
		m_map[it->first] = it->second;
}

template <typename T>
sparseMatrix<T>::~sparseMatrix() {
	m_map.clear();
}

template <typename T>
void sparseMatrix<T>::insert(int _x, int _y, T _t) {
	if (_x > m_x || _x < 1
		|| _y > m_y || _y < 1)
		throw std::logic_error("invalid data");
	m_map[Point(_x, _y)] = std::move(_t);
}

template <typename T>
void sparseMatrix<T>::remove(int _x, int _y) {
	if (_x > m_x || _x < 1
		|| _y > m_y || _y < 1)
		throw std::logic_error("invalid data");
	m_map.erase(Point(_x, _y));
}

template <typename T>
T sparseMatrix<T>::at(int _x, int _y) {
	return m_map[Point(_x, _y)];
}

template<typename T>
std::ostream & operator << (std::ostream & _o, sparseMatrix<T> _m) {
	for (auto x : _m)
		std::cout << x.first << ' ' << x.second << '\n';
	return _o;
}

template<typename T>
myIterator<Point, T> sparseMatrix<T>::begin() {
	return myIterator<Point,T>(m_map.begin());
}

template<typename T>
myIterator<Point, T> sparseMatrix<T>::end() {
	return myIterator<Point,T>(m_map.end());
}

template<typename T>
int sparseMatrix<T>::distance(Point _p1, Point _p2) {
	if (_p1.GetX() > m_x || _p1.GetY() > m_y
		|| _p2.GetX() > m_x || _p2.GetY() > m_y)
		throw std::logic_error("invalid coordinates ");
	if (_p1 > _p2)
		std::swap(_p1, _p2);
	int result = 0;
	int y = _p2.GetY() - _p1.GetY();
	if (y > 0) {
		result += m_x * (y-1);
		result += _p2.GetX();
		result += m_x - _p1.GetX();
	}
	else	{
		result += _p2.GetX() - _p1.GetX();
	}
	return result;
}

#endif//_SPARSEMATRIX_H_
