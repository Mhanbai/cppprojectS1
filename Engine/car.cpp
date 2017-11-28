#include "car.h"

Car::Car()
{
	m_Graphics = 0;
	m_Model = 0;

	// Set up gameplay variables
	accelerationInput = 0.0f;
	steerInput = 0.0f;
	graphicsAngle = 0.0f;
	startAccelerationFactor = 10.0f;
	gear = 0.5f;
	startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	forwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	acceleration = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isAccelerating = false;
	isTurningLeft = false;
	isTurningRight = false;
	isBreakReversing = false;

	//Car handling
	frictionFactor = 0.5f;
	lateralFrictionFactor = 3.0f;
	steerFactor = 1.0f;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(GraphicsClass *& graphics, SoundClass* sound, HWND &hwnd, NetworkClass* &network, char* modelFilename, WCHAR* textureFilename)
{
	bool result;
	m_Graphics = graphics;
	m_Network = network;
	m_Sound = sound;

	lastMessageSent = 0.0f;

	result = m_Graphics->AddToPipeline(m_Model, hwnd, modelFilename, textureFilename);
	if (!result) {
		return false;
	}

	mesh = m_Model->GetCollisionMesh();

	m_Graphics->m_D3D->GetWorldMatrix(worldMatrix);

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		D3DXVec3Transform(&product, &mesh[i], &worldMatrix);
		mesh[i] = D3DXVECTOR3(product.x, product.y, product.z);
	}

	return true;
}

void Car::Shutdown()
{
	m_Graphics = 0;
	m_Model = 0;
}

void Car::Frame(float deltaTime, float gameTime)
{
	speed = (D3DXVec3Length(&velocity));

	if (speed < 5.0f) {
		if (idlecarsfx == false) {
			if (slowcarsfx == true) {
				m_Sound->StopLooping();
				slowcarsfx = false;
			}
			m_Sound->LoopSound("../Engine/data/caridle.wav");
			idlecarsfx = true;
		}
	}
	else if (speed < 30.0f) {
		if (slowcarsfx == false) {
			m_Sound->StopLooping();
			idlecarsfx = false;
			fastcarsfx = false;
			m_Sound->LoopSound("../Engine/data/carslow.wav");
			slowcarsfx = true;
		}
		gear = 0.5f;
	}
	else if ((speed >= 30.0f) && (speed < 60.0f)) {
		gear = 0.75f;
	}
	else {
		if (fastcarsfx == false) {
			m_Sound->StopLooping();
			slowcarsfx = false;
			m_Sound->LoopSound("../Engine/data/carfast.wav");
			fastcarsfx = true;
		}
		gear = 1.0f;
	}

	accelerationFactor = startAccelerationFactor + (speed * gear);


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

	//Angle of steering equals user input multiplied by how well car handles
	steerAngle = steerInput * steerFactor * deltaTime;
	steerAngle = steerAngle * (speed / 100.0f);

	//Calculate new forward vector
	D3DXMatrixRotationY(&rotation, steerAngle); //Create a matrix for rotation around Y from angle of steering
	D3DXVec3Transform(&nextForwardVector, &forwardVector, &rotation); //Transform forward vector by rotation matrix
	forwardVector = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z); //As Vec3Transform returns a Vector4, put these values into a Vector3

	//Calculate new right vector from forward vector
	D3DXVec3Normalize(&forwardVectorNormalized, &forwardVector);
	D3DXVec3Cross(&rightVector, &forwardVectorNormalized, &upVector);

	lateralVelocity = rightVector * D3DXVec3Dot(&velocity, &rightVector);
	lateralFriction = -lateralVelocity * lateralFrictionFactor;

	//Acceleration is equal to direction car is facing multiplied by the rate of acceleration multiplied by input
	acceleration = forwardVector * accelerationInput * accelerationFactor;

	//Friction is equal to the reverse of velocity multiplied by how frictiony the surface is
	friction = -velocity * frictionFactor;
	velocity += (friction + lateralFriction) * deltaTime;

	//Increase velocity by acceleration
	if (speed < 100.0f) {
		velocity += acceleration * deltaTime;
	}

	//Add velocity to position
	position = position + velocity * deltaTime;

	//Calculate angle car is facing for graphics
	graphicsAngle = atan2(forwardVector.z, forwardVector.x) - atan2(startingForwardVector.z, startingForwardVector.x);

	//Set the position of the cars model
	m_Model->SetPosition(position.x, position.y, position.z);
	m_Model->SetRotation(graphicsAngle * 57.2958f);

	//If we are in a multiplayer game, send a position update message every half second
	if (m_Network->connectionEstablished == true) {
		if ((gameTime - lastMessageSent) > 0.1f) {
			m_Network->PositionUpdate(position.x, position.z, gameTime, graphicsAngle);
			lastMessageSent = gameTime;
		}
	}

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		D3DXVec3Transform(&product, &mesh[i], &worldMatrix);
		mesh[i] = D3DXVECTOR3(product.x + position.x, product.y + position.x, product.y + position.z);
	}

	/////////////////////////

	/*char spdBuffer[64];
	sprintf_s(spdBuffer, "SPEED: X: %f Y: %f", velocity.x, velocity.z);

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, spdBuffer, 60, 150, 1.0f, 1.0f, 1.0f);*/

	/*char lenBuffer[64];
	sprintf_s(lenBuffer, "MAGNITUDE: %f", D3DXVec3Length(&velocity));

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, lenBuffer, 60, 150, 1.0f, 1.0f, 1.0f);*/
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
