#include "car.h"

Car::Car()
{
	m_Graphics = 0;
	m_Model = 0;

	// Set up gameplay variables
	velAngle = 0.0f;
	velLength = 0.0f;

	acceleration = 0.1f;
	maxForwardSpeed = 2.0f;

	reverseAcceleration = 0.01f;
	maxBackwardSpeed = -0.3f;

	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(GraphicsClass *& graphics, HWND &hwnd, WCHAR* textureFilename)
{
	bool result;
	m_Graphics = graphics;

	result = m_Graphics->AddToPipeline(m_Model, hwnd, "../Engine/data/c_main.txt", textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void Car::Shutdown()
{
	m_Graphics = 0;
	m_Model = 0;
}

void Car::Frame()
{
	position = position + velocity;

	m_Model->SetPosition(position.x, position.y, position.z);
	m_Model->SetRotation(velAngle);
}

void Car::Accelerate()
{
	if (velocity.z < maxForwardSpeed) {
		velocity.z += acceleration;
	}
}

void Car::BreakReverse()
{
	if (velocity.z > 0.1f) {
		velocity.z -= acceleration;
	}
	else if (velocity.z > maxBackwardSpeed) {
		velocity.z -= reverseAcceleration;
	}
}

void Car::TurnLeft()
{
	velAngle = velAngle + 0.5f;
}

void Car::TurnRight()
{
	velAngle = velAngle - 0.5f;
}

void Car::SetPosition(float xPos, float yPos, float zPos, float rotation)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	velAngle = rotation;
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}
