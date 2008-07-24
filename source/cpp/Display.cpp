extern "C" {
#include <ogcsys.h>
#include <gccore.h>

#include <wiiuse/wpad.h>
}

#include "globals.h"

#include <wiisprite.h>
#include "Canvas.h"

using namespace wsp;

void *Display(void*) {

	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

	GameWindow gwd;
	LayerManager manager(1);//remeber, it's the number of items in it

	gwd.InitVideo();

	Canvas t(60,100);
	t.clear(Canvas::pixel(0,255,255));
	t.vline(0 ,0 ,99,Canvas::pixel(0,255,0));
	t.vline(59,0 ,99,Canvas::pixel(0,255,0));
	t.hline(1 ,58,49,Canvas::pixel(0,255,0));
	t.flush();

	Sprite t_sprite;
	t_sprite.SetImage(&t);
	t_sprite.SetStretchHeight(4);
	t_sprite.SetStretchWidth(4);

	manager.Insert(&t_sprite,0);

	bool runGUI=true;
	
	//TODO: add input and display proccessing loop
	while(runGUI) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsHeld(WPAD_CHAN_0);
		ir_t ir;

		WPAD_IR(WPAD_CHAN_0, &ir);

		if(pressed & WPAD_BUTTON_HOME) {
			runGUI=false;
		}

		t_sprite.SetPosition(ir.sx-WSP_POINTER_CORRECTION_X, ir.sy-WSP_POINTER_CORRECTION_Y); // We use these constants to translate the position correctly to the screen
		t_sprite.Move(-((f32)t_sprite.GetWidth()/2), -((f32)t_sprite.GetHeight()/2)); // And these to make our image appear at the center of this position.
		t_sprite.SetRotation(ir.angle/2); 

		manager.Draw(0, 0); 

		gwd.Flush();

	}

	gwd.StopVideo();

	return (void*)0;
}

void startDisplayThread() {
	LWP_CreateThread(&displayThread,Display,NULL,NULL,NULL,1);
}
