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
