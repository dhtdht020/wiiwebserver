#pragma once

#include <wiisprite.h>
using namespace wsp;

#include <set>
using std::set;

class Canvas : public Image {
public:

	class pixel {
	public:
		u8 r,g,b,a;
		void operator!(void);
		bool operator!=(const pixel&);
		bool operator==(const pixel&);

		pixel(void);
		pixel(const unsigned int r, const unsigned int g, const unsigned int b);
		pixel(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a);
	};

	class position {
	public:
		position(unsigned int x,unsigned int y);
		unsigned int x,y;
		
		bool operator <(const position &p2) const;
	};

	Canvas(const unsigned int width, const unsigned int height);
	~Canvas(void);

	Canvas::pixel &operator() (const unsigned int x, const unsigned int y);

	//extra fast line drawing operators
	void hline(const unsigned int x1,const unsigned int x2,const unsigned int y,const pixel &p);
	void vline(const unsigned int x,const unsigned int y1, const unsigned int y2,const pixel &p);
	//non constnes is not accidental
	void simple45line(unsigned int x,unsigned int y, const unsigned int lenght,bool up);
	void filledRect(const unsigned int x1, const unsigned int x2, const unsigned int y1, const unsigned int y2, const pixel &p);
	void floodFill(const unsigned int x, const unsigned int y, const pixel &p);
	void clear(const pixel &p);

	void operator!(void);

	void flush(void);


protected:
	pixel *_RGBApixels;
private:
	void _convertPixels();

	static const unsigned int tilesize=4;

	//used by flood fill
	void tryToAdd(set<position> &open,bool *closed,const pixel &p,signed int xOffset,signed int yOffset,const position &pos);
};
