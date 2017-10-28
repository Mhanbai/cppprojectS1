#include "game.h"

Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	mainPlayer = 0;
}

Game::Game(const Game &)
{
}

Game::~Game()
{
}

bool Game::Initialize(InputClass* &input, GraphicsClass* &graphics, HWND &hwnd)
{
	bool result;
	m_Input = input;
	m_Graphics = graphics;

	mainPlayer = new Car();
	if (!mainPlayer)
	{
		return false;
	}

	result = mainPlayer->Initialize(m_Graphics, hwnd, L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	mainPlayer->SetPosition(0.0f, 0.0f, 10.0f, 0.0f);

	return true;
}

void Game::Shutdown()
{
	m_Input = 0;
	m_Graphics = 0;

	if (mainPlayer) {
		mainPlayer->Shutdown();
		delete mainPlayer;
		mainPlayer = 0;
	}
}

bool Game::Frame()
{
	mainPlayer->Frame();

	m_Graphics->m_Camera->Follow(mainPlayer->GetPosition());

	if (m_Input->IsUpPressed() == true)
	{
		mainPlayer->Accelerate();
	}

	if (m_Input->IsDownPressed() == true)
	{
		mainPlayer->BreakReverse();
	}

	if (m_Input->IsLeftPressed() == true)
	{
		mainPlayer->TurnLeft();
	}

	if (m_Input->IsRightPressed() == true)
	{
		mainPlayer->TurnRight();
	}

	return true;
}