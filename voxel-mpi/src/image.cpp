#include "../include/image.h"

Image::Image(unsigned int width, unsigned int height) : width(width), heigth(height){
}
Image::~Image(){
}
unsigned int Image::getWidth() const{
	return width;
}
unsigned int Image::getHeight() const{
	return heigth;
}



TargaImage::TargaImage(unsigned int width, unsigned int height, unsigned depth) : Image(width, height), pixels(NULL){
	this->depth = depth;
	pixels = new ubyte[width * height * depth];

	header.identSize = 0;
	header.colorMapType = 0;
	//header.imageType = TGA_IMAGE_TYPE_RGB; // set in inheritanced

	header.colorMapStart = 0;
	header.colorMapLength = 0;
	header.colorMapBits = 0;

	header.xStart = 0;
	header.yStart = 0;
	header.width = width;
	header.height = heigth;
	header.bitsPerPixel = depth*8;
	header.descriptor = 0;
}
TargaImage::~TargaImage(){
	DELETE_CPP_TAB(pixels);
}
const struct TargaHeader* TargaImage::getHeaderPTR() const{
	return &header;
}
const ubyte* TargaImage::getPixelsPTR() const{
	return pixels;
}
long TargaImage::sizeInBytes() const{
	return width * heigth * depth;
}
unsigned int TargaImage::getDepth() const{
	return depth;
}



TargaImageRGB::TargaImageRGB(unsigned int width, unsigned int height) : TargaImage(width, height, 3){
	header.imageType = TGA_IMAGE_TYPE_RGB;
}
TargaImageRGB::~TargaImageRGB(){
}

void TargaImageRGB::setPixel(unsigned int x, unsigned int y, ubyte red, ubyte green, ubyte blue){
	int blueID = (((heigth-1-y)*heigth + x)* depth);
	pixels[blueID] = blue;
	pixels[blueID+1] = green;
	pixels[blueID+2] = red;
}



TargaImageGrey::TargaImageGrey(unsigned int width, unsigned int heigth) : TargaImage(width, heigth, 1) {
	header.imageType = TGA_IMAGE_TYPE_GREY;
}
TargaImageGrey::~TargaImageGrey(){
}
void TargaImageGrey::setPixel(unsigned int x, unsigned int y, ubyte grey){
	int pixID = (((heigth-1-y)*heigth + x));
	pixels[pixID] = grey;
}
