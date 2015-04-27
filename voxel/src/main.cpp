#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "../include/file.h"
#include "../include/image.h"
#include "../include/util.h"
#include "../include/test.h"
#include "../include/wavefront.h"
#include "../include/my_time.h"

using namespace std;
int voxelizator(int argc, char** argv);
int displayHelp();
bool validateParameters(std::string inputFilePath, std::string plane);
GRID_PLANE getGridPlaneEnum(std::string parameter);

int main(int argc, char** argv){
	//testParameterParser1( argc, argv);
	//testTargaImageWriter1();
	//testWavefrontOBJReader1("./torus.obj");
	//testAlgorithmClassesOperators1();
	//testMathematicianRayTriangleIntersection();
	//testMathematicianTriangleBoxOverlap();
	//testAlg2DQuad();
	//testFaceGetShapeMethods("./torus.obj");
	//testQuadTree("./torus.obj");
	//testAlg2DQuad2();
	//testGridVoxelization("./torus.obj");

	if(argc < 2){
		return displayHelp();
	}
	return voxelizator(argc, argv);
}

int voxelizator(int argc, char** argv){
	double startTime, endTime;

	ParameterParser paramParser(argc, argv);
	std::string inputFilePath = paramParser.getStringParameter("-i", ""); // drugi param to defaultowo zwracany
	unsigned xRes = paramParser.getDoubleParameter("-x", 128);
	unsigned yRes = paramParser.getDoubleParameter("-y", 128);
	unsigned zRes = paramParser.getDoubleParameter("-z", 128);
	unsigned imagesNumber = paramParser.getDoubleParameter("-n", 32);
	std::string plane = paramParser.getStringParameter("-p", "c");
	unsigned depth = paramParser.getDoubleParameter("-d", 8);
	unsigned treshold = paramParser.getDoubleParameter("-t", 10);

	if(validateParameters(inputFilePath, plane) == false)
		return -1;

	startTime = nowSeconds();

	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(inputFilePath) );
	WavefrontOBJ* obj = objReader->readWavefrontOBJ_NEW();
	if(obj == NULL){
		delete objReader;
		std::cout << "# Podaj nazwe istniejacego *.obj do wczytania: -i <sciezka>" << std::endl;
		return -1;
	}
	QuadTree quadTree(treshold, depth, obj);
	endTime = nowSeconds();

	std::cout << "# " << "Drzewo utworzone w: " << endTime - startTime << " sekund" << std::endl;

	Grid grid(&quadTree,xRes,yRes,zRes);
	GridWriter gridWriter(&grid, imagesNumber, "./", "cross_section_");
	GRID_PLANE gridPlane = getGridPlaneEnum(plane);

	startTime = nowSeconds();
	grid.voxelization();
	endTime = nowSeconds();

	std::cout << "# Woxelizacja ukonczona w: " << endTime - startTime << " sekund" << std::endl;

	startTime = nowSeconds();
	gridWriter.writeImages(gridPlane);
	endTime = nowSeconds();

	std::cout << "# Zapis obrazow zakonczony w: " << endTime - startTime << " sekund" << std::endl;

	delete objReader;
	delete obj;
	return 0;
}

int displayHelp(){
	std::cout << "# Parametry programu voxelizujacego:" << std::endl;
	std::cout << "# -i <plik>      \t plik wejsciowy *.obj" << std::endl;
	std::cout << "# -x <szerokosc> \t rozdzielczość generowanych danych w osi x (domyslnie: 128)" << std::endl;
	std::cout << "# -y <wysokosc>  \t rozdzielczość danych w osi y (domyslnie: 128)" << std::endl;
	std::cout << "# -z <glebokosc> \t rozdzielczość danych w osi z (domyslnie: 128)" << std::endl;
	std::cout << "# -n <liczba>    \t liczba generowanych obrazów (domyslnie: 32)" << std::endl;
	std::cout << "# -p <wartosc>   \t płaszczyzna generowania przekrojow; mozliwe wartosci: " << std::endl;
	std::cout << "# \t c (ang. coronal) – czolowa, domyslna" << std::endl;
	std::cout << "# \t t (ang. transverse) – poprzeczna" << std::endl;
	std::cout << "# \t s (ang. sagittal) - strzalkowa" << std::endl;
	std::cout << "# -d <liczba>    \t max glebokosc drzewa QuadTree (domyslnie: 8)" << std::endl;
	std::cout << "# -t <liczba>    \t max granica liczby facow w lisciu drzewa, ponizej ktorej nie jest dzielone (domyslnie: 10)" << std::endl;
	return 0;
}

bool validateParameters(std::string inputFilePath, std::string plane){
	if(inputFilePath == ""){
		std::cout << "# Podaj nazwe pliku *.obj do wczytania: -i <sciezka>" << std::endl;
		return false;
	}
	if((plane != "c") && (plane != "t") && (plane != "s")){
		std::cout << "# Dla param. -p podaj jeden ze znakow: c, t lub s" << std::endl;
		return false;
	}

	return true;
}

GRID_PLANE getGridPlaneEnum(std::string parameter){
	if(parameter == "c")
		return GRID_PLANE_CORONAL;
	if(parameter == "t")
		return GRID_PLANE_TRANSVERSE;
	if(parameter == "s")
		return GRID_PLANE_SAGITTAL;
	return GRID_PLANE_CORONAL;
}
