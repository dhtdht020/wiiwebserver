#include "Canvas.h"

#define XLOOP for(unsigned int xi=x1;xi<x2;xi++)
#define YLOOP for(unsigned int yi=y1;yi<y2;yi++)

void Canvas::hline(const unsigned int x1, const unsigned int x2, const unsigned int y, const rgbaImage::pixel &p) {
	XLOOP {
		operator()(xi,y)=p;
	}
};

void Canvas::vline(const unsigned int x, const unsigned int y1, const unsigned int y2, const rgbaImage::pixel &p) {
	YLOOP {
		operator()(x,yi)=p;
	}
};

void Canvas::filledRect(const unsigned int x1, const unsigned int x2, const unsigned int y1, const unsigned int y2, const rgbaImage::pixel &p) {
	YLOOP {
		XLOOP {
			operator()(xi,yi)=p;
		}
	}
};

void Canvas::operator ~(void) {
	for(unsigned int iy=GetHeight();iy>0;iy--) {
		for(unsigned int ix=GetWidth();ix>0;ix--) {
			~operator()(ix-1,iy-1);
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
