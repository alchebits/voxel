#ifndef VOXEL_H_
#define VOXEL_H_

#include <iostream>
#include <algorithm>
#include "algorithm2d.h"
#include "algorithm3d.h"
#include "wavefront.h"

class QuadTree;
class QuadTreeNode;
class Grid;


class QuadTree{
private:
	Mathematician mat;
	unsigned facesTreshold; // granica ilosci facow, ponizej ktorej nie trzeba juz "ciac" quada znowu na 4
	unsigned maxDepth; // max. dopuszczalny "podzial"
	BoundingBox quadTreeAABB; // pudlo otaczajace caly model (jak najciasniej sie da)
	QuadTreeNode* root;
public:
	QuadTree(unsigned facesTreshold, unsigned maxDepth, BoundingBox quadTreeAABB, const std::vector<const Face*>* allFaces );
	QuadTree(unsigned facesTreshold, unsigned maxDepth, const WavefrontOBJ* obj  );
	~QuadTree();
	std::vector<realtype>* calcRayIntersects(const Point2D& pos);

	friend class Grid;
};



class QuadTreeNode{
private:
	QuadTreeNode* leftTop;
	QuadTreeNode* rightTop;
	QuadTreeNode* rightBottom;
	QuadTreeNode* leftBottom;

	unsigned depth;
	Quad2D quad; // opisuje powieszchnie danego wezla (kwadracik)
	BoundingBox aabb; // box, ktory bedzie tworzony dla danego wezla przy uzyciu quada oraz minz maxz
	std::vector<const Face*> myOverlapFaces; // Tylko liscie beda mialy wypelniana liste przecinajacych faceow, jesli lisc przestaje nim byc, nalezy ta liste wyczyscic

public:
	QuadTreeNode(unsigned depth, Quad2D quad, BoundingBox aabb, const std::vector<const Face*>* parentFaces);
	~QuadTreeNode();
	void rootSubdivideAll(unsigned facesTreshold,unsigned maxDepth); // dzieli drzewo do czasu osiagniecia granic

	// zwraca null, w razie punktu spoza listy
	const std::vector<const Face*>* getPossibleIntersectFaceList(const Point2D& pos); // funkcja wyszukuje lisc z kwadratem, w ktorym zawiera sie punkt. (promien start z punktu ma szanse przeciac te facey
	bool isLeaf(); // true, jesli lisc
	unsigned getDepth();
	unsigned getIntersectFacesCount(); // zwraca liste przecinajacych face-ow

	friend class QuadTree;
private:
	void subdivideOn4(); // powoduje dodane nowych 4 quadzikow i wywolanie algorytmu wyszukujacego przecinajace		 // facey, dla wezla, przez co ten przestaje byc lisciem	   /
	void prepareOwnFacesList(const std::vector<const Face*>* parentFaces);	// z listy rodzica przeglada face'y i dodaje do swojej listy te, co go przecinaja
};


class Grid{
protected:
	QuadTree* quadTree;
	unsigned xRes, yRes, zRes;
	bool*** voxels;
	BoundingBox gridAABB; // ten box jest "szescienny" gdzie dlugosc kazdej sciany to max odleglosc w AABB drzewa QuadTree
	Point2D tmpPoint;
public:
	Grid(QuadTree* quadTree, unsigned xRes, unsigned yRes, unsigned zRes);
	~Grid();

	void voxelization(); // odpala proces voxelizacji obiektu. po wywolaniu metody zawiera w sobie gotowa siatke

	bool getVoxel(unsigned x, unsigned y, unsigned z) const;
	unsigned getSizeX() const;
	unsigned getSizeY() const;
	unsigned getSizeZ() const;
private:
	void setTmpPoint2D(unsigned xID, unsigned yID); // ustawia punkt na centrum odpowiadajacej siatki
	realtype calculateCellZCenter(unsigned zID);
	void computeGridAABB(const BoundingBox& quadTreeAABB);
	void allocateVoxelsTabMemory();
	void freeVoxelsTabMemory();
};

#endif
