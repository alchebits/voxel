#ifndef FILE_H_
#define FILE_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include "wavefront.h"
#include "image.h"
#include "util.h"
#include "voxel.h"
typedef char byte;


class Path{
protected:
	std::string path;
public:
	explicit Path(const std::string& path);
	virtual ~Path();
	const std::string& getPath() const;
	void setPath(const std::string& path);
};


class FilePath : public Path{
protected:
	std::string fileName;
	std::string fileExtension;
public:
	explicit FilePath(const std::string& path);
	virtual ~FilePath();
	const std::string& getFileName();
	const std::string& getFileExtension();
	bool isFileExists();
};


class CatalogPath : public Path{
protected:
	std::string catalogName;
public:
	explicit CatalogPath(const std::string& path);
	virtual ~CatalogPath();
	const std::string& getCatalogName();
};


struct Bytes{
	byte* bytesTab;
	long tabSize;
};


class FileReader {
protected:
	FilePath filePath;
public:
	explicit FileReader(const FilePath& filePath);
	virtual ~FileReader();
};


class BinaryFileReader : public FileReader {
protected:
public:
	explicit BinaryFileReader(const FilePath& filePath);
	virtual ~BinaryFileReader();
	struct Bytes* getFileBytes_NEW();
};


class TextFileReader : public FileReader{
protected:
public:
	explicit TextFileReader(const FilePath& filePath);
	virtual ~TextFileReader();
	std::string& getFileText();
};

class WavefrontOBJFileReader : public TextFileReader{
protected:
public:
	explicit WavefrontOBJFileReader(const FilePath& filePath);
	virtual ~WavefrontOBJFileReader();
	WavefrontOBJ* readWavefrontOBJ_NEW();
	WavefrontOBJ* readWavefrontOBJ_NEW(const std::string& path);
	WavefrontOBJ_Data* readWavefrontOBJ_Data();
	WavefrontOBJ_Data* readWavefrontOBJ_Data(const std::string& path);
};


class TargaImageFileReader : public BinaryFileReader{
protected:
public:
	explicit TargaImageFileReader(const FilePath& filePath);
	virtual ~TargaImageFileReader();
	TargaImage* readImage_NEW(); // z podanej sciezki wczytuje obraz i zwraca w formacie
};


class FileWriter{
protected:
	FilePath filePath;
public:
	explicit FileWriter(const FilePath& filePath);
	virtual ~FileWriter();
};


class BinaryFileWriter : public FileWriter{
protected:
public:
	explicit BinaryFileWriter(const FilePath& filePath);
	virtual ~BinaryFileWriter();
};


class TextFileWriter : public FileWriter{
protected:
public:
	explicit TextFileWriter(const FilePath& filePath);
	virtual ~TextFileWriter();
};


class WavefrontOBJFileWriter : public TextFileWriter{
protected:
public:
	explicit WavefrontOBJFileWriter(const FilePath& filePath);
	virtual ~WavefrontOBJFileWriter();
};


class TargaImageFileWriter : public BinaryFileWriter {
protected:
public:
	explicit TargaImageFileWriter(const FilePath& filePath);
	virtual ~TargaImageFileWriter();
	void writeTargaImage(const TargaImage* tgaImage);
	void writeTargaImage(const TargaImage* tgaImage, const std::string& path);
};


enum GRID_PLANE{
	GRID_PLANE_CORONAL = 0,
	GRID_PLANE_TRANSVERSE,
	GRID_PLANE_SAGITTAL
};

class GridWriter{
protected:
	const Grid* grid;
	std::string catalogPath;
	std::string imageBaseName;
	unsigned imagesNumber;
public:
	GridWriter(const Grid* grid, unsigned imagesNumber, const std::string& catalogPath, const std::string& imageBaseName);
	virtual ~GridWriter();
	virtual bool writeImages(GRID_PLANE plane);
};

#endif
