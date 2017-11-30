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
#include "soundclass.h"
#include "racetrack.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Car
////////////////////////////////////////////////////////////////////////////////
class Car
{
public:
	struct CollisionBox {
		D3DXVECTOR3 frontLeft;
		D3DXVECTOR3 frontRight;
		D3DXVECTOR3 backLeft;
		D3DXVECTOR3 backRight;
		CollisionBox()
			: frontLeft(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			frontRight(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			backLeft(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			backRight(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
		}
	};
	Car();
	Car(const Car&);
	~Car();
	bool Initialize(RaceTrack* &racetrack, GraphicsClass* &graphics, SoundClass* sound, HWND &hwnd, NetworkClass* &network, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Frame(float deltaTime, float gameTime);

	void Accelerate(bool);
	void BreakReverse(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void SetPosition(float xPos, float yPos, float zPos, float rotation);
	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetPosition();

	ModelClass* m_Model;
	CollisionBox m_Collider;

private:
	GraphicsClass* m_Graphics;
	NetworkClass* m_Network;
	SoundClass* m_Sound;
	RaceTrack* m_Racetrack;

	//Forward Vector, Up Vector & Right Vector + useful storage variables
	D3DXVECTOR3 startingForwardVector;
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR4 nextForwardVector;
	D3DXVECTOR3 forwardVectorNormalized;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	//Rotation matrix
	D3DXMATRIX rotation;

	//Position of model for graphics & game
	D3DXVECTOR3 position;
	float graphicsAngle;

	//Velocity, friction, steering etc...
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 friction;
	D3DXVECTOR3 acceleration;
	float speed;
	float accelerationFactor;
	float startAccelerationFactor;
	float gear;
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

	//Check to see if car is on track
	bool IsInsideTriangle(D3DXVECTOR3 s, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);
	bool isOnTrack = true;

	//For sound control
	bool idlecarsfx = false;
	bool slowcarsfx = false;
	bool midcarsfx = false;
	bool fastcarsfx = false;

	//Helper variables used to create the collision mesh
	D3DXVECTOR4 product;
	D3DXMATRIX worldMatrix;
	D3DXVECTOR3* mesh;
	Car::CollisionBox FindCollider();
};

#endif