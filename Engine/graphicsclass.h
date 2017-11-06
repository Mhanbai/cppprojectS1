////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "textureshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, TextClass* &text, HWND);
	void Shutdown();
	bool Frame();

	bool AddToPipeline(ModelClass* &model, HWND hwnd, char* modelFilename, WCHAR* textureFilename);
	bool AddBitmapToPipeline(BitmapClass* &bitmap, HWND hwnd, WCHAR* bitmapFilename, int width, int height);

	CameraClass* m_Camera;
	TextClass* m_Text;
	D3DClass* m_D3D;

	void SetGameState(int gameState_in);
	int GetScreenWidth();
	int GetScreenHeight();

private:
	bool Render();

private:
	int gameState;

	int m_screenWidth;
	int m_screenHeight;

	ModelClass* *modelList;
	int modelCount;
	BitmapClass* *bitmapList;
	int bitmapCount;

	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	D3DXMATRIX screenViewMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, -4.0f, 4.0f, 1.0f);
};

#endif