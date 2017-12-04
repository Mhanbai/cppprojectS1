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
	steerFactor = 1.1f;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(RaceTrack* &racetrack, GraphicsClass *& graphics, SoundClass* sound, HWND &hwnd, NetworkClass* &network, char* modelFilename, WCHAR* textureFilename)
{
	bool result;
	m_Graphics = graphics;
	m_Network = network;
	m_Sound = sound;
	m_Racetrack = racetrack;

	//Tracks how often a position update has been sent to the opposing player
	lastMessageSent = 0.0f;

	//Add the car to the graphics pipeline
	result = m_Graphics->AddToPipeline(m_Model, hwnd, modelFilename, textureFilename);
	if (!result) {
		return false;
	}

	//Find the list of positions that determine the boundaries of the car
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
	if (m_Network->errorPause == false) {
		//The speed of the car is equivalent to the magnitude of the velocity vector
		speed = (D3DXVec3Length(&velocity));

		//This handles both gear changing and car noise
		if (speed < 5.0f) { //If speed is within a certain limit
			if (idlecarsfx == false) { //And appropriate noise is not already looping
				if (slowcarsfx == true) { //If the noise for the previous speed is still looping, stop looping it
					m_Sound->StopWaveFile(2);
					slowcarsfx = false;
				}
				m_Sound->LoopWaveFile("data/caridle.wav", 2); //Then start looping the new noise
				idlecarsfx = true; //Boolean to determine that the noise is playing
			}
		}
		else if (speed < 30.0f) {
			if (slowcarsfx == false) {
				m_Sound->StopWaveFile(2);
				idlecarsfx = false;
				fastcarsfx = false;
				m_Sound->LoopWaveFile("data/carslow.wav", 2);
				slowcarsfx = true;
			}
			gear = 0.5f;
		}
		else if ((speed >= 30.0f) && (speed < 60.0f)) {
			gear = 0.75f;
		}
		else {
			if (fastcarsfx == false) {
				m_Sound->StopWaveFile(2);
				slowcarsfx = false;
				m_Sound->LoopWaveFile("data/carfast.wav", 2);
				fastcarsfx = true;
			}
			gear = 1.0f;
		}

		//Controls the 'gear' i.e. the car moves at a speed relevant to how fast its already going
		accelerationFactor = startAccelerationFactor + (speed * gear);

		//User Input
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

		//Divide the angle by speed divided by 100. This stops the car from being able to turn on the spot
		steerAngle = steerAngle * (speed / 100.0f);

		//Calculate new forward vector
		D3DXMatrixRotationY(&rotation, steerAngle); //Create a matrix for rotation around Y from angle of steering
		D3DXVec3Transform(&nextForwardVector, &forwardVector, &rotation); //Transform forward vector by rotation matrix
		forwardVector = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z); //As Vec3Transform returns a Vector4, put these values into a Vector3

		//Calculate new right vector from forward vector
		D3DXVec3Normalize(&forwardVectorNormalized, &forwardVector);
		D3DXVec3Cross(&rightVector, &forwardVectorNormalized, &upVector);

		//Lateral friction stops the car from drifting as if it is on ice
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
		m_Collider.backLeft = m_Collider.backLeft - position; //Translate the collider so the Y axis is through the origin
		D3DXVec3Transform(&nextForwardVector, &m_Collider.backLeft, &rotation); //Rotate the collider by the rotation matrix
		m_Collider.backLeft = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z); //Convert Vec4 from calculation into Vec3
		m_Collider.backLeft = m_Collider.backLeft + position; //Translate the collider back to its position
		m_Collider.backLeft = m_Collider.backLeft + velocity * deltaTime; //Apply the same velocity as for the car

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

		//Apply velocity to car position
		position = position + velocity * deltaTime;

		//Calculate angle car is facing for graphics
		graphicsAngle = atan2(forwardVector.z, forwardVector.x) - atan2(startingForwardVector.z, startingForwardVector.x);

		//Find which track nodes are close to the car
		for (unsigned int i = 0; i < m_Racetrack->relVertex.size(); i++) { //For each vertex in the list of track nodes
			D3DXVECTOR3 distance;
			D3DXVec3Subtract(&distance, &position, &m_Racetrack->relVertex[i]); //Find the vector between the car and the node
			float fltDist = D3DXVec3Length(&distance); //Find magnitude in order to get distance
			if (fltDist < 80.0f) { //If distance is less than 70, add it to the 'nodes that are close' list 
				toCompare.push_back(m_Racetrack->relVertex[i]);
			}
		}

		if (toCompare.size() > 0) { //Go through the list of nodes that are close
			int topRightCount = 0;
			int topLeftCount = 0;
			int backRightCount = 0;
			int backLeftCount = 0;
			for (unsigned int i = 0; i < toCompare.size(); i++) {
				if (GetLateralPosition(toCompare[i], m_Collider.frontRight, m_Collider.backRight)) {
					backRightCount++; //If theres at least one node on the left...
				}
				else {
					topLeftCount++; //...and at least one on the right...
				}
			}

			if ((backRightCount > 0) && (topLeftCount > 0)) {
				isOnTrack = true; //...then we're on the track.
			}
			else {
				isOnTrack = false;
			}
		}
		else {
			isOnTrack = false; //If there are no nodes that are close, we're obviously not on the track
		}

		if (isOnTrack == true) { //if we're not on the track, we should be level with the terrain
			position.y = 2.0f;
		}
		else if (isOnTrack == false) {
			position.y = 4.0f;
		}

		toCompare.clear(); //Clear the list of nodes that are close for next time

		//Set the position of the cars model
		m_Model->SetPosition(position.x, position.y, position.z);
		m_Model->SetRotation(graphicsAngle * 57.2958f);
	}

	//If we are in a multiplayer game, send a position update message every half second
	if ((m_Network->connectionEstablished == true) && (gameStarted == true)) {
		if ((gameTime - lastMessageSent) > 0.1f) {
			m_Network->PositionUpdate(position.x, position.z, gameTime, graphicsAngle);
			lastMessageSent = gameTime;
		}
	}
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

