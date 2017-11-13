#include "car.h"

Car::Car()
{
	m_Graphics = 0;
	m_Model = 0;

	// Set up gameplay variables
	steerInput = 0.0f;
	graphicsAngle = 0.0f;
	startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	forwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	acceleration = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
	accelerationInput = 0.0f;
	accelerationFactor = 0.75f;
	frictionFactor = 0.00005f;
	lateralFrictionFactor = 0.05f;
	steerFactor = 0.03f;
	maxSpeed = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isAccelerating = false;
	isTurningLeft = false;
	isTurningRight = false;
	isBreakReversing = false;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(GraphicsClass *& graphics, HWND &hwnd, char* modelFilename, WCHAR* textureFilename)
{
	bool result;
	m_Graphics = graphics;

	result = m_Graphics->AddToPipeline(m_Model, hwnd, modelFilename, textureFilename);
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
	// Change input values based on user input
	if (isAccelerating) {
		accelerationInput = 1.0f;
	}
	else if (isBreakReversing) {
		accelerationInput = -0.5f;
	}
	else {
		accelerationInput = 0.0f;
	} 

	if (isTurningLeft) {
		if (isBreakReversing) {
			steerInput = 1.0f;
		}
		else {
			steerInput = -1.0f;
		}
	}
	else if (isTurningRight) {
		if (isBreakReversing) {
			steerInput = -1.0f;
		}
		else {
			steerInput = 1.0f;
		}
	}
	else {
		steerInput = 0.0f;
	}

	//Normalize the velocity to find the current speed
	D3DXVec3Normalize(&currentSpeed, &velocity);

	//Angle of steering equals user input multiplied by how well car handles
	steerAngle = steerInput * steerFactor;

	//Calculate new forward vector
	D3DXMatrixRotationY(&rotation, steerAngle); //Create a matrix for rotation around Y from angle of steering
	D3DXVec3Transform(&nextForwardVector, &forwardVector, &rotation); //Transform forward vector by rotation matrix
	forwardVector = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z); //As Vec3Transform returns a Vector4, put these values into a Vector3

	//Calculate new right vector from forward vector
	D3DXVec3Normalize(&forwardVectorNormalized, &forwardVector);
	D3DXVec3Cross(&rightVector, &forwardVectorNormalized, &upVector);

	lateralVelocity = rightVector * D3DXVec3Dot(&velocity, &rightVector);
	lateralFriction = -velocity * lateralFrictionFactor;

	//Acceleration is equal to direction car is facing multiplied by the rate of acceleration multiplied by input
	acceleration = forwardVector * accelerationInput * accelerationFactor;

	//Friction is equal to the reverse of velocity multiplied by how frictiony the surface is
	friction = -velocity * frictionFactor;
	velocity += friction + lateralFriction;

	//Check that current speed is not higher than max speed
	if (currentSpeed < maxSpeed) {
		//Add acceleration to velocity
		velocity += acceleration;
	}

	//Add velocity to position
	position = position + velocity;

	//Calculate angle car is facing for graphics
	graphicsAngle = atan2(forwardVector.z, forwardVector.x) - atan2(startingForwardVector.z, startingForwardVector.x);

	//Set the position of the cars model
	m_Model->SetPosition(position.x, position.y, position.z);
	m_Model->SetRotation(graphicsAngle * 57.2958f);

	//Debug Info
	char posX[20];
	sprintf_s(posX, "X: %f", position.x);
	char posZ[20];
	sprintf_s(posZ, "Z: %f", position.z);
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence2, posX, 60, 70, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence3, posZ, 60, 90, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	/*m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, "4: Test", 60, 110, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence5, "5: Test", 60, 130, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence6, "6: Test", 60, 150, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());*/
}

void Car::Accelerate(bool set)
{
	if (set == true) {
		isAccelerating = true;
	}
	else {
		isAccelerating = false;
	}
}

void Car::BreakReverse(bool set)
{
	if (set == true) {
		isBreakReversing = true;
	}
	else {
		isBreakReversing = false;
	}
}

void Car::TurnLeft(bool set)
{
	if (set == true) {
		isTurningLeft = true;
	}
	else {
		isTurningLeft = false;
	}
}

void Car::TurnRight(bool set)
{
	if (set == true) {
		isTurningRight = true;
	}
	else {
		isTurningRight = false;
	}
}

void Car::SetPosition(float xPos, float yPos, float zPos, float rotation)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	graphicsAngle = rotation;
}

D3DXVECTOR3 Car::GetForwardVector()
{
	return forwardVector;
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}
