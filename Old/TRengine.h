#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define WINDOW_CLASS_NAME "GLClass"

typedef enum {false, true} bool;

typedef struct vector2d {
    uint32_t X;
    uint32_t Y;
} vector2d;

typedef struct PixelStruct {
    union {
        uint32_t n;
        struct {
            uint8_t r; uint8_t g; uint8_t b; uint8_t a;
        };
    };
} PixelType;

typedef struct TextureStruct {
	uint32_t width;
	uint32_t height;
	GLuint id;
	PixelType textureData[];  
} TextureType;

typedef struct ButtonInput {
	bool pressed;
	bool released;
	bool held;
} ButtonInputType;

typedef enum KeyMapping {
	key_0 = 0x30, key_1 = 0x31, key_2 = 0x32, key_3 = 0x33, key_4 = 0x34, key_5 = 0x35, key_6 = 0x36, key_7 = 0x37, key_8 = 0x38, key_09 = 0x39, 
	key_A = 0x41, key_B = 0x42, key_C = 0x43, key_D = 0x44, key_E = 0x45, key_F = 0x46, key_G = 0x47, key_H = 0x48, key_I = 0x49, key_J = 0x4A,
	key_K = 0x4B, key_L = 0x4C, key_M = 0x4D, key_N = 0x4E, key_O = 0x4F, key_P = 0x50, key_Q = 0x51, key_R = 0x52, key_S = 0x53, key_T = 0x54,
	key_U = 0x55, key_V = 0x56, key_W = 0x57, key_X = 0x58, key_Y = 0x59, key_Z = 0x5A, key_Arrow_Left = 0x25, key_Arrow_Up = 0x26, key_Arrow_Right = 0x27,
	key_Arrow_Down = 0x28, key_Esc = 0x1B, key_Ctrl = 0x11, key_Shift = 0x10, key_Enter = 0x0D, key_Backspace = 0x08, key_Spacebar = 0x20, key_Delete = 0x2E
} KeyMapping;

HDC glDeviceContext;
HGLRC glRenderContext;
HWND windowHandle;
HANDLE engineThread;
vector2d vScreenSize;
vector2d vViewSize;
vector2d vWindowSize;
vector2d vWindowPosition;
vector2d vViewPosition;
vector2d vMousePosition;
bool bKeyboardNewState[100];
bool bKeyboardOldState[100];
bool bMouseOldState[3];
bool bMouseNewState[3];
bool bApplicationIsRunning = false;
bool bFullscreenIsEnabled = false;
bool bVsyncIsEnabled = false;
ButtonInputType biMouse[3];
ButtonInputType biKeyboard[100];
uint32_t iScale = 1;
GLuint iId = 0;
uint32_t iTextureNumber = 0;
uint32_t iFramesPerSecond = 0;
clock_t clTimeCounter1;
clock_t clTimeCounter2;
float fElapsedTime = 0.0f;
char cWindowTitle[40];
TextureType *tCurrentTarget = NULL;
TextureType *tLayers[32];

void TextureCreate(uint32_t width, uint32_t height, PixelType *imageData);
void TextureUpdate(TextureType *texture);
void TextureApply(uint32_t id); 
void TextureDelete(uint32_t id); // Should also delete from tLayers and push back others
bool TextureDraw(uint32_t x, uint32_t y, TextureType *texture);
bool TextureDrawPartial(uint32_t x, uint32_t y, uint32_t top, uint32_t left, uint32_t right, uint32_t bottom, TextureType *texture);
PixelType Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
bool PixelWrite(uint32_t x, uint32_t y, PixelType pixel, TextureType *texture);
bool PixelDraw(uint32_t x, uint32_t y, PixelType pixel);
PixelType PixelRead(uint32_t x, uint32_t y, TextureType *texture);
PixelType PixelCheck(uint32_t x, uint32_t y);
bool Construct(uint32_t screenWidth, uint32_t screenHeight, uint32_t scale, bool boolFullscreen, bool boolVsync);
bool Start();
DWORD WINAPI EngineThread();
void CoreUpdate();
bool OnUserInitialize();
bool OnUserUpdate();
void ClearBuffer(PixelType pixel,bool boolDepth);
bool CreateWindowPane(bool boolFullscreen, vector2d windowPosition, vector2d windowSize);
bool CreateDevice(HWND windowHandle, bool boolFullscreen, bool boolVsync);
bool CreateGraphics(bool boolFullscreen, bool boolVsync, vector2d viewPosition, vector2d viewSize);
LRESULT CALLBACK WindowEventHandler(HWND windowHandle, UINT uMessage, WPARAM wParameter, LPARAM lParameter);
void WindowUpdateSize(uint32_t x, uint32_t y);
void ViewportUpdate();
void Terminate();
void CurrentTargetSetI(uint32_t index);
void CurrentTargetSetP(TextureType *texture);
void IntToStr(uint32_t value, char destination[]);
bool LoadImageFromPath(char imagePath[]);
void MouseUpdatePosition(int32_t x, int32_t y);
void MouseUpdateState(bool state, int32_t button);
void KeySetState(bool state, int32_t key);


