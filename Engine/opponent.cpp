#include "opponent.h"

Opponent::Opponent()
{
}

Opponent::Opponent(const Opponent &)
{
}

Opponent::~Opponent()
{
}

bool Opponent::Initialize(GraphicsClass *& graphics, HWND & hwnd, NetworkClass* &network, TextClass* &text, char * modelFilename, WCHAR * textureFilename)
{
	bool result;
	m_Graphics = graphics;
	m_Network = network;
	m_Text = text;

	graphicsAngle = 0.0f;

	position = D3DXVECTOR3(-12.0f, 2.0f, 0.0f);

	result = m_Graphics->AddToPipeline(m_Model, hwnd, modelFilename, textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void Opponent::Shutdown()
{
	m_Graphics = 0;
	m_Model = 0;
	m_Network = 0;
	m_Text = 0;
}

void Opponent::Frame(float deltaTime, float totalTime)
{
	if (m_Network->updateAvailable == true) {
		//Make life easier by assigning the last two relevant messages to variables
		Update pos0 = m_Network->positionUpdates[m_Network->positionUpdates.size() - 1];
		Update pos1 = m_Network->positionUpdates[m_Network->positionUpdates.size() - 2];

		//Calculate velocities for X and Z from last two messages
		velX = (pos0.posX - pos1.posX) / (pos0.timeStamp - pos1.timeStamp);
		velZ = (pos0.posZ - pos1.posZ) / (pos0.timeStamp - pos1.timeStamp);

		//Calculate the time for when we are trying to predict
		predictionTime = totalTime - pos0.timeStamp;

		//Linear prediction model to calculate where we want the car to be
		if (!(isnan(velX)) && (!isnan(velZ))) {
			lastPosition = position;
			nextPosition = D3DXVECTOR3((pos0.posX + velX * predictionTime), 2.0f, (pos0.posZ + velZ * predictionTime));
			currentForwardVector = forwardVector;
			D3DXVec3Subtract(&nextForwardVector, &nextPosition, &lastPosition);
		}
		frame = 1;

		if (startUpdatingAngle == false) {
			for (int i = 0; i < m_Network->positionUpdates.size(); i++) {
				if ((m_Network->positionUpdates[i].posX > 0.0f) || (m_Network->positionUpdates[i].posZ > 0.0f)) {
					startUpdatingAngle = true;
				}
			}
		}

		m_Network->updateAvailable = false;
	}

	D3DXVec3Lerp(&position, &lastPosition, &nextPosition, 0.16666666666666f * frame);
	D3DXVec3Lerp(&forwardVector, &currentForwardVector, &nextForwardVector, 0.16666666666666f * frame);

	if (startUpdatingAngle == true) {
		graphicsAngle = atan2(forwardVector.z, forwardVector.x) - atan2(startingForwardVector.z, startingForwardVector.x);
	}

	frame++;

	//Set the model to where the car is
	m_Model->SetPosition(position.x, position.y, position.z);
	m_Model->SetRotation(graphicsAngle * 57.2958f);
}

void Opponent::SetPosition(float xPos, float yPos, float zPos, float rotation)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	graphicsAngle = rotation;
}
