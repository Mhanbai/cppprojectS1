#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: opponent.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _OPPONENT_H_
#define _OPPONENT_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <windows.h>
#include <time.h>
#include "graphicsclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Opponent
////////////////////////////////////////////////////////////////////////////////
class Opponent
{
public:
	Opponent();
	Opponent(const Opponent&);
	~Opponent();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();

	void Frame(float deltaTime);
	void SetPosition(float xPos, float yPos, float zPos, float rotation);

private:
	GraphicsClass* m_Graphics;
	ModelClass* m_Model;

	//Position of model for graphics & game
	D3DXVECTOR3 position;
	float graphicsAngle;
};

#endif