void TextureCreate(uint32_t width, uint32_t height, PixelType *imageData){
    TextureType *texture = malloc(sizeof(TextureType)+sizeof(PixelType)*width*height);

    glGenTextures(1, &iId);
    glBindTexture(GL_TEXTURE_2D, iId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    texture->id = iId;
    texture->height = height;
    texture->width = width;
    for (int i = 0; i < width*height; i++){
        texture->textureData[i] = imageData[i];
    };

    tLayers[iTextureNumber] = texture;
    iTextureNumber++;
};

void TextureUpdate(TextureType *texture){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture->textureData);
};

void TextureApply(uint32_t id){
    glBindTexture(GL_TEXTURE_2D, id);
};

void TextureDelete(uint32_t id){
    glDeleteTextures(1, &id);
};

bool TextureDraw(uint32_t x, uint32_t y, TextureType *texture){
    if (((x + texture->width) < vScreenSize.X) && ((y + texture->height) < vScreenSize.Y)){
        x = x*iScale;
        y = y*iScale;
        if (iScale > 1){
            for (uint32_t i = 0; i < texture->width; i++){
                for (uint32_t j = 0; j < texture->height; j++){
                    for (uint32_t i2 = 0; i2 < iScale; i2++){
                        for (uint32_t j2 = 0; j2 < iScale; j2++){
                            PixelWrite(x + (i*iScale) + i2, y + (j*iScale) + j2, PixelRead(i, j, texture), tLayers[0]);
                        };
                    };
                };
            };
        } else {
            for (uint32_t i = 0; i < texture->width; i++){
                for (uint32_t j = 0; j < texture->height; j++){
                    PixelWrite(x + i, y + j, PixelRead(i, j, texture), tLayers[0]);
                }
            };
        };
        return true;
    };
    return false;
};

bool TextureDrawPartial(uint32_t x, uint32_t y, uint32_t top, uint32_t left, uint32_t right, uint32_t bottom, TextureType *texture){
    if ((bottom < texture->height) && (right < texture->width)){
        x = x*iScale;
        y = y*iScale;
        if (iScale > 1){
                for (uint32_t i = left; i < right + 1; i++){
                    for (uint32_t j = top; j < bottom + 1; j++){
                        for (uint32_t i2 = 0; i2 < iScale; i2++){
                            for (uint32_t j2 = 0; j2 < iScale; j2++){
                                PixelWrite(x + ((i - left)*iScale) + i2, y + ((j - top)*iScale) + j2, PixelRead(i, j, texture), tLayers[0]);
                            };
                        };
                    };	
                };      
        } else {
            for (uint32_t i = left; i < right + 1; i++){
                for (uint32_t j = top; j < bottom + 1; j++){
                    PixelWrite(x + i - left, y + j - top, PixelRead(i, j, texture), tLayers[0]);
                };
            };
        };
        return true;
    };
    return false;
};

PixelType Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){
    PixelType pixel;
    PixelType *mem = &pixel;
	mem->n = red | (green << 8) | (blue << 16) | (alpha << 24);
    return pixel;   
};

bool PixelWrite(uint32_t x, uint32_t y, PixelType pixel, TextureType *texture){
    if ((x < texture->width) && (y < texture->height)){
        texture->textureData[(texture->width)*y + x] = pixel;
        return true;
    };
    return false;
};

