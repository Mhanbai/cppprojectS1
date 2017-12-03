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
#include <vector>
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
	void Reset();
	void Shutdown();

	void Frame(float deltaTime, float totalTime);
	void SetPosition(float xPos, float yPos, float zPos, float rotation);

private:
	//References to system objects
	GraphicsClass* m_Graphics;
	ModelClass* m_Model;
	NetworkClass* m_Network;
	TextClass* m_Text;

	//Variables for interpolation of position
	D3DXVECTOR3 lastPosition;
	D3DXVECTOR3 nextPosition;
	float velX;
	float velZ;
	float predictionTime;
	float lastPredictionTime = 0.0f;
	float frame = 1;
	float extrapolationFactor = 0.0f;

	//Position of model for graphics & game
	D3DXVECTOR3 currentForwardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 forwardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 nextForwardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float lastGraphicsAngle = 0.0f;
	float graphicsAngle = 0.0f;
	float nextGraphicsAngle = 0.0f;
	bool startUpdating = false;
};

#endif