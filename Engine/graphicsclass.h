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
#include "lightshaderclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include <vector>


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
	bool Frame(D3DXVECTOR3 toFollow);

	void AddToGraphicsPipeline(ModelClass* toAdd);

	D3DClass* m_D3D;

private:
	bool Render(float, float, D3DXVECTOR3);

private:
	std::vector<ModelClass*> graphicsPipeline;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
};

#endif