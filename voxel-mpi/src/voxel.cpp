#include "../include/voxel.h"

#define MASTER_IDD 0

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

	BoundingBox bbLT(leftTopQuad, minZ, maxZ); // left top
	BoundingBox bbRT(rightTopQuad, minZ, maxZ); // right top
	BoundingBox bbRB(rightBottomQuad, minZ, maxZ); // itd
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
					voxels[i][j][k] = 0;
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
						voxels[i][j][k] = 0; // wewnatrz
					}else{
						voxels[i][j][k] = 1; //zewnatrz
					}
				}
			}
		}
	}
	delete zList;
}
// Metoda voxelizacji dla procesu nadrzednego statyczne rozdzielanie
void Grid::voxelizationMaster_MPI_Static(int masterID, int processQuantity){
	std::cout << "# Statyczne rozdzielanie - proces master " << masterID << " voxelizuje" << processQuantity << std::endl;
}
void Grid::voxelizationSlaves_MPI_Static(int slavesID, int processQuantity){
	std::cout << "# Statyczne rozdzielanie - proces slave " << slavesID << " voxelizuje" << processQuantity << std::endl;
}
// Metoda voxelizacji dla procesu nadrzednego dynamiczne rozdzielanie
void Grid::voxelizationMaster_MPI_Dynamic(int masterID, int processQuantity){
	std::cout << "# Dynamiczne rozdzielanie - proces master " << masterID << " voxelizuje" << processQuantity << std::endl;
	unsigned nextRowColumnCode;
	int activeSlaves;
	unsigned nextRow, nextColumn;
	char dataBuffer[zRes];
	MPI_Status status;

	activeSlaves = processQuantity - 1;
	nextRow = 0;
	nextColumn = 0;

	// Wyslanie kazdemu poczatkowego numeru wiersza i kolumny nad ktorym pracuje
	for(int i = 0 ; i < activeSlaves ; i++){
		codeRowColumnLabel(nextRow,nextColumn, &nextRowColumnCode);
		MPI_Send(&nextRowColumnCode, 1, MPI_UNSIGNED, i+1, 0, MPI_COMM_WORLD);
		nextRow++;
		if( nextRow == xRes ){
			nextRow = 0;
			nextColumn++;
		}
	}

	do{
		MPI_Recv(dataBuffer, zRes, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		activeSlaves--;
		// Numer odebranego wiersza to etykieta komunikatu
		unsigned recRowColumnCode = status.MPI_TAG;
		unsigned recRow, recColumn;
		decodeRowColumnLabel(recRowColumnCode, &recRow, &recColumn);
		unsigned slave = status.MPI_SOURCE;
		// Skopiowanie wyniku
		//std::cout << nextRow << " " << nextColumn << std::endl;
		memcpy(voxels[recRow][recColumn], dataBuffer, sizeof(char)*zRes);
		if (nextColumn < yRes) {
			codeRowColumnLabel(nextRow,nextColumn, &nextRowColumnCode);
			MPI_Send(&nextRowColumnCode,1,MPI_UNSIGNED,slave,0,MPI_COMM_WORLD);
			activeSlaves++;
			nextRow++;
			if( nextRow == xRes ){
				nextRow = 0;
				nextColumn++;
			}
		} else {
			//std::cout << "MASTER WYLACZA SLAVE" << std::endl;
			// Nie, wyślij sygnał zakończenia procesu podrzędnego
			unsigned end= 0xFFFFFFFF;
			MPI_Send(&end,1,MPI_UNSIGNED,slave,0,MPI_COMM_WORLD);
		}


	}while(activeSlaves > 0);
}
void Grid::voxelizationSlaves_MPI_Dynamic(int slavesID, int processQuantity){
	std::cout << "# Dynamiczne rozdzielanie - proces slave " << slavesID << " voxelizuje " << processQuantity << std::endl;

	char dataBuffer[zRes];
	unsigned rowColumnCode;
	unsigned row, column;
	std::vector<realtype>* zList = NULL;
	MPI_Status status;

	while(1){
		MPI_Recv(&rowColumnCode, 1, MPI_UNSIGNED, MASTER_IDD ,0,MPI_COMM_WORLD, &status);
		if( rowColumnCode == 0xFFFFFFFF ){
			//std::cout << "# SLAVE" << slavesID << ", KONCZE PRACE !!!" << std::endl;
			return;
		}
		decodeRowColumnLabel(rowColumnCode, &row, &column);
		//std::cout << "# slave" << slavesID << ", wiersz: " << row << ", kolumna: " << column << std::endl;

		// Obliczenie "wiersza" o podanym x i y (czyli serii kwadracikow po wybranym X
		delete zList;
		zList = NULL;
		this->setTmpPoint2D(row, column);
		zList = quadTree->calcRayIntersects(tmpPoint);
		if(zList == NULL){
			for(unsigned k = 0 ; k < zRes ; ++k){
				dataBuffer[k] = 0;
			}
		}else{
			for(unsigned k = 0 ; k < zRes ; ++k){
				realtype zc = this->calculateCellZCenter(k);
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
					dataBuffer[k] = 0; // wewnatrz
				}else{
					dataBuffer[k] = 1; //zewnatrz
				}
			}
		}

		// Przeslanie obliczonego elementu
		MPI_Send(dataBuffer,zRes,MPI_CHAR, MASTER_IDD ,rowColumnCode,MPI_COMM_WORLD);
	}
}

