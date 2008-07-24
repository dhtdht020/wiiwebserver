#include "Canvas.h"

Canvas::Canvas(const unsigned int width, const unsigned int height) : rgbaImage(width,height) {
}

Canvas::~Canvas(void) {
	delete[] _RGBApixels;
}

void Canvas::flush(void) {
	_Flush();
};

Canvas::position::position(unsigned int x_p,unsigned int y_p) : x(x_p), y(y_p) {};

bool Canvas::position::operator <(const Canvas::position &p2) const {
	if(y==p2.y) {
		return x<p2.x;
	} else {
		return y<p2.y;
	}
};

