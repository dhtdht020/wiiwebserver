#pragma once

#include "rgbaImage.h"

#include <set>
using std::set;

class Canvas : public rgbaImage {
public:



	class position {
	public:
		position(unsigned int x,unsigned int y);
		unsigned int x,y;
		
		bool operator <(const position &p2) const;
	};

	Canvas(const unsigned int width, const unsigned int height);
	~Canvas(void);

	//extra fast line drawing operators
	void hline(const unsigned int x1,const unsigned int x2,const unsigned int y,const pixel &p);
	void vline(const unsigned int x,const unsigned int y1, const unsigned int y2,const pixel &p);
	//non constnes is not accidental
	void simple45line(unsigned int x,unsigned int y, const unsigned int lenght,bool up);
	void BresenhamLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const pixel &p);
	void filledRect(const unsigned int x1, const unsigned int x2, const unsigned int y1, const unsigned int y2, const pixel &p);
	void floodFill(const unsigned int x, const unsigned int y, const pixel &p);
	void clear(const pixel &p);

	void operator~(void);

	void flush(void);


private:

	//used by flood fill
	void tryToAdd(set<position> &open,bool *closed,const pixel &p,signed int xOffset,signed int yOffset,const position &pos);
};
