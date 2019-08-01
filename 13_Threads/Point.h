#ifndef _POINT_H_
#define _POINT_H_
#include <iostream>

class Point
{
public:
	Point():x(-1), y(-1){}
	Point(int _x, int _y);
	Point(const Point & _p);
	Point(Point && _p);

	Point & operator = (const Point & _p);
	bool operator ==(const Point & _p);
	bool operator !=(const Point & _p);
	bool operator <(const Point & _p);
	bool operator >(Point _p) const;

	int GetX() const { return  x; }
	int GetY() const { return  y; }

	int  y,  x;
};

Point::Point(int _x, int _y) :  x(_x),  y(_y) {}
Point::Point(const Point & _p) : Point(_p. x, _p. y) {}
Point::Point(Point && _p) : Point(_p. x, _p. y) {}

Point & Point::operator=(const Point & _p) {
	 x = _p. x;
	 y = _p. y;

	return *this;
}

bool Point::operator !=(const Point & _p) {
	return !(*this == _p);
}

bool Point::operator >(Point _p) const {
	if ( y > _p. y)
		return true;
	else if ( y == _p. y) {
		if ( x > _p. x)
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
	return ( x == _p. x
		&&  y == _p. y);
}

bool operator ==(const Point & _p1, const Point & _p2){
	return (_p1.GetX() == _p2.GetX()
		&& _p1.GetY() == _p2.GetY());
}

#endif //_POINT_H_
 