void Car::EndGameState()
{
	friction = -velocity * 2.0f;
	velocity += (friction + lateralFriction) * 0.016f;
}

bool Car::GetLateralPosition(D3DXVECTOR3 toTest, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
{
	//This calculation returns a negative number if 'toTest' is to the left of the two line points, and a positive number or zero if it is to the right
	float orientation = ((linePoint2.x - linePoint1.x) * (toTest.z - linePoint1.z)) - ((linePoint2.z - linePoint1.z) * (toTest.x - linePoint1.x));
	if (orientation >= 0.0f) {
		return true; //Point is to the right
	}
	else {
		return false; //Point is to the left
	}
}

bool Car::CheckIntersection(Car::CollisionBox carRect, RaceTrack::CheckPoint line)
{
	if ((GetLateralPosition(carRect.backLeft, line.point1, line.point2) && 
		GetLateralPosition(carRect.backRight, line.point1, line.point2) && 
		GetLateralPosition(carRect.frontLeft, line.point1, line.point2) && 
		GetLateralPosition(carRect.frontRight, line.point1, line.point2)) ||
		(!GetLateralPosition(carRect.backLeft, line.point1, line.point2) &&
		!GetLateralPosition(carRect.backRight, line.point1, line.point2) &&
		!GetLateralPosition(carRect.frontLeft, line.point1, line.point2) &&
		!GetLateralPosition(carRect.frontRight, line.point1, line.point2))) 
	{
		return false;
	}

	if ((line.point1.x > carRect.frontRight.x) && (line.point2.x > carRect.frontRight.x)) {
		return false;
	} else if ((line.point1.x < carRect.backLeft.x) && (line.point2.x < carRect.backLeft.x)) {
		return false;
	} else if ((line.point1.y > carRect.frontRight.y) && (line.point2.y > carRect.frontRight.y)) {
		return false;
	} else if ((line.point1.y < carRect.backLeft.y) && (line.point2.y < carRect.backLeft.y)) {
		return false;
	}
	else {
		return true;
	}
}

void Car::SetGameStarted()
{
	gameStarted = true;
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
