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
#include "terrainshaderclass.h"
#include "terrainclass.h"
#include "foliageclass.h"
#include "foliageshaderclass.h"

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
	bool AddBitmapToPipeline(int screenNo, BitmapClass* &bitmap, HWND hwnd, WCHAR* bitmapFilename, int width, int height);

	CameraClass* m_Camera;
	TextClass* m_Text;
	D3DClass* m_D3D;

	void SetGameState(int gameState_in);
	void SetMenuState(int menuState_in);
	int GetScreenWidth();
	int GetScreenHeight();

	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	FoliageClass* m_Foliage;

	int countdown = -1;

private:
	bool Render();
	void RenderSkyDome(D3DXMATRIX &worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition);
	bool RenderMainMenu(D3DXMATRIX &worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition);
	void RenderText(int mode, D3DXMATRIX &worldMatrix);

private:
	//References to system objects
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	TerrainShaderClass* m_TerrainShader;
	TerrainClass* m_Terrain;
	D3DXMATRIX screenViewMatrix;
	FoliageShaderClass* m_FoliageShader;

	//Height and width of screen
	int m_screenWidth;
	int m_screenHeight;

	//Variables which store what state the game/menu is in, so the correct assets can be rendered
	int gameState;
	int menuState;

	//Arrays which determine what assets will be displayed for each gamestate
	ModelClass* mainGameAssets[300];
	BitmapClass* menuScreenOneAssets[10];
	BitmapClass* menuScreenTwoAssets[10];
	BitmapClass* countDownAssets[4];
	int countDownAssetCount;
	int mainGameAssetCount;
	int menuScreenOneAssetCount;
	int menuScreenTwoAssetCount;
};

#endif