bool PixelDraw(uint32_t x, uint32_t y, PixelType pixel){
    if ((x < tLayers[0]->width) && (y < tLayers[0]->height)){
        x = x*iScale;
        y = y*iScale;
        if (iScale > 1){
            for (uint32_t i = 0; i < iScale; i++){
                for (uint32_t j = 0; j < iScale; j++){
                    PixelWrite(x + i, y + j, pixel, tLayers[0]);
                };
            };
        } else {
            PixelWrite(x, y, pixel, tLayers[0]);
        };
    };
};

PixelType PixelRead(uint32_t x, uint32_t y, TextureType *texture){
    if ((x < texture->width) && (y < texture->height)){
        return texture->textureData[(texture->width)*y + x];
    };
    return Pixel(0, 0, 0, 0);   
};

PixelType PixelCheck(uint32_t x, uint32_t y){
    x = x*iScale;
    y = y*iScale;
    if ((x < tLayers[0]->width) && (y < tLayers[0]->height)){
        return tLayers[0]->textureData[(tLayers[0]->width)*y + x];
    };
    return Pixel(0, 0, 0, 0);
};

bool Construct(uint32_t screenWidth, uint32_t screenHeight, uint32_t scale, bool boolFullscreen, bool boolVsync){
    vScreenSize.X = screenWidth;
    vScreenSize.Y = screenHeight;
    iScale = scale;
    vWindowSize.X = vScreenSize.X * scale; 
    vWindowSize.Y = vScreenSize.Y * scale;
    bFullscreenIsEnabled = boolFullscreen;
    bVsyncIsEnabled = boolVsync;
    return true;
};

bool Start(){
    for (int i = 0; i < 3; i++){
        biMouse[i].pressed = false;
        biMouse[i].held = false;
        biMouse[i].released = false;
    };
    vMousePosition.X = 0;
    vMousePosition.Y = 0;
    for (int i = 0; i < 100; i++){
        biKeyboard[i].pressed = false;
        biKeyboard[i].held = false;
        biKeyboard[i].released = false;
    };
    vWindowPosition.X = 50;
    vWindowPosition.Y = 50;

    CreateWindowPane(bFullscreenIsEnabled, vWindowPosition, vWindowSize);  
    WindowUpdateSize(vWindowSize.X, vWindowSize.Y);
    bApplicationIsRunning = true;
    engineThread = CreateThread(NULL, 0 , *EngineThread, NULL, 0, NULL);
	MSG message;
	while (GetMessage(&message,NULL,0,0) > 0){
		TranslateMessage(&message);
		DispatchMessage(&message);
	};
    if (engineThread != NULL){
        WaitForSingleObject(engineThread, INFINITE);
    };
    return true;
};

DWORD WINAPI EngineThread(){
    CreateGraphics(bFullscreenIsEnabled, bVsyncIsEnabled, vViewPosition, vViewSize);
    PixelType *data = malloc(sizeof(PixelType)*vWindowSize.X*vWindowSize.Y);
    for (uint32_t i = 0; i < vWindowSize.X*vWindowSize.Y; i++){
        data[i] = Pixel(255,200,255,255);
    };
    TextureCreate(vWindowSize.X, vWindowSize.Y, data);
    free(data);
    tCurrentTarget = tLayers[0];
    TextureUpdate(tCurrentTarget);

    clTimeCounter1 = clock();
    clTimeCounter2 = clock();

    if (!OnUserInitialize()){
        bApplicationIsRunning = false;
    };
    while (bApplicationIsRunning){
        CoreUpdate();
    };
	wglDeleteContext(glRenderContext);
	PostMessage(windowHandle,WM_DESTROY,0,0);
    return 1;
};

