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

bool Game::Initialize(InputClass* &input, GraphicsClass* &graphics, NetworkClass* &network, TextClass* &text, HWND &hwnd)
{
	bool result;
	m_Input = input;
	m_Graphics = graphics;
	m_Text = text;
	m_hwnd = hwnd;
	m_Network = network;

	m_Graphics->SetGameState(gameState);
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
	bool result;
	switch (gameState) {
	case 0: //Call menu logic if gamestate is set to menu
		result = MenuFrame();
		return result;
	case 1: //Call game logic if gamestate is set to game
		result = GameFrame();
		return result;
	case 2: //Call multiplayer logic if gamestate is set to multiplayer mode
		result = GameFrame();
		return result;
	case 3: //Call camera logic if gamestate is set to camera mode
		result = CameraFrame();
		return result;
	default:
		return false;
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

	return true;
}

bool Game::MenuFrame()
{
	// Controls menustate - i.e. what the user currently has selected
	if ((m_Input->IsDownPressed() == true) && (menuWasDownPressed == false)) {
		if (menuState < 3) {
			menuState++;
		}
		else {
			menuState = 0;
		}
		menuWasDownPressed = true;
	} else if ((m_Input->IsUpPressed() == true) && (menuWasUpPressed == false)) {
		if (menuState > 0) {
			menuState--;
		}
		else {
			menuState = 3;
		}
		menuWasUpPressed = true;
	}

	if (m_Input->IsDownPressed() == false) {
		menuWasDownPressed = false;
	}
	if (m_Input->IsUpPressed() == false) {
		menuWasUpPressed = false;
	}

	// Updates the pointer
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

	// Changes the gamestate based on what the user has selected when enter is pressed
	if (m_Input->IsEnterPressed() == true) {
		if (menuState == 0) {
			time(&gameStart);
			gameState = 1; // Starts a normal game
			m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		}
		else if (menuState == 1) {
			//TODO: Starts a multiplayer game
			//Ensure the correct graphics are rendering for the game state
		}
		else if (menuState == 2) {
			m_Graphics->m_Camera->SetPosition(0.0f, -4.0f, 0.0f);
			gameState = 2; // Enters 'scene mode'
			m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		}
		else if (menuState == 3) {
			return false; // Quits the game
		}
	}

	return true;
}

bool Game::GameFrame()
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

	timeSinceStart = difftime(time(NULL), gameStart);
	char timeBuffer[20];
	sprintf_s(timeBuffer, "%f", timeSinceStart);
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence1, timeBuffer, 60, 50, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, m_Network->FindIP(), 60, 110, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	return true;
}

bool Game::CameraFrame()
{
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
