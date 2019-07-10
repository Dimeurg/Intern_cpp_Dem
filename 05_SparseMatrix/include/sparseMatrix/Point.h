#ifndef _POINT_H_
#define _POINT_H_
#include <iostream>

class Point
{
public:
	Point(int _x, int _y);
	Point(const Point & _p);
	Point(Point && _p);

	Point & operator = (const Point & _p);
	bool operator ==(const Point & _p);
	bool operator !=(const Point & _p);
	bool operator <(const Point & _p);
	bool operator >(Point _p) const;

	int GetX() const { return m_x; }
	int GetY() const { return m_y; }

private:
	int m_y, m_x;
};

Point::Point(int _x, int _y) : m_x(_x), m_y(_y) {}
Point::Point(const Point & _p) : Point(_p.m_x, _p.m_y) {}
Point::Point(Point && _p) : Point(_p.m_x, _p.m_y) {}

Point & Point::operator=(const Point & _p) {
	m_x = _p.m_x;
	m_y = _p.m_y;

	return *this;
}

bool Point::operator !=(const Point & _p) {
	return !(*this == _p);
}

bool Point::operator >(Point _p) const {
	if (m_y > _p.m_y)
		return true;
	else if (m_y == _p.m_y) {
		if (m_x > _p.m_x)
			return true;
		return false;
	}
	else
		return false;
}

bool Point::operator <(const Point & _p) {
	return _p > *this;
}

std::ostream & operator << (std::ostream & _o, Point _p) {
	_o << _p.GetX() << ' ' << _p.GetY();
	return _o;
}

bool operator <(const Point & _p1, const Point & _p2) {
	return _p2 > _p1;
}

bool Point::operator ==(const Point & _p) {
	return (m_x == _p.m_x
		&& m_y == _p.m_y);
}

bool operator ==(const Point & _p1, const Point & _p2){
	return (_p1.GetX() == _p2.GetX()
		&& _p1.GetY() == _p2.GetY());
}

#endif //_POINT_H_
 