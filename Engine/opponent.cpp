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

bool Opponent::Initialize(GraphicsClass *& graphics, HWND & hwnd, char * modelFilename, WCHAR * textureFilename)
{
	bool result;
	m_Graphics = graphics;

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
}

void Opponent::Frame(float deltaTime)
{


	m_Model->SetPosition(position.x, position.y + 2.0f, position.z);
	m_Model->SetRotation(graphicsAngle * 57.2958f);
}

void Opponent::SetPosition(float xPos, float yPos, float zPos, float rotation)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	graphicsAngle = rotation;
}

