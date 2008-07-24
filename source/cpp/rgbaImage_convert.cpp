#include "rgbaImage.h"

void rgbaImage::_convertPixels() {

	const unsigned int h=GetHeight(),w=GetWidth();
	//const unsigned int halftilesize=tilesize/2;

	//code shamelessly stolen from the original library

	u8 *d=_pixels;

	for (unsigned int iy = 0; iy < h; iy += tilesize) {
		for (unsigned int ix = 0; ix < w; ix += tilesize) {
			for(unsigned int iy2 = 0;iy2 <tilesize;iy2++) {
				for(unsigned int ix2 = 0;ix2 <tilesize;ix2++) {
					pixel &p=_RGBApixels[(iy+iy2)*w+(ix+ix2)];
					*d++=p.a;
					*d++=p.r;
				}
			}
			for(unsigned int iy2 = 0;iy2 <tilesize;iy2++) {
				for(unsigned int ix2 = 0;ix2 <tilesize;ix2++) {
					pixel &p=_RGBApixels[(iy+iy2)*w+(ix+ix2)];
					*d++=p.g;
					*d++=p.b;
				}
			}
		}
	}
}
