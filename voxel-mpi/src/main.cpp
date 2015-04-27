#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <mpi.h>
#include "../include/file.h"
#include "../include/image.h"
#include "../include/util.h"
#include "../include/test.h"
#include "../include/wavefront.h"

#define MIN_PROCESS_NUM_REQUIRED 2
#define MASTER_ID 0

using namespace std;
int voxelizator(int argc, char** argv);
int voxelizatorMaster(int masterID, int processQuantity , std::string inputFile, unsigned xRes, unsigned yRes, unsigned zRes, unsigned n, std::string plane, unsigned depth, unsigned treshold, bool bySingle);
int voxelizatorSlaves(int slaveID, int processQuantity , unsigned xRes, unsigned yRes, unsigned zRes, unsigned depth, unsigned treshold, bool bySingle);

void displayHelp();
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
	return voxelizator(argc, argv);
}

int voxelizator(int argc, char** argv){
	MPI_Init(&argc, &argv);
	int myRank, processNumber;
	MPI_Comm_size(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if(argc < 2){
		if(myRank == MASTER_ID){
			displayHelp();
		}
		MPI_Finalize();
		return 0;
	}

	if(processNumber < MIN_PROCESS_NUM_REQUIRED){
		printf("# Program na conajmniej 2 procesory. Prosze podac parametr -n <liczba wieksza od 1>\n");
		MPI_Finalize();
		return -1;
	}

	ParameterParser paramParser(argc, argv);
	std::string inputFilePath = paramParser.getStringParameter("-i", ""); // drugi param to defaultowo zwracany
	unsigned xRes = paramParser.getDoubleParameter("-x", 128);
	unsigned yRes = paramParser.getDoubleParameter("-y", 128);
	unsigned zRes = paramParser.getDoubleParameter("-z", 128);
	unsigned imagesNumber = paramParser.getDoubleParameter("-n", 32);
	std::string plane = paramParser.getStringParameter("-p", "c");
	// Dla budowania drzewa
	unsigned depth = paramParser.getDoubleParameter("-d", 8);
	unsigned treshold = paramParser.getDoubleParameter("-t", 10);
	bool bySingle = paramParser.haveFlag("-single");

	if(validateParameters(inputFilePath, plane) == false){
		MPI_Finalize();
		return -1;
	}

	if(myRank == MASTER_ID){
		return voxelizatorMaster(myRank, processNumber, inputFilePath, xRes, yRes, zRes, imagesNumber, plane, depth, treshold, bySingle);
	}else{
		return voxelizatorSlaves(myRank, processNumber, xRes, yRes, zRes, depth, treshold, bySingle);
	}

	MPI_Finalize();
}

int voxelizatorMaster(int masterID, int processQuantity,
		std::string inputFile,
		unsigned xRes,
		unsigned yRes,
		unsigned zRes,
		unsigned imagesNumber,
		std::string plane,
		unsigned depth,
		unsigned treshold,
		bool bySingle){

	// KOD:
	double startTime, endTime;

	startTime = MPI_Wtime();

	int badFileFlag = 0;
	WavefrontOBJFileReader* objReader = new WavefrontOBJFileReader( FilePath(inputFile) );
	WavefrontOBJ_Data* objData = objReader->readWavefrontOBJ_Data();
	if(objData == NULL){
		badFileFlag = 1;
	}
	delete objReader;

	// Tutaj wyslanie wiadomosci, czy podano dobry plik:
	MPI_Bcast( &badFileFlag, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	if(badFileFlag == 1){
		std::cout << "#" << masterID <<" Problem z plikiem: " << inputFile << std::endl;
		MPI_Finalize();
		return -1;
	}

	// Wyslanie rozmiarow wymaganych do utworzenia tablic ( POTEM ZAMIENIC TO NA PAKOWANIE)
	MPI_Bcast( &objData->verticesVarNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->pointsIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->linesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->trianglesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->rectanglesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);

	// Wyslanie danych wierzcholkow i indeksow face'ow (POTEM ZAMIENIC NA JEDEN SPAKOWANY KOMUNIKAT)
	MPI_Bcast( objData->vertices, objData->verticesVarNumber, MPI_FLOAT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesPointsIDs, objData->pointsIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesLinesIDs, objData->linesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesTrianglesIDs, objData->trianglesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesRectanglesIDs, objData->rectanglesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

	endTime = MPI_Wtime();

	std::cout << "# MASTER Rozeslane surowe dane pliku obj w: " << endTime - startTime << " s" << std::endl;

	startTime = MPI_Wtime();
	WavefrontOBJ* obj = new WavefrontOBJ(*objData);
	QuadTree quadTree(treshold, depth, obj); // treshold=10, treeDepth=8;
	endTime = MPI_Wtime();

	std::cout << "# " << "MASTER Drzewo utworzone w: " << endTime - startTime << " s" << std::endl;

	Grid grid(&quadTree, xRes, yRes, zRes);
	GridWriter gridWriter(&grid, imagesNumber, "./", "cross_section_");
	GRID_PLANE gridPlane = getGridPlaneEnum(plane);

	MPI_Barrier(MPI_COMM_WORLD); // poczekanie, az wszystkie procesy potworza swoje drzewa
	startTime = MPI_Wtime();
	if(bySingle == false)
		grid.voxelizationMaster_MPI_DynamicRows(masterID, processQuantity);
	else
		grid.voxelizationMaster_MPI_Dynamic(masterID, processQuantity);
	endTime = MPI_Wtime();

	std::cout << "#MASTER Woxelizacja ukonczona w: " << endTime - startTime << " sekund" << std::endl;

	// Tylko master zapisuje wyniki do pliku
	startTime = MPI_Wtime();
	gridWriter.writeImages(gridPlane);
	endTime = MPI_Wtime();

	std::cout << "#MASTER Zapis obrazow zakonczony w: " << endTime - startTime << " sekund" << std::endl;

	delete obj;
	MPI_Finalize();
	return 0;
}
int voxelizatorSlaves(int slaveID, int processQuantity, unsigned xRes, unsigned yRes, unsigned zRes, unsigned depth, unsigned treshold, bool bySingle){
	// jesli udalo sie wczytac plik
	int badFileFlag = 0;
	MPI_Bcast( &badFileFlag, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	if(badFileFlag == 1){
		MPI_Finalize();
		return -1;
	}

	WavefrontOBJ_Data* objData = new WavefrontOBJ_Data();
	// Odebranie wymaganych wielkosci buforow
	MPI_Bcast( &objData->verticesVarNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->pointsIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->linesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->trianglesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( &objData->rectanglesIDsNumber, 1, MPI_UNSIGNED, MASTER_ID, MPI_COMM_WORLD);

	objData->vertices = new realtype[objData->verticesVarNumber];
	objData->facesPointsIDs = new int[objData->pointsIDsNumber];
	objData->facesLinesIDs = new int[objData->linesIDsNumber];
	objData->facesTrianglesIDs = new int[objData->trianglesIDsNumber];
	objData->facesRectanglesIDs = new int[objData->rectanglesIDsNumber];

	// Odebranie surowych danych od mastera na temat obiektu
	MPI_Bcast( objData->vertices, objData->verticesVarNumber, MPI_FLOAT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesPointsIDs, objData->pointsIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesLinesIDs, objData->linesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesTrianglesIDs, objData->trianglesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
	MPI_Bcast( objData->facesRectanglesIDs, objData->rectanglesIDsNumber, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

	WavefrontOBJ* obj = new WavefrontOBJ(*objData);
	QuadTree quadTree(treshold, depth, obj); // treshold=10, treeDepth=8;
	std::cout << "# " << "Slave" << slaveID << " - " << "drzewo utworzone" << std::endl;
	Grid grid(&quadTree,xRes,yRes,zRes);

	MPI_Barrier(MPI_COMM_WORLD); // poczekanie, az wszystkie procesy potworza swoje drzewa
	if(bySingle == false)
		grid.voxelizationSlaves_MPI_DynamicRows(slaveID, processQuantity);
	else
		grid.voxelizationSlaves_MPI_Dynamic(slaveID, processQuantity);

	std::cout << "# " << "Slave" << slaveID << " - " << "wokselizacja ukonczona" << std::endl;
	delete obj;
	MPI_Finalize();
	return 0;
}

void displayHelp(){
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
	std::cout << "# -single		   \t flaga powodujaca ze grid nie jest rozdzielany wierszami, a pojedynczymi elementami dla danego x i y (z zmienne)" << std::endl;

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
