#include "../include/voxel.h"

QuadTree::QuadTree(unsigned facesTreshold, unsigned maxDepth, BoundingBox quadTreeAABB, const std::vector<const Face*>* allFaces ){
	this->facesTreshold = facesTreshold;
	this->maxDepth = maxDepth;
	this->quadTreeAABB = quadTreeAABB;

	Point2D xyMin( quadTreeAABB.xyzMin.x, quadTreeAABB.xyzMin.y);
	Point2D xyMax( quadTreeAABB.xyzMax.x, quadTreeAABB.xyzMax.y);

	Quad2D quad(xyMin, xyMax);
	root = new QuadTreeNode(0, quad, quadTreeAABB, allFaces);
	root->rootSubdivideAll(facesTreshold, maxDepth);
}

QuadTree::QuadTree(unsigned facesTreshold, unsigned maxDepth, const WavefrontOBJ* obj  ){
	Point xyzMin(obj->getMinX(), obj->getMinY(), obj->getMinZ());
	Point xyzMax(obj->getMaxX(), obj->getMaxY(), obj->getMaxZ());
	BoundingBox modelAABB(xyzMin, xyzMax);

	this->facesTreshold = facesTreshold;
	this->maxDepth = maxDepth;
	this->quadTreeAABB = modelAABB;

	Point2D xyMin( quadTreeAABB.xyzMin.x, quadTreeAABB.xyzMin.y);
	Point2D xyMax( quadTreeAABB.xyzMax.x, quadTreeAABB.xyzMax.y);

	Quad2D quad(xyMin, xyMax);
	root = new QuadTreeNode(0, quad, quadTreeAABB, obj->getFaces());
	root->rootSubdivideAll(facesTreshold, maxDepth);
}

std::vector<realtype>* QuadTree::calcRayIntersects(const Point2D& pos){
	Point rayStartPoint(pos.x, pos.y, quadTreeAABB.xyzMin.z);
	Point rayLookAtPoint(pos.x, pos.y, quadTreeAABB.xyzMax.z);
	Ray ray(rayStartPoint, rayLookAtPoint);

	// W razie zwrocenia NULL'a, oznacza, ze punkt jest spoza sciany obiektu wiec napewno taki promien nie przetnie zadnego face'a
	const std::vector<const Face*>* possIsectFaces = root->getPossibleIntersectFaceList(pos);
	if(possIsectFaces == NULL)
		return NULL;

	std::vector<realtype>* zIntersections = new std::vector<realtype>();

	Point isectPoint;
	Triangle triangle;
        Quadrangle quad;
	for(unsigned i = 0 ; i < possIsectFaces->size() ; ++i){
		const Face* nextFace = possIsectFaces->at(i);
		switch(nextFace->getFaceType()){
		case Face::FACE_POINT:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
			break;
		case Face::FACE_LINE:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
			break;
		case Face::FACE_TRIANGLE:

			triangle = ((const TriangleFace*)nextFace)->getShape();
			if( mat.intersectRayTriangle(ray, triangle, &isectPoint) == 1 ){
				//std::cout << "przeciecie na " << isectPoint.z << std::endl;
				zIntersections->push_back(isectPoint.z);
			}

			break;
		case Face::FACE_QUADRANGLE:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
                        
//                        quad = ((const QuadrangleFace*) nextFace)->getShape();
//                        if( mat.intersectRayQuadrangle(ray, quad, &isectPoint) == 1 ){
//				zIntersections->push_back(isectPoint.z);
//			}
			break;
		default:
			std::cerr << __FILE__ << " " << __LINE__ << "To nie powinno sie wykonac !!" << std::endl;
			break;
		}
	}

	std::sort (zIntersections->begin(), zIntersections->end()); // sortowanie rosnaco
	return zIntersections;
}
QuadTree::~QuadTree(){
	delete root;
}


QuadTreeNode::QuadTreeNode(unsigned depth, Quad2D quad, BoundingBox aabb, const std::vector<const Face*>* parentFaces){
	this->depth = depth;
	this->quad = quad;
	this->aabb = aabb;
	leftTop = rightTop = rightBottom = leftBottom = NULL;

	this->prepareOwnFacesList(parentFaces);
}
QuadTreeNode::~QuadTreeNode(){
	// chyba ok bedzie
	delete leftTop;
	delete rightTop;
	delete rightBottom;
	delete leftBottom;
}

