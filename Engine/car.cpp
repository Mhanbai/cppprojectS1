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

	m_Collider = FindCollider();

	return true;
}

void Car::Shutdown()
{
	m_Graphics = 0;
	m_Model = 0;
	mesh = 0;
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

	//Update the collider
	m_Collider.backLeft = m_Collider.backLeft - position;
	D3DXVec3Transform(&nextForwardVector, &m_Collider.backLeft, &rotation);
	m_Collider.backLeft = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z);
	m_Collider.backLeft = m_Collider.backLeft + position;
	m_Collider.backLeft = m_Collider.backLeft + velocity * deltaTime;

	m_Collider.backRight = m_Collider.backRight - position;
	D3DXVec3Transform(&nextForwardVector, &m_Collider.backRight, &rotation);
	m_Collider.backRight = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z);
	m_Collider.backRight = m_Collider.backRight + position;
	m_Collider.backRight = m_Collider.backRight + velocity * deltaTime;

	m_Collider.frontLeft = m_Collider.frontLeft - position;
	D3DXVec3Transform(&nextForwardVector, &m_Collider.frontLeft, &rotation);
	m_Collider.frontLeft = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z);
	m_Collider.frontLeft = m_Collider.frontLeft + position;
	m_Collider.frontLeft = m_Collider.frontLeft + velocity * deltaTime;

	m_Collider.frontRight = m_Collider.frontRight - position;
	D3DXVec3Transform(&nextForwardVector, &m_Collider.frontRight, &rotation);
	m_Collider.frontRight = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z);
	m_Collider.frontRight = m_Collider.frontRight + position;
	m_Collider.frontRight = m_Collider.frontRight + velocity * deltaTime;

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

	char spdBuffer[64];
	sprintf_s(spdBuffer, "POS: X: %f Y: %f Z: %f", m_Collider.frontRight.x, m_Collider.frontRight.y, m_Collider.frontRight.z);

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, spdBuffer, 60, 150, 1.0f, 1.0f, 1.0f);

	char lenBuffer[64];
	sprintf_s(lenBuffer, "MAINPOS: X: %f Y: %f Z: %f", position.x, position.y, position.z);

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence5, lenBuffer, 60, 170, 1.0f, 1.0f, 1.0f);
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
	float xOffset = position.x - xPos;
	float yOffset = position.y - yPos;
	float zOffset = position.z - zPos;

	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	graphicsAngle = rotation;

	//Update position of collider
	m_Collider.frontRight.x = m_Collider.frontRight.x - xOffset;
	m_Collider.frontRight.z = m_Collider.frontRight.z - zOffset;
	m_Collider.frontLeft.x = m_Collider.frontLeft.x - xOffset;
	m_Collider.frontLeft.z = m_Collider.frontLeft.z - zOffset;
	m_Collider.backRight.x = m_Collider.backRight.x - xOffset;
	m_Collider.backRight.z = m_Collider.backRight.z - zOffset;
	m_Collider.backLeft.x = m_Collider.backLeft.x - xOffset;
	m_Collider.backLeft.z = m_Collider.backLeft.z - zOffset;
}

D3DXVECTOR3 Car::GetForwardVector()
{
	return forwardVector;
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}

Car::CollisionBox Car::FindCollider()
{
	m_Model->GetCollisionMesh(mesh);
	m_Graphics->m_D3D->GetWorldMatrix(worldMatrix);

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		D3DXVec3Transform(&product, &mesh[i], &worldMatrix);
		mesh[i] = D3DXVECTOR3(product.x, product.y, product.z);
	}

	float biggestX = mesh[0].x;
	float biggestZ = mesh[0].z;
	float lowestX = mesh[0].x;
	float lowestZ = mesh[0].z;

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		if (mesh[i].x > biggestX) {
			biggestX = mesh[i].x;
		}
		if (mesh[i].x < lowestX) {
			lowestX = mesh[i].x;
		}
		if (mesh[i].z > biggestZ) {
			biggestZ = mesh[i].z;
		}
		if (mesh[i].z < lowestZ) {
			lowestZ = mesh[i].z;
		}
	}

	CollisionBox returner;

	returner.frontLeft = D3DXVECTOR3(lowestX, 0.0f, biggestZ);
	returner.backLeft = D3DXVECTOR3(lowestX, 0.0f, lowestZ);
	returner.frontRight = D3DXVECTOR3(biggestX, 0.0f, biggestZ);
	returner.backRight = D3DXVECTOR3(biggestX, 0.0f, lowestZ);

	return returner;
}
