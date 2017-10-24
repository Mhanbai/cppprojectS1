#pragma once

#include "graphicsclass.h"

class Car {
private:
	// Length and angle of velocity vector
	float velAngle;
	float velLength;
	float acceleration;
	float maxForwardSpeed;
	float reverseAcceleration;
	float maxBackwardSpeed;

	// Velocity vector
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 position;

	// The model for this car
	ModelClass* carModel;

public:
	bool Initialize(D3DClass* m_D3D, HWND hwnd, float xPos, float yPos, float zPos, float rotation);
	void Frame(GraphicsClass* gfx);
	void Accelerate();
	void Break();
	void TurnLeft();
	void TurnRight();
	D3DXVECTOR3 GetPosition();
};