void QuadTreeNode::rootSubdivideAll(unsigned facesTreshold,unsigned maxDepth){
	//std::cout << facesTreshold << " zzz " << maxDepth << std::endl;
	// warunki stopu
	if(depth >= maxDepth){
		//std::cout << "depth " << __LINE__ << std::endl;
		return;
	}
	if(myOverlapFaces.size() <= facesTreshold){
		//	std::cout << "treshold " << __LINE__ << std::endl;
		return;
	}
	if(isLeaf() == false){
		//	std::cout << "leaf " << __LINE__ << std::endl;
		return;
	}
	//std::cout << "dziele na 4" << std::endl;
	this->subdivideOn4();
	leftTop->rootSubdivideAll(facesTreshold, maxDepth);
	rightTop->rootSubdivideAll(facesTreshold, maxDepth);
	rightBottom->rootSubdivideAll(facesTreshold, maxDepth);
	leftBottom->rootSubdivideAll(facesTreshold, maxDepth);
}

const std::vector<const Face*>* QuadTreeNode::getPossibleIntersectFaceList(const Point2D& pos){

	if(this->isLeaf())
		if(quad.isPointIn(pos))
			return &myOverlapFaces;

	if(leftTop->quad.isPointIn(pos))
		return leftTop->getPossibleIntersectFaceList(pos);
	if(rightTop->quad.isPointIn(pos))
		return rightTop->getPossibleIntersectFaceList(pos);
	if(rightBottom->quad.isPointIn(pos))
		return rightBottom->getPossibleIntersectFaceList(pos);
	if(leftBottom->quad.isPointIn(pos))
		return leftBottom->getPossibleIntersectFaceList(pos);

	// jesli nie jest w zadnym, zwroci NULL
	return NULL;
}
bool QuadTreeNode::isLeaf(){
	if(leftTop == NULL)
		return true;
	return false;
}

unsigned QuadTreeNode::getDepth(){
	return depth;
}

void QuadTreeNode::subdivideOn4(){
	realtype minZ = aabb.xyzMin.z;
	realtype maxZ = aabb.xyzMax.z;

	Quad2D leftTopQuad = quad.leftTopPart();
	Quad2D rightTopQuad = quad.rightTopPart();
	Quad2D rightBottomQuad = quad.rightBottomPart();
	Quad2D leftBottomQuad = quad.leftBottomPart();

	BoundingBox bbLT(leftTopQuad, minZ, maxZ);
	BoundingBox bbRT(rightTopQuad, minZ, maxZ);
	BoundingBox bbRB(rightBottomQuad, minZ, maxZ);
	BoundingBox bbLB(leftBottomQuad, minZ, maxZ);

	//std::cout << bbLT << std::endl;
	//std::cout << bbRT << std::endl;
	//std::cout << bbRB << std::endl;
	//std::cout << bbLB << std::endl;

	this->leftTop = new QuadTreeNode(depth+1, leftTopQuad, bbLT, &myOverlapFaces);
	this->rightTop = new QuadTreeNode(depth+1, rightTopQuad, bbRT, &myOverlapFaces);
	this->rightBottom = new QuadTreeNode(depth+1, rightBottomQuad, bbRB, &myOverlapFaces);
	this->leftBottom = new QuadTreeNode(depth+1, leftBottomQuad, bbLB, &myOverlapFaces);
	myOverlapFaces.clear();
}

unsigned QuadTreeNode::getIntersectFacesCount(){
	return myOverlapFaces.size();
}

void QuadTreeNode::prepareOwnFacesList(const std::vector<const Face*>* parentFaces){
	Mathematician mat;
	Triangle triangle;
        Quadrangle quad;
	//int counter = 0;
	for(unsigned i = 0 ; i < parentFaces->size() ; ++i){
		const Face* nextFace = parentFaces->at(i);
		switch(nextFace->getFaceType()){
		case Face::FACE_POINT:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
			break;
		case Face::FACE_LINE:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
			break;
		case Face::FACE_TRIANGLE:
			triangle = ((const TriangleFace*)nextFace)->getShape();
			if(mat.intersectTriangleBoundingBox(triangle, aabb)){
				myOverlapFaces.push_back(nextFace);
				//std::cout << "Dodaje " << depth << " "  << std::endl;
				//counter++;
			} // else { std::cout << "Nie dodaje" << std::endl; }
			break;
		case Face::FACE_QUADRANGLE:
			std::cerr << __FILE__ << " " << __LINE__ << "DO DOIMPLEMENTOWANIA, teraz nie powinno sie wykonac !!" << std::endl;
			
//                        quad = ((const QuadrangleFace*) nextFace)->getShape();
//                        if(mat.intersectQuadrangleBoundingBox(quad, aabb)){
//                            myOverlapFaces.push_back(nextFace);
//                        }
                        
                        break;
		default:
			std::cerr << __FILE__ << " " << __LINE__ << "To nie powinno sie wykonac !!" << std::endl;
			break;
		}
	}
	//std::cout << "counter policzyl dla depth " << depth << " obiektow: " << counter << std::endl;
}


