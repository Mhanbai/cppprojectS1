#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: car.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAR_H_
#define _CAR_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <windows.h>
#include "graphicsclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Car
////////////////////////////////////////////////////////////////////////////////
class Car
{
public:
	Car();
	Car(const Car&);
	~Car();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Frame();

	void Accelerate();
	void BreakReverse();
	void TurnLeft();
	void TurnRight();
	void ApplyNewton();
	void SetPosition(float xPos, float yPos, float zPos, float rotation);
	D3DXVECTOR3 GetPosition();

private:
	GraphicsClass* m_Graphics;
	ModelClass* m_Model;

	float velAngle;
	float velLength;
	float acceleration;
	float maxForwardSpeed;
	float reverseAcceleration;
	float maxBackwardSpeed;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 position;

};

#endif