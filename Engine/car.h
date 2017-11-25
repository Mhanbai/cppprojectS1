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
#include <time.h>
#include "graphicsclass.h"
#include "networkclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Car
////////////////////////////////////////////////////////////////////////////////
class Car
{
public:
	Car();
	Car(const Car&);
	~Car();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, NetworkClass* &network, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Frame(float deltaTime, float gameTime);

	void Accelerate(bool);
	void BreakReverse(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void SetPosition(float xPos, float yPos, float zPos, float rotation);
	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetPosition();

	int speed;

private:
	GraphicsClass* m_Graphics;
	ModelClass* m_Model;
	NetworkClass* m_Network;

	//Forward Vector, Up Vector & Right Vector + useful storage variables
	D3DXVECTOR3 startingForwardVector;
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR4 nextForwardVector;
	D3DXVECTOR3 forwardVectorNormalized;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	//Rotation matrix
	D3DXMATRIX rotation;

	//Current Speed & Max Speed
	D3DXVECTOR3 currentSpeed;
	D3DXVECTOR3 maxSpeed;

	//Position of model for graphics & game
	D3DXVECTOR3 position;
	float graphicsAngle;

	//Velocity, friction, steering etc...
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 friction;
	D3DXVECTOR3 acceleration;
	float accelerationFactor;
	float frictionFactor;
	float accelerationInput;
	float steerInput;
	float steerFactor;
	float steerAngle;
	float steeringScalar;

	//Values for lateral velocity
	D3DXVECTOR3 lateralVelocity;
	D3DXVECTOR3 lateralFriction;
	float lateralFrictionFactor;

	//Booleans for user input
	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;
	bool isBreakReversing;

	//Information for network messages
	float timeStamp;
	float lastMessageSent;
};

#endif