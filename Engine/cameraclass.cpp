////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass(TextClass* &text, D3DClass* &d3d)
{
	m_Text = text;
	m_D3D = d3d;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	distance = 20.0f; //Distance to keep from car
	height = -4.0f; //Height from ground
	springConstant = 80.0f;
	dampConstant = 2.0f * sqrt(springConstant);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render()
{
	D3DXVECTOR3 position;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &-carPos, &up);

	////DEBUG///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*D3DXVECTOR3 up, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = -1.0f;

	m_rotationX = -90.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);*/

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::Follow(D3DXVECTOR3 followTarget, D3DXVECTOR3 targetForwardVector, float deltaTime)
{
	carPos = followTarget;

	D3DXVECTOR3 myPosition = GetPosition();
	D3DXVECTOR3 idealPosition = followTarget - targetForwardVector * distance + up * height;
	D3DXVECTOR3 displacement = myPosition + idealPosition;
	D3DXVECTOR3 springAcceleration = (-springConstant * displacement) - (dampConstant * velocity);
	velocity += springAcceleration * deltaTime;

	SetPosition(myPosition.x + (velocity.x * deltaTime), myPosition.y + (velocity.y * deltaTime), myPosition.z + (velocity.z * deltaTime));
}
