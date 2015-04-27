#ifndef WAVEFRONT_H_
#define WAVEFRONT_H_

#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include "algorithm3d.h"


class Face{
protected:
	std::vector<Point*>* points; // wskaznik na liste wszystkich wektorow obiektu, na ktory sklada sie dany face
	std::vector<int> pointsIndexes; // okresla indexy wektorow, ktore buduja dany face
public:
	static const unsigned FACE_POINT = 0;
	static const unsigned FACE_LINE = 1;
	static const unsigned FACE_TRIANGLE = 2;
	static const unsigned FACE_QUADRANGLE = 3;

	Face(std::vector<Point*>* points);
	virtual ~Face();
	virtual int getPointsNumber() const = 0;
	const std::vector<int>* getPointsIndexes_PTR() const;
	unsigned getFaceType() const;

protected:
	void setPointsIDs(const int* pointsIDs, int pointsNumber);
};

class PointFace : public Face{
protected:
public:
	PointFace(std::vector<Point*>* points, const int* pointsIDs);
	virtual ~PointFace();
	virtual int getPointsNumber() const;
	Point getShape() const;
};

class LineFace : public Face{
protected:
public:
	LineFace(std::vector<Point*>* points, const int* pointsIDs);
	virtual ~LineFace();
	virtual int getPointsNumber() const;
	Line getShape() const;
};

class TriangleFace : public Face{
protected:
public:
	TriangleFace(std::vector<Point*>* points, const int* pointsIDs);
	virtual ~TriangleFace();
	virtual int getPointsNumber() const;
	Triangle getShape() const;
};

class QuadrangleFace : public Face{
protected:
public:
	QuadrangleFace(std::vector<Point*>* points, const int* pointsIDs);
	virtual ~QuadrangleFace();
	virtual int getPointsNumber() const;
	Quadrangle getShape() const;
};

/*Jeden plik obj - jako jeden model*/
/*Wymagane info. max wartosci (do utworzenia minimalnego boxa) oraz face'y*/
class WavefrontOBJ{
private:
	bool firstPointAdded;
	realtype minX;
	realtype maxX;
	realtype minY;
	realtype maxY;
	realtype minZ;
	realtype maxZ;
	std::vector<Point*> points;
	std::vector<const Face*> faces;
public:
	WavefrontOBJ();
	~WavefrontOBJ();

	/*Narazie dwie proste metody, w razie potrzeby mozna poprzeciazac*/
	void addPoint(const realtype* xyz);
	void addPoint(realtype x, realtype y, realtype z);
	void addFace(const int* pointsIDs, int pointsNumber); // pointsNumber - liczba wykorzystywanych elementow tablicy ( n = 1 daje PointFace3D, n=2 LineFace itd)
	const std::vector<const Face*>* getFaces() const;

	realtype getMaxX() const;
	realtype getMaxY() const;
	realtype getMaxZ() const;
	realtype getMinX() const;
	realtype getMinY() const;
	realtype getMinZ() const;

	void sysout();
private:
	// sprawdza czy w danym pinkcie jest jakis max or min i
	// uaktualnia:
	void checkIsMaxOrMin(realtype x, realtype y, realtype z);
};

#endif
