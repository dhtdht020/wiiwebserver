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
	if(closed[pos.x+pos.y*GetWidth()]) return;

	pixel p2=operator()(pos.x+xOffset,pos.y+yOffset);

	if(p2!=p) return;

	open.insert(position(pos.x+xOffset,pos.y+yOffset));

};

void Canvas::floodFill(unsigned int x, unsigned int y, const pixel &p) {

	set<position> open;
	open.insert(position(x,y));

	pixel basepixel=operator()(x,y);

	bool *closed=new bool[GetHeight()*GetWidth()];

	for(;!open.empty();) {
		const position& pos=*(open.begin());

		closed[pos.x+pos.y*GetWidth()]=true;

		tryToAdd(open,closed,p,0,-1,pos);
		tryToAdd(open,closed,p,0,1,pos);
		tryToAdd(open,closed,p,-1,0,pos);
		tryToAdd(open,closed,p,1,0,pos);

		operator()(pos.x,pos.y)=p;

		open.erase(pos);
	}
	delete[] closed;
}


