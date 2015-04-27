#include "../include/wavefront.h"


Face::Face(std::vector<Point*>* points){
	this->points = points;
}
Face::~Face(){
}
const std::vector<int>* Face::getPointsIndexes_PTR() const{
	return &pointsIndexes;
}
void Face::setPointsIDs(const int* pointsIDs, int pointsNumber){
	for(int i = 0 ; i < pointsNumber ; ++i){
		pointsIndexes.push_back(pointsIDs[i]);
	}
}
unsigned Face::getFaceType() const{
	switch(pointsIndexes.size()){
	case 1:
		return FACE_POINT;
	case 2:
			return FACE_LINE;
	case 3:
			return FACE_TRIANGLE;
	case 4:
			return FACE_QUADRANGLE;
	default:
		std::cerr << __FILE__ " " << __LINE__ << "Ten kod nie mial prawa sie wykonac!" << std::endl;
		return 0;
	}
}

PointFace::PointFace(std::vector<Point*>* points, const int* pointsIDs) : Face(points){
	this->setPointsIDs(pointsIDs,1);
}
PointFace::~PointFace(){
}
int PointFace::getPointsNumber() const{
	return 1;
}
Point PointFace::getShape() const{
	int id1 = pointsIndexes[0];
	return *points->at(id1);
}


LineFace::LineFace(std::vector<Point*>* points, const int* pointsIDs) : Face(points){
	this->setPointsIDs(pointsIDs,2);
}
LineFace::~LineFace(){
}
int LineFace::getPointsNumber() const{
	return 2;
}
Line LineFace::getShape() const{
	int id1 = pointsIndexes[0];
	int id2 = pointsIndexes[1];
	return Line(*points->at(id1),*points->at(id2));
}

TriangleFace::TriangleFace(std::vector<Point*>* points, const int* pointsIDs) : Face(points){
	this->setPointsIDs(pointsIDs,3);
}
TriangleFace::~TriangleFace(){
}
int TriangleFace::getPointsNumber() const{
	return 3;
}
Triangle TriangleFace::getShape() const{
	int id1 = pointsIndexes[0];
	int id2 = pointsIndexes[1];
	int id3 = pointsIndexes[2];
	return Triangle(*points->at(id1),*points->at(id2),*points->at(id3));
}


QuadrangleFace::QuadrangleFace(std::vector<Point*>* points, const int* pointsIDs) : Face(points){
	this->setPointsIDs(pointsIDs,4);
}
QuadrangleFace::~QuadrangleFace(){
}
int QuadrangleFace::getPointsNumber() const{
	return 4;
}
Quadrangle QuadrangleFace::getShape() const{
	int id1 = pointsIndexes[0];
	int id2 = pointsIndexes[1];
	int id3 = pointsIndexes[2];
	int id4 = pointsIndexes[3];
	return Quadrangle(*points->at(id1),*points->at(id2),*points->at(id3),*points->at(id4));
}


WavefrontOBJ::WavefrontOBJ(){
	// Wavefront indeksowanie zaczyna sie od 1, wiec nalezy na starto zapelnic indeks 0
	points.push_back(new Point(0,0,0));
	firstPointAdded = false;
}
WavefrontOBJ::~WavefrontOBJ(){
	for(unsigned i = 0 ; i < faces.size() ; ++i){
		delete faces[i];
	}
	for(unsigned i = 0 ; i < points.size() ; ++i){
		delete points[i];
	}
}
void WavefrontOBJ::addPoint(const realtype* xyz){
	this->addPoint(xyz[0],xyz[1],xyz[2]);
}
void WavefrontOBJ::addPoint(realtype x, realtype y, realtype z){
	this->checkIsMaxOrMin(x,y,z);
	Point* newPoint = new Point(x,y,z);
	points.push_back(newPoint);
}
void WavefrontOBJ::addFace(const int* pointsIDs, int pointsNumber){
	Face* newFace = NULL;

	switch(pointsNumber){
	case 1:
		newFace = new PointFace(&points, pointsIDs);
		break;
	case 2:
		newFace = new LineFace(&points, pointsIDs);
		break;
	case 3:
		newFace = new TriangleFace(&points, pointsIDs);
		break;
	case 4:
		newFace = new QuadrangleFace(&points, pointsIDs);
		break;
	default:
		std::cerr << __FILE__ << " " << __LINE__ << ": Face nie moze miec " << pointsNumber << "wierzcholkow !!" << std::endl;
		break;
	}

	if(newFace != NULL){
		faces.push_back(newFace);
	}
}
const std::vector<const Face*>* WavefrontOBJ::getFaces() const{
	return &faces;
}
realtype WavefrontOBJ::getMaxX() const {
	return maxX;
}

realtype WavefrontOBJ::getMaxY() const {
	return maxY;
}

realtype WavefrontOBJ::getMaxZ() const {
	return maxZ;
}

realtype WavefrontOBJ::getMinX() const {
	return minX;
}

realtype WavefrontOBJ::getMinY() const {
	return minY;
}

realtype WavefrontOBJ::getMinZ() const {
	return minZ;
}

void WavefrontOBJ::checkIsMaxOrMin(realtype x, realtype y, realtype z){

	// szukajac min, max, to najlepsze wyjscie na start szukania min i max (wziac pierwsze wartosci)
	if(!firstPointAdded){
		minX = maxX = x;
		minY = maxY = y;
		minZ = maxZ = z;
		firstPointAdded = true;
		return;
	}

	if(x < minX){
		minX = x;
	}else
	if(x > maxX){
		maxX = x;
	}

	if(y < minY){
		minY = y;
	}else
	if( y > maxY){
		maxY = y;
	}

	if(z < minZ){
		minZ = z;
	}else
	if(z > maxZ){
		maxZ = z;
	}
}


void WavefrontOBJ::sysout(){
	std::cout << "Wierzcholki:" << std::endl;

	for(unsigned i = 0 ; i < points.size() ; ++i){
		std::cout << "v " << points[i]->x << " "<< points[i]->y << " "<< points[i]->z << std::endl;
	}

	std::cout << std::endl << "Face'y: " << std::endl;
	for(unsigned i = 0 ; i < faces.size() ; ++i){
		const std::vector<int>* facePointsIndexes = faces[i]->getPointsIndexes_PTR();
		std::cout << "f " ;
		for( unsigned j = 0 ; j < facePointsIndexes->size() ; ++j ){
			std::cout << facePointsIndexes->at(j) << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "maxX: " << maxX << std::endl;
	std::cout << "maxY: " << maxY << std::endl;
	std::cout << "maxZ: " << maxZ << std::endl;
	std::cout << "minX: " << minX << std::endl;
	std::cout << "minY: " << minY << std::endl;
	std::cout << "minZ: " << minZ << std::endl;
}

