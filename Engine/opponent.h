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
#include "networkclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Opponent
////////////////////////////////////////////////////////////////////////////////
class Opponent
{
public:
	Opponent();
	Opponent(const Opponent&);
	~Opponent();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, NetworkClass* &network, TextClass* &text, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();

	void Frame(float deltaTime, float totalTime);
	void SetPosition(float xPos, float yPos, float zPos, float rotation);

private:
	GraphicsClass* m_Graphics;
	ModelClass* m_Model;
	NetworkClass* m_Network;
	TextClass* m_Text;

	//Variables for interpolation of position
	float velX;
	float velZ;
	float velA;
	float predictionTime;

	//Position of model for graphics & game
	D3DXVECTOR3 startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 position;
	float graphicsAngle;
};

#endif