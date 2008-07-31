#include "Canvas.h"
#include <set>
using std::set;

void Canvas::tryToAdd(set<position> &open,bool *closed,const pixel &p,signed int xOffset,signed int yOffset,const position &pos) {

	//sanity checks to stay in the buffer
	if(pos.y==0 && yOffset<0) return;
	if(pos.x==0 && xOffset<0) return;
	if(pos.x+1==GetWidth() && xOffset>0) return;
	if(pos.y+1==GetHeight() && yOffset>0) return;

	//we definitly shouldn't add stuff we checked already
	if(closed[(pos.x+xOffset)+(pos.y+yOffset)*GetWidth()]) return;

	pixel p2=getpixel(pos.x+xOffset,pos.y+yOffset);

	if(p2!=p) return;

	open.insert(position(pos.x+xOffset,pos.y+yOffset));

};

void Canvas::floodFill(unsigned int x, unsigned int y, const pixel &p) {

	set<position> open;
	open.insert(position(x,y));

	pixel basepixel=getpixel(x,y);

	bool *closed=new bool[GetHeight()*GetWidth()];

	for(unsigned int iy=0;GetHeight()>iy;iy++) {
		for(unsigned int ix=0;GetWidth()>ix;ix++) {
			closed[iy*GetWidth()+ix]=false;
		}
	}

	while(!open.empty()) {
		const set<position>::const_iterator itr=open.begin();
		const position& pos=*itr;

		closed[pos.x+pos.y*GetWidth()]=true;

		tryToAdd(open,closed,basepixel,0,-1,pos);
		tryToAdd(open,closed,basepixel,0,1,pos);
		tryToAdd(open,closed,basepixel,-1,0,pos);
		tryToAdd(open,closed,basepixel,1,0,pos);

		getpixel(pos.x,pos.y)=p;

		open.erase(itr);
	}
	delete[] closed;
}


