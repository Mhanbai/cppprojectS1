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

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
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

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	bool AddToPipeline(ModelClass* &model, HWND hwnd, char* modelFilename, WCHAR* textureFilename);

	CameraClass* m_Camera;

private:
	bool Render();

private:
	ModelClass* *modelList;
	int modelCount;
	D3DClass* m_D3D;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	D3DXMATRIX screenViewMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, -4.0f, 4.0f, 1.0f);
};

#endif