#pragma once

#include <wiisprite.h>
using namespace wsp;

class Canvas : public Image {
public:

	class pixel {
	public:
		u8 r,g,b,a;
		void operator!(void);
	};

	class position {
	public:
		position(unsigned int x,unsigned int y);
		unsigned int x,y;
		
	};

	Canvas(void);
	~Canvas(void);

	Canvas::pixel &operator() (unsigned int x, unsigned int y);

	//extra fast line drawing operators
	void hline(unsigned int x1,unsigned int x2,unsigned int y,const pixel &p);
	void vline(unsigned int x,unsigned int y1,unsigned int y2,const pixel &p);
	void filledRect(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, const pixel &p);
	void floodFill(unsigned int x, unsigned int y, const pixel &p);

	void operator!(void);

	void flush(void);
};