void CoreUpdate(){
    clTimeCounter2 = clock();
    fElapsedTime = ((float)clTimeCounter2 - (float)clTimeCounter1)/CLOCKS_PER_SEC;
    clTimeCounter1 = clTimeCounter2;

    for (int i = 0; i < 3; i++){
        biMouse[i].pressed = false;
        biMouse[i].released = false;
        if (bMouseOldState[i] != bMouseNewState[i]){
            if (bMouseNewState[i]){
                biMouse[i].pressed = !biMouse[i].held;
                biMouse[i].held = true;
            } else {
                biMouse[i].released = true;
                biMouse[i].held = false;
            };
        };
        bMouseOldState[i] = bMouseNewState[i];
    };

    for (int i = 0; i < 100; i++){
        biKeyboard[i].pressed = false;
        biKeyboard[i].released = false;
        if (bKeyboardOldState[i] != bKeyboardNewState[i]){
            if (bKeyboardNewState[i]){
                biKeyboard[i].pressed = !biKeyboard[i].held;
                biKeyboard[i].held = true;
            } else {
                biKeyboard[i].released = true;
                biKeyboard[i].held = false;
            };
        };
        bKeyboardOldState[i] = bKeyboardNewState[i];
    };

    ClearBuffer(Pixel(0, 0, 0, 255), true);
    if (!OnUserUpdate()){
        bApplicationIsRunning = false;
    };
    glViewport(vViewPosition.X, vViewPosition.Y, vViewSize.X, vViewSize.Y);
    ClearBuffer(Pixel(0, 0, 0, 255), true);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
    TextureApply(tCurrentTarget->id);
    TextureUpdate(tCurrentTarget);
    glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
    SwapBuffers(glDeviceContext);

    iFramesPerSecond++;
    if (fElapsedTime >= 1.0f){
        fElapsedTime -= 1.0f;
        IntToStr(iFramesPerSecond, cWindowTitle);
        SetWindowText(windowHandle,cWindowTitle);
        iFramesPerSecond = 0;
    };
};

