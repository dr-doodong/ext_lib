#ifndef __Z64GEOGUI_H__
#define __Z64GEOGUI_H__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nanovg/src/nanovg.h>
#include <ExtLib.h>
#include <ExtGui/Vector.h>
#include <ExtGui/Input.h>

extern char* gBuild;
extern f32 gPixelRatio;

#define SPLIT_GRAB_DIST  4
#define SPLIT_CTXM_DIST  32
#define SPLIT_BAR_HEIGHT 28
#define SPLIT_SPLIT_W    2.0
#define SPLIT_ROUND_R    2.0
#define SPLIT_CLAMP      ((SPLIT_BAR_HEIGHT + SPLIT_SPLIT_W * 1.25) * 2)

#define SPLIT_TEXT_PADDING 4
#define SPLIT_TEXT         12

#define SPLIT_TEXT_H         (SPLIT_TEXT_PADDING * 2 + SPLIT_TEXT)
#define SPLIT_ELEM_X_PADDING 6
#define SPLIT_ELEM_Y_PADDING (SPLIT_TEXT_H + SPLIT_ELEM_X_PADDING)

struct GeoGridContext;
struct Split;
typedef void (* SplitFunc)(
	void*,
	void*,
	struct Split*
);

typedef enum {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
} TextAlign;

typedef enum {
	BAR_TOP,
	BAR_BOT
} GuiBarIndex;

typedef enum {
	VTX_BOT_L = 0,
		VTX_TOP_L,
		VTX_TOP_R,
		VTX_BOT_R,
		EDGE_L = 0,
		EDGE_T,
		EDGE_R,
	EDGE_B
} SplitPos;

typedef enum {
	DIR_L = 0,
		DIR_T,
		DIR_R,
		DIR_B,
} SplitDir;

typedef enum {
	SPLIT_POINT_NONE = 0,
		SPLIT_POINT_BL = (1 << 0),
		SPLIT_POINT_TL = (1 << 1),
		SPLIT_POINT_TR = (1 << 2),
		SPLIT_POINT_BR = (1 << 3),
	
		SPLIT_POINTS   = (SPLIT_POINT_TL | SPLIT_POINT_TR | SPLIT_POINT_BL | SPLIT_POINT_BR),
	
		SPLIT_SIDE_L   = (1 << 4),
		SPLIT_SIDE_T   = (1 << 5),
		SPLIT_SIDE_R   = (1 << 6),
		SPLIT_SIDE_B   = (1 << 7),
	
		SPLIT_SIDE_H   = (SPLIT_SIDE_L | SPLIT_SIDE_R),
		SPLIT_SIDE_V   = (SPLIT_SIDE_T | SPLIT_SIDE_B),
		SPLIT_SIDES    = (SPLIT_SIDE_H | SPLIT_SIDE_V),
} SplitState;

typedef enum {
	EDGE_STATE_NONE = 0,
		EDGE_HORIZONTAL = (1 << 0),
		EDGE_VERTICAL   = (1 << 1),
	
		EDGE_ALIGN      = (EDGE_HORIZONTAL | EDGE_VERTICAL),
	
		EDGE_STICK_L    = (1 << 2),
		EDGE_STICK_T    = (1 << 3),
		EDGE_STICK_R    = (1 << 4),
		EDGE_STICK_B    = (1 << 5),
	
		EDGE_STICK      = (EDGE_STICK_L | EDGE_STICK_T | EDGE_STICK_R | EDGE_STICK_B),
	
		EDGE_EDIT       = (1 << 6),
} EdgeState;

typedef struct SplitVtx {
	struct SplitVtx* prev;
	struct SplitVtx* next;
	Vec2d pos;
	u8    killFlag;
} SplitVtx;

typedef struct SplitEdge {
	struct SplitEdge* prev;
	struct SplitEdge* next;
	SplitVtx* vtx[2];
	f64 pos;
	EdgeState state;
	u8 killFlag;
} SplitEdge;

typedef struct Split {
	struct Split* prev;
	struct Split* next;
	SplitState    stateFlag;
	SplitEdge*    edge[4];
	SplitVtx* vtx[4];
	Rect  rect; // Absolute XY, relative WH
	Rect  cect;
	Vec2s center;
	Vec2s mousePos; // relative
	Vec2s mousePressPos;
	bool  mouseInSplit;
	bool  mouseInHeader;
	u32   id;
	u32   prevId;
	bool  blockMouse;
	void* instance;
	struct {
		bool useCustomBG;
		RGB8 color;
	} bg;
	struct {
		char data[160];
	} header;
} Split;

