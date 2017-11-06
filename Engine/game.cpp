#include "game.h"

Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	mainPlayer = 0;
	m_raceTrack = 0;
	menuScreen = 0;
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
	m_hwnd = hwnd;

	result = InitializeMenuScreen();
	if (!result) {
		MessageBox(m_hwnd, L"Could not initialize menu screen.", L"Error", MB_OK);
		return false;
	}
	result = InitializeMainGame();
	if (!result) {
		MessageBox(m_hwnd, L"Could not initialize gameplay scene.", L"Error", MB_OK);
		return false;
	}
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
	switch (gameState) {
	case 0:
		if (m_Input->IsUpPressed() == true) {
			if (menuState < 3) {
				menuState++;
			}
			else {
				menuState = 0;
			}
		}
		if (m_Input->IsDownPressed() == true) {
			if (menuState > 0) {
				menuState--;
			}
			else {
				menuState = 3;
			}
		}
		if (m_Input->IsEnterPressed() == true) {
			if (menuState == 0) {
				gameState = 1;
			}
			else if (menuState == 1) {

			}
			else if (menuState == 2) {
				gameState = 2;
			}
			else if (menuState == 3) {
				return false;
			}
		}

		if (menuState == 0) {
			pointer->height_in = menuScreen->height_in + 25;
		}
		else if (menuState == 1) {
			pointer->height_in = menuScreen->height_in + 89;
		}
		else if (menuState == 2) {
			pointer->height_in = menuScreen->height_in + 153;
		}
		else if (menuState == 3) {
			pointer->height_in = menuScreen->height_in + 217;
		}

		m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		return true;
	case 1:
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

		m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		return true;
	case 2:
		m_Graphics->m_Camera->SetPosition(0.0f, -4.0f, 0.0f);
		if (m_Input->IsUpPressed() == true) {

		}
		if (m_Input->IsDownPressed() == true) {

		}
		if (m_Input->IsLeftPressed() == true)
		{

		}
		if (m_Input->IsRightPressed() == true)
		{

		}
		return true;
	}
}

bool Game::InitializeMenuScreen()
{
	bool result;
	result = m_Graphics->AddBitmapToPipeline(menuBackground, m_hwnd, L"../Engine/data/mainmenubackground.dds", m_Graphics->GetScreenWidth(), m_Graphics->GetScreenHeight());
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(menuScreen, m_hwnd, L"../Engine/data/mainmenu.dds", 256, 256);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(menuTitle, m_hwnd, L"../Engine/data/title.dds", 1008, 114);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(pointer, m_hwnd, L"../Engine/data/pointer.dds", 19, 14);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	menuScreen->width_in = (m_Graphics->GetScreenWidth() / 2) - 128;
	menuScreen->height_in = (m_Graphics->GetScreenHeight() / 2);
	menuTitle->width_in = (m_Graphics->GetScreenWidth() / 2) - 504;
	menuTitle->height_in = ((m_Graphics->GetScreenHeight() / 2) / 2);
	pointer->width_in = menuScreen->width_in - 25;
	pointer->height_in = menuScreen->height_in + 25;

	return true;
}

bool Game::InitializeMainGame()
{
	bool result;
	mainPlayer = new Car();
	if (!mainPlayer)
	{
		return false;
	}

	result = mainPlayer->Initialize(m_Graphics, m_hwnd, "../Engine/data/c_main.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	m_raceTrack = new RaceTrack();
	if (!m_raceTrack)
	{
		return false;
	}

	result = m_raceTrack->Initialize(m_Graphics, m_hwnd);
	if (!result) {
		return false;
	}

	mainPlayer->SetPosition(-12.0f, 0.0f, 0.0f, 0.0f);
}
