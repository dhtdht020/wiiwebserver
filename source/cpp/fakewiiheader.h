//a bunch of wild guesses for the prototypes of the functions to get to the linker when building on windows

struct GXRModeObj;

int VIDEO_Init();
int	PAD_Init();
int	WPAD_Init();

GXRModeObj *VIDEO_GetPreferredMode(int);
void *SYS_AllocateFramebuffer(GXRModeObj *);

#define MEM_K0_TO_K1
#define VI_DISPLAY_PIX_SZ 1
#define FALSE 0

int	VIDEO_Configure(GXRModeObj *);
int	VIDEO_SetNextFramebuffer(void *);
int	VIDEO_SetBlack(int);
int	VIDEO_Flush();

int fatInitDefault();