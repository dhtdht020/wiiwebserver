#include "Canvas.h"

#define XLOOP for(unsigned int xi=x1;xi<x2;xi++)
#define YLOOP for(unsigned int yi=y1;yi<y2;yi++)

Canvas::Canvas(const unsigned int width, const unsigned int height) {
	_InitializeImage(width,height);
	_RGBApixels=new pixel[width*height];
}

Canvas::~Canvas(void) {
	delete[] _RGBApixels;
}

inline Canvas::pixel &Canvas::operator ()(const unsigned int x, const unsigned int y) {
	return _RGBApixels[x+y*GetWidth()];
}

void Canvas::hline(const unsigned int x1, const unsigned int x2, const unsigned int y, const Canvas::pixel &p) {
	XLOOP {
		operator()(xi,y)=p;
	}
}

void Canvas::vline(const unsigned int x, const unsigned int y1, const unsigned int y2, const Canvas::pixel &p) {
	YLOOP {
		operator()(x,yi)=p;
	}
}

void Canvas::filledRect(const unsigned int x1, const unsigned int x2, const unsigned int y1, const unsigned int y2, const Canvas::pixel &p) {
	YLOOP {
		XLOOP {
			operator()(xi,yi)=p;
		}
	}
};

void Canvas::operator !(void) {
	for(unsigned int iy=GetHeight();iy>0;iy--) {
		for(unsigned int ix=GetWidth();ix>0;ix--) {
			!operator()(ix-1,iy-1);
		}
	}
};

void Canvas::clear(const pixel &p) {
	for(unsigned int iy=GetHeight();iy>0;iy--) {
		for(unsigned int ix=GetWidth();ix>0;ix--) {
			operator()(ix-1,iy-1)=p;
		}
	}
};

void Canvas::flush(void) { 
	_convertPixels();
	_Flush();
};

void Canvas::pixel::operator !() {
	r^=256;
	g^=256;
	b^=256;
};

bool Canvas::pixel::operator !=(const pixel &p2) {
	return r!=p2.r || g!=p2.g || b!=p2.b;
};

bool Canvas::pixel::operator ==(const pixel &p2) {
	return r==p2.r && g==p2.g && b==p2.b;
};

Canvas::position::position(unsigned int x_p,unsigned int y_p) : x(x_p), y(y_p) {};

bool Canvas::position::operator <(const Canvas::position &p2) const {
	if(y==p2.y) {
		return x<p2.x;
	} else {
		return y<p2.y;
	}
};

Canvas::pixel::pixel() : r(255), g(0), b(255), a(0) {};//fricking pink to notice undefined data

Canvas::pixel::pixel(const unsigned int r_p, const unsigned int g_p, const unsigned int b_p) :
	r(r_p), g(g_p), b(b_p), a(255) {};

	Canvas::pixel::pixel(const unsigned int r_p, const unsigned int g_p, const unsigned int b_p, const unsigned int a_p) :
	r(r_p), g(g_p), b(b_p), a(a_p) {};

