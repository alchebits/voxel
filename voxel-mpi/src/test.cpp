#include "../include/test.h"


void testParameterParser1(int argc, char** argv){
	std::string inputFileParam = "-i";
	std::string widthParam = "-x";
	std::string heightParam = "-y";
	std::string depthParam = "-z";
	std::string imageNumberParam = "-n";
	std::string sectionPlaneParam = "-p";

	ParameterParser paramParser(argc, argv);

	if(paramParser.haveStringParameter(inputFileParam)){
		std::cout << paramParser.getStringParameter(inputFileParam, "default input") << std::endl;
	}
	if(paramParser.haveDoubleParameter(widthParam)){
		std::cout << paramParser.getDoubleParameter(widthParam, 15) << std::endl;
	}
	if(paramParser.haveDoubleParameter(heightParam)){
		std::cout << paramParser.getDoubleParameter(heightParam, 20) << std::endl;
	}
	if(paramParser.haveDoubleParameter(depthParam)){
		std::cout << paramParser.getDoubleParameter(depthParam, 25) << std::endl;
	}
	if(paramParser.haveDoubleParameter(imageNumberParam)){
		std::cout << paramParser.getDoubleParameter(imageNumberParam, 12) << std::endl;
	}
	if(paramParser.haveStringParameter(sectionPlaneParam)){
		std::cout << paramParser.getStringParameter(sectionPlaneParam, "defaultowy") << std::endl;
	}
}

void testTargaImageWriter1(){
	TargaImageRGB tgaImageRGB(512, 512);
	for(int i = 0 ; i < 512 ; ++i){
		for(int j = 0 ; j < 512 ; ++j){
			tgaImageRGB.setPixel(i, j,  i%256, j%256, 0); //x,y, r,g,b
		}
	}

	FilePath filePath("./imgTargaTest1.tga");
	TargaImageFileWriter imgWriter(filePath);
	imgWriter.writeTargaImage(&tgaImageRGB);
}

void testWavefrontOBJReader1(std::string objFilePath){
	WavefrontOBJ* obj = NULL;
	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(objFilePath) );
	obj = objReader->readWavefrontOBJ_NEW();
	obj->sysout();
	//getchar();
	delete objReader;
	delete obj;
}

void testAlgorithmClassesOperators1(){
	Vector vec1(1,2,3);
	Vector vec2(1,2,4);
	std::cout << ( vec1 != vec2) << std::endl;

	Vector nullVec = (Vector)0;
	std::cout << nullVec << std::endl;

	Vector vec1scal = 5 * vec1;
	std::cout << vec1scal << std::endl;

	Vector xVec(1,0,0);
	Vector yVec(0,1,0);
	Vector crossVec = xVec * yVec;
	std::cout << crossVec << std::endl;
}

void testMathematicianRayTriangleIntersection(){

	Mathematician mat;

	Point PO(0,0,-1);
	Point P1(0,0,1);
	Ray ray(PO,P1);

	Point tp1(-0.5, -4.5, 3.0);
	Point tp2(1.5, -0.5, 3.0);
	Point tp3(0, 2.5, 1.0);
	Triangle triangle(tp1,tp2,tp3);

	Point tp12(0.5,-0.5,0.5);
	Point tp22(0.5,-0.5,0);
	Point tp32(0.5,0.5,0);
	Triangle triangle2(tp12,tp22,tp32);
	Point I;
	std::cout << "Czy przecina: " << mat.intersectRayTriangle(ray,triangle,&I) << std::endl;
	std::cout << "Punkt przeciecia: " << I << std::endl;
	std::cout << mat.intersectRayTriangle(ray,triangle2,&I) << std::endl;
	std::cout << I << std::endl;
}

void testMathematicianTriangleBoxOverlap(){
	Mathematician mat;

	Point xyzMin(-2,-3,-4);
	Point xyzMax(6,5,6);
	BoundingBox box(xyzMin, xyzMax);

	Point p1(-100, -100, 0);
	Point p2(100, -100, 0);
	Point p3(0,100,0);
	Triangle triangle(p1,p2,p3);
	std::cout << "troj. w box: " << mat.intersectTriangleBoundingBox(triangle,box) << std::endl;

	Point p12(-1, -1, 20);
	Point p22(1, -1, 20);
	Point p32(0, 1, 20);
	Triangle triangle2(p12,p22,p32);
	std::cout << "troj2. w box: " << mat.intersectTriangleBoundingBox(triangle2,box) << std::endl;
}
void testAlg2DQuad(){
	Point2D xyMin(2,2);
	Point2D xyMax(8,10);
	Quad2D quad(xyMin, xyMax);
	Quad2D leftTop = quad.leftTopPart();
	Quad2D rightTop = quad.rightTopPart();
	Quad2D rightBottom = quad.rightBottomPart();
	Quad2D leftBottom = quad.leftBottomPart();

	std::cout << "full: " << quad << std::endl;
	std::cout << "lt: " << leftTop << std::endl;
	std::cout << "rt: " << rightTop << std::endl;
	std::cout << "rb: " << rightBottom << std::endl;
	std::cout << "lb: " << leftBottom << std::endl;

	std::cout << quad.isPointIn(Point2D(8,10)) << std::endl;
}