typedef struct {
	Rect rect;
} StatusBar;

typedef struct {
	Vec2s  pos;
	Split* split;
	char** optionList;
	s32    num;
	s32    hoverNum;
	Rect   hoverRect;
} GeoCtxMenu;

typedef struct {
	char* taskName;
	SplitFunc init;
	SplitFunc destroy;
	SplitFunc update;
	SplitFunc draw;
	s32 size;
} SplitTask;

typedef struct GeoGridContext {
	GeoCtxMenu ctxMenu;
	Split*     actionSplit;
	Split*     splitHead;
	SplitVtx*  vtxHead;
	SplitEdge* edgeHead;
	StatusBar  bar[2];
	Rect prevWorkRect;
	Rect workRect;
	SplitEdge* actionEdge;
	struct {
		f64 clampMax;
		f64 clampMin;
	} slide;
	InputContext* input;
	Vec2s* winDim;
	void*  vg;
	void*  passArg;
	SplitTask* taskTable;
	s32    taskTableNum;
#ifndef NDEBUG
	bool   jsonSettings;
#endif
} GeoGridContext;

bool GeoGrid_Cursor_InRect(Split* split, Rect* rect);
void GeoGrid_Layout_SaveJson(GeoGridContext* geoCtx);
Vec2s GeoGrid_Layout_LoadJson(GeoGridContext* geoCtx, Vec2s* winDim);

void GeoGrid_Init(GeoGridContext* geoCtx, Vec2s* winDim, InputContext* input, void* vg);
void GeoGrid_Update(GeoGridContext* geoCtx);
void GeoGrid_Draw(GeoGridContext* geoCtx);

void Element_Init(GeoGridContext* geoCtx);
void Element_Update(GeoGridContext* geoCtx);
void Element_Draw(GeoGridContext* geoCtx, Split* split);
void Element_PostDraw(GeoGridContext* geoCtx, Split* split);

typedef struct ElButton {
	char*    txt;
	u8       state;
	u8       hover;
	u8       toggle;
	u8       autoWidth;
	Rect     rect;
	NVGcolor colorOL;
	NVGcolor colorIL;
} ElButton;

typedef struct {
	char*    txt;
	s32      size;
	Rect     rect;
	NVGcolor bgCl;
	u8 hover        : 1;
	u8 isNumBox     : 1;
	u8 isHintText   : 2;
	TextAlign align : 3;
	struct {
		u8  isInt : 1;
		u8  updt  : 1;
		f32 value;
		f32 min;
		f32 max;
	} nbx;
} ElTextbox;

typedef struct {
	char* txt;
	Rect  rect;
} ElText;

typedef struct {
	Rect rect;
	u8   toggle;
	NVGcolor color;
	NVGcolor colorO;
	f32 lerp;
	u8  hover;
} ElCheckbox;

typedef struct {
	char txt[32];
	Rect rect;
	
	f32  vValue; /* Visual Value */
	f32  pValue; /* Previous Value */
	f32  value;
	f32  min;
	f32  max;
	
	u8   isSliding : 1;
	u8   isInt     : 1;
	u8   hover     : 1;
	u8   holdState : 1;
	NVGcolor color;
	NVGcolor hovColor;
	
	s32 isTextbox;
	ElTextbox textBox;
} ElSlider;

s32 Element_Button(GeoGridContext*, Split*, ElButton*);
void Element_Textbox(GeoGridContext*, Split*, ElTextbox*);
f32 Element_Text(GeoGridContext* geoCtx, Split* split, ElText* txt);
s32 Element_Checkbox(GeoGridContext* geoCtx, Split* split, ElCheckbox* this);
void Element_Slider_SetValue(ElSlider* this, f32 val);
f32 Element_Slider(GeoGridContext* geoCtx, Split* split, ElSlider* this);

void Element_PushToPost();
void Element_SetRect(Rect* rect, f32 x, f32 y, f32 w);
void Element_SetRect_Two(Split* split, Rect* rectA, f32 separate, Rect* rectB, f32 y);

#endif