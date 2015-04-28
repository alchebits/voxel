#include "../include/file.h"

Path::Path(const std::string& path) : path(path){
}
Path::~Path(){
}
const std::string& Path::getPath() const{
	return this->path;
}
void Path::setPath(const std::string& path){
	this->path = path;
}

FilePath::FilePath(const std::string& path) : Path(path){
}
FilePath::~FilePath(){
}


FileReader::FileReader(const FilePath& filePath) : filePath(filePath){
}
FileReader::~FileReader(){
}


TextFileReader::TextFileReader(const FilePath& filePath) : FileReader(filePath){
}
TextFileReader::~TextFileReader(){
}


#define OBJ_OBJECT "o" // 1 x string
#define OBJ_VERTEX "v" // 3 x float
#define OBJ_TEXCOORD "vt" // 1 - 3 x float
#define OBJ_NORMAL "vn" // 3 x float
#define OBJ_SMOOTH "s" // 1 x string==off or 1-n x int
#define OBJ_GROUP "g" // 1 x string
#define OBJ_USE_MATERIAL "usemtl" // 1 x string
#define OBJ_MATERIAL_LIB "mtllib" // 1 x string
#define OBJ_FACE "f" // 1 - 4 x (float or float/float, or float/float/float, or float//float)

WavefrontOBJFileReader::WavefrontOBJFileReader(const FilePath& filePath) : TextFileReader(filePath){
}
WavefrontOBJFileReader::~WavefrontOBJFileReader(){
}
WavefrontOBJ* WavefrontOBJFileReader::readWavefrontOBJ_NEW(){
	return this->readWavefrontOBJ_NEW(filePath.getPath());
}
WavefrontOBJ* WavefrontOBJFileReader::readWavefrontOBJ_NEW(const std::string& path){
	std::string strLineBuffer;
	std::string strTokenBuffer;
	std::string strFaceBuffer;
	std::string strMtlLib;
	unsigned uSpace;
	StringTokenizer lineTokenizer(false);
	StringTokenizer faceTokenizer(false,std::string("/") );

	// bufory do wczytywania danych
	realtype xyzBuffer[3];

	int facePointsIDs[4];
	int facePointsCounter=0;

	IOWTextFileReader objFile( path.c_str() );
	if(!objFile.isOpened()){
		return NULL;
	}

	WavefrontOBJ* wavefrontOBJ = new WavefrontOBJ();

	// Wczytywanie pliku liniami i parsowanie calego pliku obj
	while(objFile.nextLine(strLineBuffer)){
		//std::cout << strLineBuffer << std::endl;
		lineTokenizer.loadString(strLineBuffer); // zaladowanie stringa do tokenizowania
		// parsowanie aktualnej linii
		while(lineTokenizer.nextToken(strTokenBuffer)) {
			// 'v' wieszcholek (3 floaty)
			if( strcmp(strTokenBuffer.c_str(),OBJ_VERTEX) == 0 ){
				// wczytywanie kolejnych danych wieszcholka
				for(unsigned i = 0 ; lineTokenizer.nextToken(strTokenBuffer) ; ++i){
					xyzBuffer[ i ] = atof( strTokenBuffer.c_str() );
				}
				wavefrontOBJ->addPoint(xyzBuffer);
			}
			// 'f' -powieszchnia od 1 do 4 opisow powieszchni, kazdy od 1 do 3 usnigned (ew oddzielone / )
			if( strcmp(strTokenBuffer.c_str(),OBJ_FACE) == 0 ){
				facePointsCounter = 0;
				while(lineTokenizer.nextToken(strTokenBuffer)){
					faceTokenizer.loadString(strTokenBuffer);
					for(unsigned i = 0 ; faceTokenizer.nextToken(strFaceBuffer,uSpace) ; ++i ){
						// index danych vertexa
						if(i == 0 ){
							facePointsIDs[facePointsCounter++] = atoi( strFaceBuffer.c_str() );
						}else
							// dane normalnej ( nie bylo danych tekstury)
							if(i == 1 && uSpace == 2){

							}else
								// dane texcoordow
								if( i == 1 && uSpace == 1){

								}else
									// dane normalnej (byly dane texcoorda)
									if(i == 2 && uSpace == 1){

									}
					}
				}
				wavefrontOBJ->addFace(facePointsIDs,facePointsCounter);
			}
		}
	}
	objFile.closeFile();
	return wavefrontOBJ;
}

FileWriter::FileWriter(const FilePath& filePath) : filePath(filePath){
}
FileWriter::~FileWriter(){
}