void testAlg2DQuad2(){
	Point2D xyMin(-1.42852,-0.339851);
	Point2D xyMax(1.43,0.339851);
	Quad2D quad(xyMin, xyMax);
	Quad2D leftTop = quad.leftTopPart();
	Quad2D rightTop = quad.rightTopPart();
	Quad2D rightBottom = quad.rightBottomPart();
	Quad2D leftBottom = quad.leftBottomPart();

	std::cout << "full: " << quad << std::endl;
	std::cout << "lt: " << leftTop << std::endl;
	std::cout << "rt: " << rightTop << std::endl;
	std::cout << "rb: " << rightBottom << std::endl;
	std::cout << "lb: " << leftBottom << std::endl;
	//	maxX: 1.43
	//	maxY: 0.339851
	//	maxZ: 1.42963
	//	minX: -1.42852
	//	minY: -0.339851
	//	minZ: -1.42963
	std::cout << quad.isPointIn(Point2D(8,10)) << std::endl;
}

void testFaceGetShapeMethods(std::string objFilePath){
	WavefrontOBJ* obj = NULL;
	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(objFilePath) );
	obj = objReader->readWavefrontOBJ_NEW();
	//obj->sysout();

	const std::vector<const Face*>* faces = obj->getFaces();

	for(unsigned i = 0 ; i < faces->size() ; ++i){
		const Face* nextFace = faces->at(i);
		switch(nextFace->getFaceType()){
		case Face::FACE_POINT:
			std::cout << "p"<< i <<":\n"<< ((const PointFace*)nextFace)->getShape() << std::endl;
			break;
		case Face::FACE_LINE:
			std::cout << "l"<< i <<":\n"<< ((const LineFace*)nextFace)->getShape() << std::endl;
			break;
		case Face::FACE_TRIANGLE:
			std::cout << "t"<< i <<":\n"<< ((const TriangleFace*)nextFace)->getShape() << std::endl;
			break;
		case Face::FACE_QUADRANGLE:
			std::cout << "q"<< i <<":\n"<< ((const QuadrangleFace*)nextFace)->getShape() << std::endl;
			break;
		default:
			std::cerr << __FILE__ << " " << __LINE__ << "To nie powinno sie wykonac !!" << std::endl;
			break;
		}
	}

	delete objReader;
	delete obj;
}

void testQuadTree(std::string objFilePath){
	WavefrontOBJ* obj = NULL;
	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(objFilePath) );
	obj = objReader->readWavefrontOBJ_NEW();

	realtype minX = obj->getMinX();
	realtype minY = obj->getMinY();
	realtype minZ = obj->getMinZ();

	realtype maxX = obj->getMaxX();
	realtype maxY = obj->getMaxY();
	realtype maxZ = obj->getMaxZ();

	Point xyzMin(minX,minY,minZ);
	Point xyzMax(maxX,maxY,maxZ);
	BoundingBox modelAABB(xyzMin, xyzMax);

	QuadTree quadTree(10,8, modelAABB, obj->getFaces());

	delete objReader;
	delete obj;
}

void testGridVoxelization(std::string objFilePath){
	WavefrontOBJ* obj = NULL;
	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(objFilePath) );
	obj = objReader->readWavefrontOBJ_NEW();

	realtype minX = obj->getMinX();
	realtype minY = obj->getMinY();
	realtype minZ = obj->getMinZ();

	realtype maxX = obj->getMaxX();
	realtype maxY = obj->getMaxY();
	realtype maxZ = obj->getMaxZ();

	Point xyzMin(minX,minY,minZ);
	Point xyzMax(maxX,maxY,maxZ);
	BoundingBox modelAABB(xyzMin, xyzMax);

	QuadTree quadTree(10,8, modelAABB, obj->getFaces());
	unsigned res = 512;
	Grid grid(&quadTree,res,res,res);

	grid.voxelization();

	TargaImageRGB tgaImageRGB(res, res);
	for(unsigned i = 0 ; i < res ; ++i){
		for(unsigned j = 0 ; j < res ; ++j){
			if( grid.getVoxel(i,j,res/2) ){
				tgaImageRGB.setPixel(i,j, 255,255,255);
			}else{
				tgaImageRGB.setPixel(i,j, 0,0,0);
			}
		}
	}

	FilePath filePath("./imgTargaTest1.tga");
	TargaImageFileWriter imgWriter(filePath);
	imgWriter.writeTargaImage(&tgaImageRGB);

	delete objReader;
	delete obj;
}


