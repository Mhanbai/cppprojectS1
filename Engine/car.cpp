#include "car.h"

Car::Car()
{
	m_Graphics = 0;
	m_Model = 0;
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

	result = m_Graphics->AddToPipeline(m_Model, hwnd, "../Engine/data/car.txt", textureFilename);
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

bool Car::Frame()
{
	return true;
}
