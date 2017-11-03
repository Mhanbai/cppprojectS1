#include "game.h"

Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	mainPlayer = 0;
	m_raceTrack = 0;
}

Game::Game(const Game &)
{
}

Game::~Game()
{
}

bool Game::Initialize(InputClass* &input, GraphicsClass* &graphics, TextClass* &text, HWND &hwnd)
{
	bool result;
	m_Input = input;
	m_Graphics = graphics;
	m_Text = text;

	mainPlayer = new Car();
	if (!mainPlayer)
	{
		return false;
	}

	result = mainPlayer->Initialize(m_Graphics, hwnd,  "../Engine/data/c_main.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	m_raceTrack = new RaceTrack();
	if (!m_raceTrack)
	{
		return false;
	}

	result = m_raceTrack->Initialize(m_Graphics, hwnd);
	if (!result) {
		return false;
	}

	mainPlayer->SetPosition(-12.0f, 0.0f, 0.0f, 0.0f);

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

	m_Graphics->m_Camera->Follow(mainPlayer->GetPosition(), mainPlayer->GetForwardVector());

	if (m_Input->IsUpPressed() == true) {
		mainPlayer->Accelerate(true);
	}
	else {
		mainPlayer->Accelerate(false);
	}
	
	if (m_Input->IsDownPressed() == true) {
		mainPlayer->BreakReverse(true);
	}
	else {
		mainPlayer->BreakReverse(false);
	}

	if (m_Input->IsLeftPressed() == true)
	{
		mainPlayer->TurnLeft(true);
	}
	else {
		mainPlayer->TurnLeft(false);
	}

	if (m_Input->IsRightPressed() == true)
	{
		mainPlayer->TurnRight(true);
	}
	else {
		mainPlayer->TurnRight(false);
	}

	return true;
}