void ClearBuffer(PixelType pixel,bool boolDepth){
	glClearColor(((float)pixel.r)/255.0f,((float)pixel.g)/255.0f,((float)pixel.b)/255.0f,((float)pixel.a)/255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (boolDepth){
		glClear(GL_DEPTH_BUFFER_BIT);
	};
};

bool CreateWindowPane(bool boolFullscreen, vector2d windowPosition, vector2d windowSize){
    WNDCLASS windowClass;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpfnWndProc = WindowEventHandler;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.lpszMenuName = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszClassName = WINDOW_CLASS_NAME;
	RegisterClass(&windowClass);
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

	vector2d vTopLeft;
	vTopLeft.X = windowPosition.X;
	vTopLeft.Y = windowPosition.Y;
	
	if (boolFullscreen){
		dwExStyle = 0;
		dwStyle = WS_VISIBLE | WS_POPUP;
		HMONITOR hmon = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi;
		mi.cbSize = sizeof(mi);
		if (!GetMonitorInfo(hmon, &mi)) { 
            return false;
        };
		vWindowSize.X = mi.rcMonitor.right;
		vWindowSize.Y = mi.rcMonitor.bottom;
		vTopLeft.X = 0;
		vTopLeft.Y = 0;
	};

	RECT windowRectangle;
	windowRectangle.left = 0; windowRectangle.top = 0;
	windowRectangle.right = windowSize.X; windowRectangle.bottom = windowSize.Y;
	AdjustWindowRectEx(&windowRectangle, dwStyle, false, dwExStyle);
	int width = windowRectangle.right - windowRectangle.left;
	int height = windowRectangle.bottom - windowRectangle.top;

	windowHandle = CreateWindowEx(dwExStyle, WINDOW_CLASS_NAME, "App", dwStyle, vTopLeft.X, vTopLeft.Y, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
};

bool CreateDevice(HWND windowHandle, bool boolFullscreen, bool boolVsync){
    glDeviceContext = GetDC(windowHandle);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); pfd.nVersion = 1; pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA; pfd.cColorBits = 32; pfd.cAccumBits = 0;
	pfd.cRedBits = 0; pfd.cRedShift = 0; pfd.cGreenBits = 0; pfd.cGreenShift = 0; pfd.cBlueBits = 0; pfd.cBlueShift = 0; pfd.cAlphaBits = 0; pfd.cAlphaShift = 0;
	pfd.cAccumRedBits = 0; pfd.cAccumGreenBits = 0; pfd.cAccumBlueBits = 0; pfd.cAccumAlphaBits = 0; pfd.cDepthBits = 0; pfd.cStencilBits = 0; pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE; pfd.bReserved = 0; pfd.dwLayerMask = 0; pfd.dwVisibleMask = 0; pfd.dwDamageMask = 0;

	int pf = 0;
	if (!(pf = ChoosePixelFormat(glDeviceContext,&pfd))) {
		return false;
	};
	SetPixelFormat(glDeviceContext,pf,&pfd);
	if (!(glRenderContext = wglCreateContext(glDeviceContext))){
		return false;
	};
	wglMakeCurrent(glDeviceContext, glRenderContext);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
};

bool CreateGraphics(bool boolFullscreen, bool boolVsync, vector2d viewPosition, vector2d viewSize){
    CreateDevice(windowHandle, bFullscreenIsEnabled,bVsyncIsEnabled);
    glViewport(viewPosition.X,viewPosition.Y,viewSize.X,viewSize.Y);
    return true;
};

LRESULT CALLBACK WindowEventHandler(HWND windowHandle, UINT uMessage, WPARAM wParameter, LPARAM lParameter){
    switch(uMessage){
        case WM_SIZE: WindowUpdateSize(lParameter & 0xFFFF, (lParameter >> 16) & 0xFFFF); break;
        case WM_MOUSEMOVE: {
            uint16_t x = lParameter & 0xFFFF; uint16_t y = (lParameter >> 16) & 0xFFFF;
            int16_t ix = *(int16_t*)&x;   int16_t iy = *(int16_t*)&y;
            MouseUpdatePosition(ix,iy);
            break; 
        };
        case WM_KEYDOWN: KeySetState(true, wParameter); break;
        case WM_KEYUP: KeySetState(false, wParameter); break;
        case WM_LBUTTONDOWN: MouseUpdateState(true, 0); break;
        case WM_LBUTTONUP: MouseUpdateState(false, 0); break;
        case WM_RBUTTONDOWN: MouseUpdateState(true, 1); break;
        case WM_RBUTTONUP: MouseUpdateState(false, 1); break;
        case WM_MBUTTONDOWN: MouseUpdateState(true, 2); break;
        case WM_MBUTTONUP: MouseUpdateState(false, 2); break;
        case WM_DESTROY: PostQuitMessage(0); break;
        case WM_CLOSE: Terminate(); break;
    };
    return DefWindowProc(windowHandle, uMessage, wParameter, lParameter);
};

void WindowUpdateSize(uint32_t x, uint32_t y){
    vWindowSize.X = x;
    vWindowSize.Y = y;
    ViewportUpdate();
};

void ViewportUpdate(){
	uint32_t windowWidth = vScreenSize.X * iScale;
	uint32_t windowHeight = vScreenSize.Y * iScale;
	float ratio = (float)windowWidth / (float)windowHeight;

	vViewSize.X = (uint32_t)vWindowSize.X;
	vViewSize.Y = (uint32_t)((float)vViewSize.X/ratio);

	if (vViewSize.Y > vWindowSize.Y){
		vViewSize.Y = vWindowSize.Y;
		vViewSize.X = (uint32_t)((float)vViewSize.Y * ratio);
	};
	vViewPosition.X = (vWindowSize.X-vViewSize.X)/2;
	vViewPosition.Y = (vWindowSize.Y-vViewSize.Y)/2;
};

void Terminate(){
	for (uint32_t i = 0; i < iTextureNumber; i++){
		free(&tLayers[i]);
	};
	bApplicationIsRunning = false;
};

void CurrentTargetSetI(uint32_t index){
    tCurrentTarget = tLayers[index];
};

void CurrentTargetSetP(TextureType *texture){
    tCurrentTarget = texture;
};

void IntToStr(uint32_t value, char destination[]){
    char const digit[] = "0123456789";
    char* p = destination;
    uint32_t shifter = value;
    do {
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do {
        *--p = digit[value%10];
        value = value/10;
    } while(value);
};

bool LoadImageFromPath(char imagePath[]){
	int width, height, numberOfComponents;
	unsigned char *data = stbi_load(imagePath, &width, &height, &numberOfComponents, 4);
	if (data==NULL){
		printf("[FAIL] Unable to load image");
		return false;
	};
	TextureCreate(width,height,(PixelType*)data);
	stbi_image_free(data);
	return true;
};

void MouseUpdatePosition(int32_t x, int32_t y){
    vMousePosition.X = x / iScale;
    vMousePosition.Y = y / iScale;
};

void MouseUpdateState(bool state, int32_t button){
    bMouseNewState[button] = state;
};
void KeySetState(bool state, int32_t key){
    bKeyboardNewState[key] = state;
};