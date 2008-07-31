#include "Canvas.h"
#include <algorithm>
using std::swap;

#define XLOOP for(unsigned int xi=x1;xi<x2;xi++)
#define YLOOP for(unsigned int yi=y1;yi<y2;yi++)

void Canvas::hline(const unsigned int x1, const unsigned int x2, const unsigned int y, const rgbaImage::pixel &p) {
	for(unsigned int xi=x1;xi<x2;xi++) {
		getpixel(xi,y)=p;
	}
};

void Canvas::vline(const unsigned int x, const unsigned int y1, const unsigned int y2, const rgbaImage::pixel &p) {
	for(unsigned int yi=y1;yi<y2;yi++) {
		getpixel(x,yi)=p;
	}
};

void Canvas::filledRect(const unsigned int x1, const unsigned int x2, const unsigned int y1, const unsigned int y2, const rgbaImage::pixel &p) {
	YLOOP {
		XLOOP {
			getpixel(xi,yi)=p;
		}
	}
};

void Canvas::operator ~(void) {
	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			~getpixel(ix,iy);
		}
	}
};

void Canvas::clear(const pixel &p) {
	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			getpixel(ix,iy)=p;
		}
	}
};

void Canvas::BresenhamLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, const pixel &p) {
	//code shamelesly stolen from wikipedia and c++ified
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax + 1) / 2;
	int ystep;
	int y = y0;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for(unsigned int x= x0;x<=x1;x++) {
		if(steep) {
			getpixel(y,x)=p;
		} else {
			getpixel(x,y)=p;
		}
		error = error + deltay;
		if (error >= 0) {
			y = y + ystep;
			error = error - deltax;
		}
	}

}
