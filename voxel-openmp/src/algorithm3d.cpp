#include "../include/algorithm3d.h"

Vector::Vector(){
	x = y = z = 0;
}
Vector::Vector(int number){
	x = y = z = number;
}
Vector::Vector(realtype x, realtype y, realtype z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector::Vector(const Point& P){
	x = P.x;
	y = P.y;
	z = P.z;
}
bool Vector::operator == (const Vector& vec2) const{
	if((x == vec2.x) && (y == vec2.y) && (z == vec2.z))
		return true;
	return false;
}
bool Vector::operator != (const Vector& vec2) const{
	if((x == vec2.x) && (y == vec2.y) && (z == vec2.z))
		return false;
	return true;
}
Vector Vector::operator * (const Vector& vec2) const{
	realtype rX = (y * vec2.z) - (vec2.y * z);
	realtype rY = (z * vec2.x) - (vec2.z * x);
	realtype rZ = (x * vec2.y) - (vec2.x * y);
	return Vector(rX,rY,rZ);
}
Vector Vector::operator / (realtype scalar) const{
	return Vector(x/scalar, y/scalar, z/scalar);
}
Vector operator * (realtype scalar, const Vector& vec) {
	return Vector(vec.x*scalar, vec.y*scalar, vec.z*scalar);
}
std::ostream& operator<< (std::ostream& out, const Vector& vector) {
	out << vector.x << " " << vector.y << " " << vector.z;
	return out;
}




Point::Point(realtype x, realtype y, realtype z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Point::Point(){
	x = y = z = 0;
}
Point::Point(int number){
	x = y = z = number;
}
Point::Point(const Vector& V){
	x = V.x;
	y = V.y;
	z = V.z;
}
bool Point::operator == (const Point& p2) const{
	if((x == p2.x) && (y == p2.y) && (z == p2.z))
		return true;
	return false;
}
bool Point::operator != (const Point& p2) const{
	if((x == p2.x) && (y == p2.y) && (z == p2.z))
		return false;
	return true;
}
Point Point::operator + (const Vector& vec) const{
	return Point(x+vec.x, y+vec.y, z+vec.z);
}
Vector Point::operator - (const Point& p2) const{
	return Vector(x-p2.x, y-p2.y, z-p2.z);
}
Vector Point::operator + (const Point& p2) const{
	return Vector(x+p2.x, y+p2.y, z+p2.z);
}
std::ostream& operator<< (std::ostream& out, const Point& point){
	out << point.x << " " << point.y << " " << point.z;
	return out;
}



Line::Line(const Point& P0, const Point& P1){
	this->P0 = P0;
	this->P1 = P1;
}
std::ostream& operator<< (std::ostream& out, const Line& line){
	out << line.P0 << std::endl << line.P1;
	return out;
}

Ray::Ray(const Point& P0, const Point& P1){
	this->P0 = P0;
	this->P1 = P1;
}


Segment::Segment(const Point& P0, const Point& P1){
	this->P0 = P0;
	this->P1 = P1;
}


Plane::Plane(const Point& P0,const Vector& n ){
	this->P0 = P0;
	this->n = n;
}

Triangle::Triangle(){
}
Triangle::Triangle(const Point& P0,const Point& P1, const Point& P2){
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
}
std::ostream& operator<< (std::ostream& out, const Triangle& triangle){
	out << triangle.P0 << std::endl << triangle.P1 << std::endl << triangle.P2;
	return out;
}

Quadrangle::Quadrangle(const Point& P0,const Point& P1, const Point& P2, const Point& P3){
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	this->P3 = P3;
}
std::ostream& operator<< (std::ostream& out, const Quadrangle& quadrangle){
	out << quadrangle.P0 << std::endl << quadrangle.P1 << std::endl << quadrangle.P2 << std::endl << quadrangle.P3;
	return out;
}

Polyline::Polyline(int n, const Point *P){
	this->P = new Point[n];
	for(int i = 0 ; i < n ; ++i){
		this->P[i] = P[i];
	}
}
Polyline::~Polyline(){
	delete[] P;
}

Polygon::Polygon(int n, const Point *P){
	this->P = new Point[n];
	for(int i = 0 ; i < n ; ++i){
		this->P[i] = P[i];
	}
}
Polygon::~Polygon(){
	delete[] P;
}

BoundingBox::BoundingBox(){
	// Punkty same poustawiaja sie na 0 (konstr. domyslne)
	halfSizeX = halfSizeY = halfSizeZ = 0;
}
BoundingBox::BoundingBox(const Point& xyzMin, const Point& xyzMax ){
	this->xyzMin = xyzMin;
	this->xyzMax = xyzMax;

	this->calculateCenterAndHalfs();
}
BoundingBox::BoundingBox(const Quad2D& quad, realtype minZ, realtype maxZ){
	this->xyzMin.x = quad.xyMin.x;
	this->xyzMin.y = quad.xyMin.y;
	this->xyzMin.z = minZ;

	this->xyzMax.x = quad.xyMax.x;
	this->xyzMax.y = quad.xyMax.y;
	this->xyzMax.z = maxZ;

	this->calculateCenterAndHalfs();
}
BoundingBox::BoundingBox(const Point& center, realtype cubeHalfXYZ){ // tworzy szeszcian
	this->center = center;
	this->halfSizeX = this->halfSizeY = this->halfSizeZ = cubeHalfXYZ;

	this->calculateCornersUsingCenterAndHalfs();
}
void BoundingBox::calculateCenterAndHalfs(){
	Vector subVec = xyzMax + xyzMin;
	//std::cout << "sub vec: " << subVec << std::endl;
	Vector halfVec = subVec / 2.0;
	//std::cout << "half vec: " << halfVec << std::endl;

	center = Point(halfVec);
	halfSizeX = xyzMax.x - center.x;
	halfSizeY = xyzMax.y - center.y;
	halfSizeZ = xyzMax.z - center.z;
}
void BoundingBox::calculateCornersUsingCenterAndHalfs(){
	xyzMin.x = center.x - halfSizeX;
	xyzMin.y = center.y - halfSizeY;
	xyzMin.z = center.z - halfSizeZ;

	xyzMax.x = center.x + halfSizeX;
	xyzMax.y = center.y + halfSizeY;
	xyzMax.z = center.z + halfSizeZ;
}
std::ostream& operator << (std::ostream& out, const BoundingBox& aabb){
	out << "#xyzmin " << aabb.xyzMin << std::endl;
	out << "#xyzmax " << aabb.xyzMax << std::endl;
	out << "#center " << aabb.center ;
	return out;
}

int Mathematician::intersectRayTriangle( const Ray& R,const Triangle& T, Point* I )
{
	Vector    u, v, n;              // triangle vectors
	Vector    dir, w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.P1 - T.P0;
	v = T.P2 - T.P0;
	n = u * v;              // cross product
	if (n == (Vector)0)             // triangle is degenerate
		return -1;                  // do not deal with this case

	dir = R.P1 - R.P0;              // ray direction vector
	w0 = R.P0 - T.P0;
	a = -dot(n,w0);
	b = dot(n,dir);
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			return 2;
		else return 0;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return 0;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	*I = R.P0 + r * dir;            // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u,u);
	uv = dot(u,v);
	vv = dot(v,v);
	w = *I - T.P0;
	wu = dot(w,u);
	wv = dot(w,v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                       // I is in T
}

bool Mathematician::isPointInBoundingBox(const Point& point, const BoundingBox& box){
	if( (point.x >= box.xyzMin.x) && (point.x <= box.xyzMax.x) )
		if( (point.y >= box.xyzMin.y) && (point.y <= box.xyzMax.y) )
			if( (point.z >= box.xyzMin.z) && (point.z <= box.xyzMax.z) )
				return true;
	return false;
}
bool Mathematician::intersectTriangleBoundingBox(const Triangle& triangle, const BoundingBox& box){
	// wypelnianie niezbednych tablic-buforow dla metody z out_alg...
	this->fillBoxCenterBuf(box.center);
	this->fillBoxHalfSizeBuf(box);
	this->fillTrivertsBuf(triangle);

	int result = triBoxOverlap(boxCenterBuf,boxHalfSizeBuf,trivertsBuf);
	//std::cout << "in metod: " << result << std::endl;
	return (result == 1) ? true : false;
}

void Mathematician::fillBoxCenterBuf(const Point& boxCenter){
	boxCenterBuf[0] = boxCenter.x;
	boxCenterBuf[1] = boxCenter.y;
	boxCenterBuf[2] = boxCenter.z;
}
void Mathematician::fillBoxHalfSizeBuf(const BoundingBox& box){
	boxHalfSizeBuf[0] = box.halfSizeX;
	boxHalfSizeBuf[1] = box.halfSizeY;
	boxHalfSizeBuf[2] = box.halfSizeZ;
}
// triverts - triangle vertex
void Mathematician::fillTrivertsBuf(const Triangle& T){
	trivertsBuf[0][0] = T.P0.x;
	trivertsBuf[0][1] = T.P0.y;
	trivertsBuf[0][2] = T.P0.z;

	trivertsBuf[1][0] = T.P1.x;
	trivertsBuf[1][1] = T.P1.y;
	trivertsBuf[1][2] = T.P1.z;

	trivertsBuf[2][0] = T.P2.x;
	trivertsBuf[2][1] = T.P2.y;
	trivertsBuf[2][2] = T.P2.z;
}