// to co wyzej ale wierszami
void Grid::voxelizationMaster_MPI_DynamicRows(int masterID, int processQuantity){
	std::cout << "# Dynamiczne rozdzielanie - proces master " << masterID << " voxelizuje" << processQuantity << std::endl;
	int activeSlaves;
	unsigned nextRow, nextColumn;
	char dataBuffer[zRes*yRes];
	MPI_Status status;

	activeSlaves = processQuantity - 1;
	nextRow = 0;
	nextColumn = 0;

	// Wyslanie kazdemu  poczatkowego numeru wierszanad ktorym pracuje
	for(int i = 0 ; i < activeSlaves ; i++){

		MPI_Send(&nextRow, 1, MPI_UNSIGNED, i+1, 0, MPI_COMM_WORLD);
		nextRow++;
		if( nextRow == xRes ){
			nextRow = 0;
			nextColumn++;
		}
	}

	do{
		MPI_Recv(dataBuffer, zRes*yRes, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		activeSlaves--;
		// Numer odebranego wiersza to etykieta komunikatu
		unsigned recRow = status.MPI_TAG;
		unsigned slave = status.MPI_SOURCE;
		// Skopiowanie wyniku
		//std::cout << nextRow << " " << nextColumn << std::endl;
		for(unsigned i = 0 ; i < yRes ; i++){
			memcpy(voxels[recRow][i], &dataBuffer[i*yRes], sizeof(char)*zRes);
		}
		//std::cout << "robie wiersz " << nextRow << std::endl;
		if (nextRow < xRes) {
			MPI_Send(&nextRow,1,MPI_UNSIGNED,slave,0,MPI_COMM_WORLD);
			activeSlaves++;
			nextRow++;
//			if( nextRow == xRes ){
//				nextRow = 0;
//				nextColumn++;
//			}
		} else {
			//std::cout << "MASTER WYLACZA SLAVE" << std::endl;
			// Nie, wyślij sygnał zakończenia procesu podrzędnego
			unsigned end= 0xFFFFFFFF;
			MPI_Send(&end,1,MPI_UNSIGNED,slave,0,MPI_COMM_WORLD);
		}


	}while(activeSlaves > 0);
}
// to co wyzej ale calymi wierszami
void Grid::voxelizationSlaves_MPI_DynamicRows(int slavesID, int processQuantity){
	std::cout << "# Dynamiczne rozdzielanie - proces slave " << slavesID << " voxelizuje" << processQuantity << std::endl;

	char dataBuffer[zRes*yRes];
	unsigned row;
	std::vector<realtype>* zList = NULL;
	MPI_Status status;

	while(1){
		MPI_Recv(&row, 1, MPI_UNSIGNED, MASTER_IDD ,0,MPI_COMM_WORLD, &status);
		if( row == 0xFFFFFFFF ){
			//std::cout << "# SLAVE" << slavesID << ", KONCZE PRACE !!!" << std::endl;
			return;
		}
		//std::cout << "# slave" << slavesID << ", wiersz: " << row << ", kolumna: " << column << std::endl;
		for(unsigned j = 0 ; j < yRes ; ++j){
			// Obliczenie "wiersza" o podanym x i y (czyli serii kwadracikow po wybranym X
			delete zList;
			zList = NULL;
			this->setTmpPoint2D(row, j);
			zList = quadTree->calcRayIntersects(tmpPoint);
			if(zList == NULL){
				for(unsigned k = 0 ; k < zRes ; ++k){
					dataBuffer[k+j*yRes] = 0;
				}
			}else{
				for(unsigned k = 0 ; k < zRes ; ++k){
					realtype zc = this->calculateCellZCenter(k);
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
						dataBuffer[k+j*yRes] = 0; // wewnatrz
					}else{
						dataBuffer[k+j*yRes] = 1; //zewnatrz
					}
				}
			}
		}
		// Przeslanie obliczonego elementu
		MPI_Send(dataBuffer,zRes*yRes,MPI_CHAR, MASTER_IDD ,row,MPI_COMM_WORLD);
	}
}

void Grid::codeRowColumnLabel(unsigned row, unsigned column, unsigned* resultLabel){
	*resultLabel = (row & 0x0000FFFF) << 16 ;
	*resultLabel = (*resultLabel) | (0x0000FFFF & column);
}
void Grid::decodeRowColumnLabel(unsigned label, unsigned* resultRow, unsigned* resultColumn){
	*resultColumn = label & 0x0000FFFF;
	label = label >> 16;
	*resultRow = label & 0x0000FFFF;
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
char Grid::getVoxel(unsigned x, unsigned y, unsigned z) const{
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
	voxels = new char**[xRes];
	for(unsigned i = 0 ; i < xRes ; ++i){
		voxels[i] = new char*[yRes];
	}
	for(unsigned i = 0 ; i < xRes ; ++i){
		for(unsigned j = 0 ; j < yRes ; ++j){
			voxels[i][j] = new char[zRes];
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
