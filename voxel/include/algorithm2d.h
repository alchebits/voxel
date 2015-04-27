#ifndef ALGORITHM2D_H_
#define ALGORITHM2D_H_

#include <iostream>

typedef float realtype;

class Point2D{
public:
	realtype x,y;
	Point2D();
	Point2D(realtype x, realtype y);

	Point2D operator + (const Point2D& p2) const;
	Point2D operator / (realtype scalar) const;
	friend std::ostream& operator<< (std::ostream& out, const Point2D& point) ;
};
std::ostream& operator<< (std::ostream& out, const Point2D& point) ;

class Quad2D{
public:
	Point2D xyMin; // -x -y
	Point2D xyMax; // +x +y

	Point2D center;
	realtype halfSizeX;
	realtype halfSizeY;

	Quad2D();
	Quad2D(const Point2D& xyMin, const Point2D& xyMax);
	bool isPointIn(const Point2D& point); // true, jesli punkt zawiera sie w kwadracie
	Quad2D leftTopPart();
	Quad2D rightTopPart();
	Quad2D rightBottomPart();
	Quad2D leftBottomPart();

	friend std::ostream& operator<< (std::ostream& out, const Quad2D& quad) ;
};
std::ostream& operator<< (std::ostream& out, const Quad2D& quad) ;

#endif