BinaryFileWriter::BinaryFileWriter(const FilePath& filePath) : FileWriter(filePath){
}
BinaryFileWriter::~BinaryFileWriter(){
}


TargaImageFileWriter::TargaImageFileWriter(const FilePath& filePath) : BinaryFileWriter(filePath){
}
TargaImageFileWriter::~TargaImageFileWriter(){
}
void TargaImageFileWriter::writeTargaImage(const TargaImage* tgaImage){
	this->writeTargaImage(tgaImage, filePath.getPath());
}
void TargaImageFileWriter::writeTargaImage(const TargaImage* tgaImage, const std::string& path){
	std::ofstream ofs(path.c_str(), std::ios::binary);

	const TargaHeader* tgaHeader = tgaImage->getHeaderPTR();
	const char* pixels = (const char*)tgaImage->getPixelsPTR();
	long pixelsSizeBytes = tgaImage->sizeInBytes();

	ofs.write((char*)&tgaHeader->identSize,1);
	ofs.write((char*)&tgaHeader->colorMapType,1);
	ofs.write((char*)&tgaHeader->imageType,1);
	ofs.write((char*)&tgaHeader->colorMapStart,2);
	ofs.write((char*)&tgaHeader->colorMapLength,2);
	ofs.write((char*)&tgaHeader->colorMapBits,1);
	ofs.write((char*)&tgaHeader->xStart,2);
	ofs.write((char*)&tgaHeader->yStart,2);
	ofs.write((char*)&tgaHeader->width,2);
	ofs.write((char*)&tgaHeader->height,2);
	ofs.write((char*)&tgaHeader->bitsPerPixel,1);
	ofs.write((char*)&tgaHeader->descriptor,1);
	ofs.write(pixels, pixelsSizeBytes);
	ofs.close();
}



GridWriter::GridWriter( const Grid* grid,
		unsigned imagesNumber,
		const std::string& catalogPath,
		const std::string& imageBaseName){

	this->grid = grid;
	this->catalogPath = catalogPath;
	this->imagesNumber = imagesNumber;
	this->imageBaseName = imageBaseName;
}

GridWriter::~GridWriter(){
}

bool GridWriter::writeImages(GRID_PLANE plane){
	unsigned width = 0;
	unsigned heigth = 0;
	unsigned depth = 0;

	switch(plane){
	case GRID_PLANE_CORONAL:
		width = grid->getSizeX();
		heigth = grid->getSizeY();
		depth = grid->getSizeZ();
		break;
	case GRID_PLANE_TRANSVERSE:
		width = grid->getSizeZ();
		heigth = grid->getSizeX();
		depth = grid->getSizeY();
		break;
	case GRID_PLANE_SAGITTAL:
		width = grid->getSizeX();
		heigth = grid->getSizeZ();
		depth = grid->getSizeY();
		break;
	default:
		std::cerr << "--(!): Case w tej linijce nie powinien sie wykonac:" << __LINE__ << std::endl;
		break;
	}

	// "c" - xy kol. ob w z
	TargaImageGrey tgaImageGrey(width, heigth);
	for(unsigned imgID = 0 ; imgID < imagesNumber ; ++imgID){
		for(unsigned i = 0 ; i < width ; ++i){
			for(unsigned j = 0 ; j < heigth ; ++j){
				unsigned crossDepth = (imgID * depth) / imagesNumber;
				if(plane == GRID_PLANE_CORONAL)
				{
					if( grid->getVoxel(i, j, crossDepth) ){
						tgaImageGrey.setPixel(i, j, 255);
					}else{
						tgaImageGrey.setPixel(i, j, 0);
					}
				}else
				if(plane == GRID_PLANE_TRANSVERSE)
				{
					if( grid->getVoxel(crossDepth, j, i) ){
						tgaImageGrey.setPixel(i, j, 255);
					}else{
						tgaImageGrey.setPixel(i, j, 0);
					}
				}else
				if(plane == GRID_PLANE_SAGITTAL)
				{
					if( grid->getVoxel(i,crossDepth, j) ){
						tgaImageGrey.setPixel(i, j, 255);
					}else{
						tgaImageGrey.setPixel(i, j, 0);
					}
				}
			}
		}
		std::stringstream bufor;
		std::string append = "";
		if(imgID < 100) append = "0";
		if(imgID < 10) append = "00";
		bufor << catalogPath << imageBaseName << append << imgID << ".tga";

		FilePath filePath(bufor.str());
		TargaImageFileWriter imgWriter(filePath);
		imgWriter.writeTargaImage(&tgaImageGrey);
	}

	return true;
}


