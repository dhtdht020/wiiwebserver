#include "rgbaImage.h"

rgbaImage::rgbaImage(const unsigned int width, const unsigned int height) {
	_InitializeImage(width,height);
	_RGBApixels=new pixel[width*height];
};

rgbaImage::~rgbaImage(void) {
};

rgbaImage::pixel &rgbaImage::getpixel(const unsigned int x, const unsigned int y) {
	return _RGBApixels[x+y*GetWidth()];
};

void rgbaImage::pixel::operator ~() {
	r^=256;
	g^=256;
	b^=256;
};

bool rgbaImage::pixel::operator !=(const pixel &p2) {
	return r!=p2.r || g!=p2.g || b!=p2.b;
};

bool rgbaImage::pixel::operator ==(const pixel &p2) {
	return r==p2.r && g==p2.g && b==p2.b;
};

rgbaImage::pixel::pixel() : r(255), g(0), b(255), a(0) {};//fricking pink to notice undefined data

rgbaImage::pixel::pixel(const unsigned int r_p, const unsigned int g_p, const unsigned int b_p) :
	r(r_p), g(g_p), b(b_p), a(255) {
};

rgbaImage::pixel::pixel(const unsigned int r_p, const unsigned int g_p, const unsigned int b_p, const unsigned int a_p) :
	r(r_p), g(g_p), b(b_p), a(a_p) {
};

void rgbaImage::_Flush() {
	_convertPixels();
	Image::_Flush();
};
