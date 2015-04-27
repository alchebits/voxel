#include "../include/algorithm2d.h"

Point2D::Point2D(){
	x = y = 0;
}
Point2D::Point2D(realtype x, realtype y){
	this->x = x;
	this->y = y;
}
Point2D Point2D::operator + (const Point2D& p2) const{
	return Point2D(x+p2.x, y+p2.y);
}
Point2D Point2D::operator / (realtype scalar) const{
	return Point2D(x / scalar, y / scalar);
}
std::ostream& operator<< (std::ostream& out, const Point2D& point){
	out << point.x << " " << point.y;
	return out;
}

Quad2D::Quad2D(){
	halfSizeX = 0;
	halfSizeY = 0;
}
/* Tylko dla prostokatow rownoleglych do osi X i Y*/
Quad2D::Quad2D(const Point2D& xyMin, const Point2D& xyMax){
	this->xyMin = xyMin;
	this->xyMax = xyMax;

	// obliczenie punktu centralnego oraz odleglosci
	center = (xyMin + xyMax) / 2;
	halfSizeX = xyMax.x - center.x;
	halfSizeY = xyMax.y - center.y;
}
bool Quad2D::isPointIn(const Point2D& point){ // true, jesli punkt zawiera sie w kwadracie
	if( (point.x <= xyMax.x) && (point.x >= xyMin.x) ){
		if( (point.y <= xyMax.y) && (point.y >= xyMin.y) ){
			return true;
		}
	}
	return false;
}
Quad2D Quad2D::leftTopPart(){
	Point2D p1(xyMin.x, xyMin.y + halfSizeY);
	Point2D p2(xyMin.x + halfSizeX, xyMax.y);
	return Quad2D(p1, p2);
}
Quad2D Quad2D::rightTopPart(){
	return Quad2D(center,xyMax);
}
Quad2D Quad2D::rightBottomPart(){
	Point2D p4( xyMin.x + halfSizeX, xyMin.y );
	Point2D p3( xyMax.x, xyMin.y + halfSizeY );
	return Quad2D(p4, p3);
}
Quad2D Quad2D::leftBottomPart(){
	return Quad2D(xyMin,center);
}
std::ostream& operator<< (std::ostream& out, const Quad2D& quad){
	out << "min: " << quad.xyMin << ", max: " << quad.xyMax ;
	return out;
}
