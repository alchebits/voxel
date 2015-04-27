#ifndef IMAGE_H_
#define IMAGE_H_

#define DELETE_CPP_TAB(ptr) delete[] ptr; ptr = NULL;

#include<iostream>

typedef unsigned char ubyte;
typedef short shortt;

/* 18 bajtow*/
struct TargaHeader{
	ubyte  identSize;          // size of ID field that follows 18 byte header (0 usually)
	ubyte  colorMapType;      // type of colour map 0=none, 1=has palette
	ubyte  imageType;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

	shortt colorMapStart;     // first colour map entry in palette
	shortt colorMapLength;    // number of colours in palette
	ubyte  colorMapBits;      // number of bits per palette entry 15,16,24,32

	shortt xStart;             // image x origin
	shortt yStart;             // image y origin
	shortt width;              // image width in pixels
	shortt height;             // image height in pixels
	ubyte  bitsPerPixel;       // image bits per pixel 8,16,24,32
	ubyte  descriptor;         // image descriptor bits (vh flip bits)
};

/*Uproszczona klasa obslugujaca obrazy TGA (24 bitow na pixel rgb) (do rozwiniecia w razie potrzeby)*/
/*Poprawnie to po tej klasie powinno dziedziczyc wiele innych odp. roznym rodzajom pliku tga*/

class ColorRGB{
public:
	ubyte red, green, blue;
	// TODO dopisac konstruktor, ewentualnie obmyslec hierarchie dla klas
};

class Image{
protected:
	unsigned int width;
	unsigned int heigth;
public:
	Image(unsigned int width, unsigned int height);
	virtual ~Image();
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	virtual long sizeInBytes() const = 0;
};

class TargaImage : public Image{
protected:
	static const ubyte TGA_IMAGE_TYPE_NONE = 0;
	static const ubyte TGA_IMAGE_TYPE_INDEXED = 1;
	static const ubyte TGA_IMAGE_TYPE_RGB = 2;
	static const ubyte TGA_IMAGE_TYPE_GREY = 3;
	static const ubyte TGA_IMAGE_TYPE_RLE = 8;

	struct TargaHeader header;
	ubyte* pixels; // tablica ze wszystkimi danymi pikselowymi bajt po bajcie
	unsigned depth;
public:
	TargaImage(unsigned int width, unsigned int heigth, unsigned depth);
	virtual ~TargaImage();
	virtual long sizeInBytes() const ;
	virtual unsigned int getDepth() const;
	const struct TargaHeader* getHeaderPTR() const;
	const ubyte* getPixelsPTR() const;
protected:
};

class TargaImageRGB : public TargaImage{
protected:
public:
	TargaImageRGB(unsigned int width, unsigned int heigth);
	virtual ~TargaImageRGB();
	void setPixel(unsigned int x, unsigned int y, ubyte red, ubyte green, ubyte blue);
};

/*Na koniec jak starczy czasu  testowac:*/
class TargaImageGrey : public TargaImage{
protected:
public:
	TargaImageGrey(unsigned int width, unsigned int heigth);
	virtual ~TargaImageGrey();
	void setPixel(unsigned int x, unsigned int y, ubyte grey);

};

#endif