Grid::Grid(QuadTree* quadTree, unsigned xRes, unsigned yRes, unsigned zRes){
	this->xRes = xRes;
	this->yRes = yRes;
	this->zRes = zRes;
	this->voxels = NULL;
	this->quadTree = quadTree;

	this->computeGridAABB(this->quadTree->quadTreeAABB);
	this->allocateVoxelsTabMemory();
}
Grid::~Grid(){
	this->freeVoxelsTabMemory();
}

void Grid::voxelization(){
	std::vector<realtype>* zList = NULL;
	for(unsigned i = 0 ; i < xRes ; ++i){
		for(unsigned j = 0 ; j < yRes ; ++j){
			delete zList;
			zList = NULL;
			this->setTmpPoint2D(i, j);
			zList = quadTree->calcRayIntersects(tmpPoint);
			if(zList == NULL){
				for(unsigned k = 0 ; k < zRes ; ++k){
					voxels[i][j][k] = false;
				}
			}else{
				for(unsigned k = 0 ; k < zRes ; ++k){
					realtype zc = this->calculateCellZCenter(k);
					//std::cout << "zc: " << zc << ", zList.size = " << zList->size() << std::endl ;
					int isectsCounter = 0;
					// liczenie wartosci z przeciec mniejszych od wartosci Z centrum komorki
					for(unsigned m = 0 ; m < zList->size() ; ++m){
						if( zc > zList->at(m) ){
							isectsCounter++;
						}else{
							break;
						}
					}
					if( (isectsCounter % 2) == 0 ){
						voxels[i][j][k] = false; // wewnatrz
					}else{
						voxels[i][j][k] = true; //zewnatrz
					}
				}
			}
		}
	}
	delete zList;
}

unsigned Grid::getSizeX() const{
	return xRes;
}
unsigned Grid::getSizeY() const{
	return yRes;
}
unsigned Grid::getSizeZ() const{
	return zRes;
}
bool Grid::getVoxel(unsigned x, unsigned y, unsigned z) const{
	return voxels[x][y][z];
}

void Grid::computeGridAABB(const BoundingBox& quadTreeAABB){
	realtype maxHalf = quadTreeAABB.halfSizeX;
	if(maxHalf < quadTreeAABB.halfSizeY)
		maxHalf = quadTreeAABB.halfSizeY;
	if(maxHalf < quadTreeAABB.halfSizeZ)
		maxHalf = quadTreeAABB.halfSizeZ;

	gridAABB = BoundingBox(quadTreeAABB.center, maxHalf); // szescian
}
void Grid::setTmpPoint2D(unsigned xID, unsigned yID){	// ustawia punkt na centrum kwadraciku o danych indeksach
	realtype eachXLength = (gridAABB.xyzMax.x - gridAABB.xyzMin.x) / xRes;
	realtype eachYLength = (gridAABB.xyzMax.y - gridAABB.xyzMin.y) / yRes;

	tmpPoint.x = gridAABB.xyzMin.x + ( xID * eachXLength + (eachXLength / 2 ));
	tmpPoint.y = gridAABB.xyzMin.y + ( yID * eachYLength + (eachYLength / 2 ));
}
realtype Grid::calculateCellZCenter(unsigned zID){ // zwraca centrum na rzeczywisten osi Z danego voxela w osi siatki Z
	realtype eachLength = (gridAABB.xyzMax.z - gridAABB.xyzMin.z) / zRes;
	//std::cout << "eachLength: " << gridAABB.xyzMin.z << std::endl;
	return gridAABB.xyzMin.z + ( zID * eachLength + (eachLength / 2));
}
void Grid::allocateVoxelsTabMemory(){
	voxels = new bool**[xRes];
	for(unsigned i = 0 ; i < xRes ; ++i){
		voxels[i] = new bool*[yRes];
	}
	for(unsigned i = 0 ; i < xRes ; ++i){
		for(unsigned j = 0 ; j < yRes ; ++j){
			voxels[i][j] = new bool[zRes];
		}
	}
}
void Grid::freeVoxelsTabMemory(){
	for(unsigned i = 0 ; i < xRes ; ++i){
		for(unsigned j = 0 ; j < yRes ; ++j){
			delete[] voxels[i][j];
		}
	}
	for(unsigned i = 0 ; i < xRes ; ++i){
		delete[] voxels[i];
	}
	delete [] voxels;
}
