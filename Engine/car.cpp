#include "car.h"

bool Car::Initialize(D3DClass* m_D3D, HWND hwnd, GraphicsClass* gfx, float xPos, float yPos, float zPos, float rotation)
{
	velLength = 0.0f;
	acceleration = 0.2f;
	reverseAcceleration = 0.1f;
	maxForwardSpeed = -1.0f;
	maxBackwardSpeed = 0.5f;

	velAngle = rotation;
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(xPos, yPos, zPos);

	carModel = new ModelClass;

	bool result = carModel->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/Car1Texture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the main Player object.", L"Error", MB_OK);
		return false;
	}

	gfx->AddToGraphicsPipeline(carModel);

	return true;
}

void Car::Frame()
{
	/*velocity.x = velLength * cos(velAngle * 0.0174532925f);
	velocity.y = velLength * sin(velAngle * 0.0174532925f);
	velocity.z = velLength * sin(velAngle * 0.0174532925f);*/

	position = position + velocity;

	carModel->SetModelPosition(position.x, position.y, position.z);
	carModel->SetModelRotation(velAngle);
}

void Car::Accelerate()
{
	if (velocity.z > maxForwardSpeed) {
		velocity.z -= acceleration;
	}
}

void Car::Break()
{
	if (velocity.z < 0.1f) {
		velocity.z += acceleration;
	}
	else {
		velocity.z += reverseAcceleration;
	}

}

void Car::TurnLeft()
{
}

void Car::TurnRight()
{
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}
