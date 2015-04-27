
#ifndef ALGORITHM3D_H_
#define ALGORITHM3D_H_

#include <iostream>
#include <cmath>
#include "out_algorithm3d.h"
#include "algorithm2d.h"

// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}
//        operators for:
//            == to test  equality
//            != to test  inequality
//            (Vector)0 =  (0,0,0)         (null vector)
//            Point   = Point ± Vector
//            Vector =  Point - Point
//            Vector =  Scalar * Vector    (scalar product)
//            Vector =  Vector * Vector    (cross product)
//    Line and Ray and Segment with defining  points {Point P0, P1;}
//        (a Line is infinite, Rays and  Segments start at P0)
//        (a Ray extends beyond P1, but a  Segment ends at P1)
//    Plane with a point and a normal {Point V0; Vector  n;}
//    Triangle with defining vertices {Point V0, V1, V2;}
//    Polyline and Polygon with n vertices {int n;  Point *V;}
//        (a Polygon has V[n]=V[0])

#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

class Vector;
class Point;
class Line;
class Ray;
class Triangle;
class Quadrangle;
class Polyline;
class Polygon;
class Mathematician;

class Vector{
public:
	realtype x,y,z;
	Vector();
	Vector(int number);
	Vector(realtype x, realtype y, realtype z);
	Vector(const Point& P);

	bool operator == (const Vector& vec2) const;
	bool operator != (const Vector& vec2) const;
	Vector operator * (const Vector& vec2) const; // cross product
	Vector operator / (realtype scalar) const;


	friend Vector operator * (realtype scalar, const Vector& vec) ; // mnozenie przez skalar
	friend std::ostream& operator<< (std::ostream& out, const Vector& vector) ;
};
Vector operator * (realtype scalar, const Vector& vec) ;
std::ostream& operator<< (std::ostream& out, const Vector& vector) ;

class Point{
public:
	realtype x,y,z;
	Point();
	Point(int number);
	Point(realtype x, realtype y, realtype z);
	Point(const Vector& V);

	bool operator == (const Point& p2) const;
	bool operator != (const Point& p2) const;
	Point operator + (const Vector& vec) const;
	Vector operator - (const Point& p2) const;
	Vector operator + (const Point& p2) const;
	friend std::ostream& operator<< (std::ostream& out, const Point& point) ;
};
std::ostream& operator<< (std::ostream& out, const Point& point) ;


class Line{
public:
	Point P0, P1;
	Line(const Point& P0, const Point& P1);

	friend std::ostream& operator<< (std::ostream& out, const Line& line);
};
std::ostream& operator<< (std::ostream& out, const Line& line);

class Ray{
public:
	Point P0, P1;
	Ray(const Point& P0, const Point& P1);
};

class Segment{
public:
	Point P0, P1;
	Segment(const Point& P0, const Point& P1);
};

class Plane{
public:
	Point P0;
	Vector  n; // wektor normalny
	Plane(const Point& P0,const Vector& n );
};

class Triangle{
public:
	Point P0, P1, P2;
	Triangle();
	Triangle(const Point& P0,const Point& P1, const Point& P2);

	friend std::ostream& operator<< (std::ostream& out, const Triangle& triangle);
};
std::ostream& operator<< (std::ostream& out, const Triangle& triangle);

class Quadrangle{
public:
	Point P0, P1, P2, P3;
	Quadrangle(const Point& P0,const Point& P1, const Point& P2, const Point& P3);

	friend std::ostream& operator<< (std::ostream& out, const Quadrangle& quadrangle);
};
std::ostream& operator<< (std::ostream& out, const Quadrangle& quadrangle);

class Polyline{
public:
	int n;
	Point *P;
	Polyline(int n, const Point *P);
	~Polyline();
};

class Polygon{
public:
	int n;
	Point *P;
	Polygon(int n, const Point *P);
	~Polygon();
};


/*Przyjecie, ze wart. osi Z rosną do "przodu" patrzacego*/
class BoundingBox{ // AABox
public:
	Point xyzMin;
	Point xyzMax;
	Point center;
	realtype halfSizeX;
	realtype halfSizeY;
	realtype halfSizeZ;

	BoundingBox();
	BoundingBox(const Point& xyzMin, const Point& xyzMax );
	BoundingBox(const Quad2D& quad, realtype minZ, realtype maxZ);
	BoundingBox(const Point& center, realtype cubeHalfXYZ); // tworzy szeszcian

	friend std::ostream& operator << (std::ostream& out, const BoundingBox& aabb);
private:
	void calculateCenterAndHalfs();
	void calculateCornersUsingCenterAndHalfs();
};
std::ostream& operator << (std::ostream& out, const BoundingBox& aabb);

class Mathematician{
protected:
	realtype boxCenterBuf[3];
	realtype boxHalfSizeBuf[3];
	realtype trivertsBuf[3][3];
public:
	// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
	//    Input:  a ray R, and a triangle T
	//    Output: *I = intersection point (when it exists)
	//    Return: -1 = triangle is degenerate (a segment or point)
	//             0 =  disjoint (no intersect)
	//             1 =  intersect in unique point I1
	//             2 =  are in the same plane
	int intersectRayTriangle(const Ray& R,const Triangle& T, Point* I );
	bool isPointInBoundingBox(const Point& point, const BoundingBox& box);
	bool intersectTriangleBoundingBox(const Triangle& triangle, const BoundingBox& box);
	Point calculateCenterBoundingBox(const BoundingBox& box);

private:
	void fillBoxCenterBuf(const Point& boxCenter);
	void fillBoxHalfSizeBuf(const BoundingBox& box);
	void fillTrivertsBuf(const Triangle& T);
};


#endif
