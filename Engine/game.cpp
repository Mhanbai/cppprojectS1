#include "game.h"

Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	mainPlayer = 0;
	m_raceTrack = 0;
	gameState = 0;
	menuState = 0;
	pointer = 0;
	menuBackground = 0;
	menuTitle = 0;
	menuScreen = 0;
	pointer2 = 0;
	menuBackground2 = 0;
	acceptButton2 = 0;
	backButton2 = 0;
	enterIP2 = 0;

	menuWasUpPressed = false;
	menuWasDownPressed = false;
	menuWasEnterPressed = false;
	menuWasNumPressed = false;
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
	sprintf_s(acceptInputBuffer, "");
	bufferSize = sizeof(acceptInputBuffer);

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

bool Game::Frame(int fpsOutput, int cpuOutput, float timerOutput)
{
	bool result;

	framesPerSec = fpsOutput;
	cpuUsage = cpuOutput;
	totalGameTime += (timerOutput / 1000);

	switch (gameState) {
	case 0: //Call menu logic if gamestate is set to menu
		result = MenuFrame();
		return result;
	case 1: //Call game logic if gamestate is set to game
		result = GameFrame();
		return result;
	case 2: //Call multiplayer logic if gamestate is set to multiplayer mode
		result = MultiplayerGameFrame();
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

	// Set up Menu Screen 1 assets

	result = m_Graphics->AddBitmapToPipeline(0, menuBackground, m_hwnd, L"../Engine/data/mainmenubackground.dds", m_Graphics->GetScreenWidth(), m_Graphics->GetScreenHeight());
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(0, menuScreen, m_hwnd, L"../Engine/data/mainmenu.dds", 256, 256);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of buttons
	menuScreen->width_in = (m_Graphics->GetScreenWidth() / 2) - 128;
	menuScreen->height_in = (m_Graphics->GetScreenHeight() / 2);

	result = m_Graphics->AddBitmapToPipeline(0, menuTitle, m_hwnd, L"../Engine/data/title.dds", 1008, 114);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of title
	menuTitle->width_in = (m_Graphics->GetScreenWidth() / 2) - 504;
	menuTitle->height_in = ((m_Graphics->GetScreenHeight() / 2) / 2);

	result = m_Graphics->AddBitmapToPipeline(0, pointer, m_hwnd, L"../Engine/data/pointer.dds", 19, 14);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of pointer
	pointer->width_in = menuScreen->width_in - 25;
	pointer->height_in = menuScreen->height_in + 25;

	// Set up Menu Screen 2 assets

	result = m_Graphics->AddBitmapToPipeline(1, menuBackground2, m_hwnd, L"../Engine/data/mainmenubackground.dds", m_Graphics->GetScreenWidth(), m_Graphics->GetScreenHeight());
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(1, enterIP2, m_hwnd, L"../Engine/data/enterip.dds", 512, 106);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of input field
	enterIP2->width_in = (m_Graphics->GetScreenWidth() / 2) - 256;
	enterIP2->height_in = (m_Graphics->GetScreenHeight() / 2) - 118;

	result = m_Graphics->AddBitmapToPipeline(1, acceptButton2, m_hwnd, L"../Engine/data/accept.dds", 256, 64);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of accept button
	acceptButton2->width_in = (m_Graphics->GetScreenWidth() / 2) - 128;
	acceptButton2->height_in = (m_Graphics->GetScreenHeight() / 2) + 96;

	result = m_Graphics->AddBitmapToPipeline(1, backButton2, m_hwnd, L"../Engine/data/back.dds", 256, 64);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of back button
	backButton2->width_in = acceptButton2->width_in;
	backButton2->height_in = acceptButton2->height_in + 64;

	result = m_Graphics->AddBitmapToPipeline(1, pointer2, m_hwnd, L"../Engine/data/pointer.dds", 19, 14);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	//Set Initial position of pointer
	pointer2->height_in = backButton2->height_in + 25;

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
	// Controls menustate - i.e. what the user currently has selected for menu screen 1
	if (menuState < 4) {
		if ((m_Input->IsDownPressed() == true) && (menuWasDownPressed == false)) {
			if (menuState < 3) {
				menuState++;
			}
			else {
				menuState = 0;
			}
			menuWasDownPressed = true;
		}
		else if ((m_Input->IsUpPressed() == true) && (menuWasUpPressed == false)) {
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
	} 

	if (menuState >= 4) {
		if ((m_Input->IsDownPressed() == true) && (menuWasDownPressed == false)) {
			if (menuState < 6) {
				menuState++;
			}
			else {
				menuState = 4;
			}
			menuWasDownPressed = true;
		}
		else if ((m_Input->IsUpPressed() == true) && (menuWasUpPressed == false)) {
			if (menuState > 4) {
				menuState--;
			}
			else {
				menuState = 6;
			}
			menuWasUpPressed = true;
		}

		if (m_Input->IsDownPressed() == false) {
			menuWasDownPressed = false;
		}
		if (m_Input->IsUpPressed() == false) {
			menuWasUpPressed = false;
		}
	}

	// Updates menu assets
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
	else if (menuState == 4) {
		pointer2->height_in = enterIP2->height_in + 68;
		pointer2->width_in = enterIP2->width_in - 19;
		MultiplayerSetUoFrame();
	}
	else if (menuState == 5) {
		pointer2->height_in = acceptButton2->height_in + 25;
		pointer2->width_in = acceptButton2->width_in - 19;
	}
	else if (menuState == 6) {
		pointer2->height_in = backButton2->height_in + 25;
		pointer2->width_in = backButton2->width_in - 19;
	}

	// Changes the gamestate based on what the user has selected when enter is pressed
	if ((m_Input->IsEnterPressed() == true) && (menuWasEnterPressed == false)) {

		menuWasEnterPressed = true;

		if (menuState == 0) {
			totalGameTime = 0.0f;
			gameState = 1; // Starts a normal game
			m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		}
		else if (menuState == 1) {
			m_Graphics->SetMenuState(1);
			menuState = 4;
		}
		else if (menuState == 2) {
			m_Graphics->m_Camera->SetPosition(0.0f, -4.0f, 0.0f);
			gameState = 3; // Enters 'scene mode'
			m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
		}
		else if (menuState == 3) {
			return false; // Quits the game
		}
		else if (menuState == 4) {
			//Accept input
			menuState = 5;
		}
		else if (menuState == 5) {
			//Start Multiplayer game

		}
		else if (menuState == 6) {
			// Go back to main menu
			m_Graphics->SetMenuState(0);
			menuState = 1;
		}
	}

	if (m_Input->IsEnterPressed() == false) {
		menuWasEnterPressed = false;
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

	char timeBuffer[32];
	sprintf_s(timeBuffer, "%f", totalGameTime);

	char fpsBuffer[32];
	sprintf_s(fpsBuffer, "FPS: %i", framesPerSec);

	char cpuBuffer[32];
	sprintf_s(cpuBuffer, "CPU: %i%%", cpuUsage);


	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence1, timeBuffer, 60, 50, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, m_Network->myLocalIP, 60, 110, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence5, m_Network->myPublicIP, 60, 130, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence6, fpsBuffer, 60, 150, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence7, cpuBuffer, 60, 170, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	return true;
}

bool Game::MultiplayerGameFrame()
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

	char timeBuffer[32];
	sprintf_s(timeBuffer, "%f", totalGameTime);

	char fpsBuffer[32];
	sprintf_s(fpsBuffer, "FPS: %i", framesPerSec);

	char cpuBuffer[32];
	sprintf_s(cpuBuffer, "CPU: %i%%", cpuUsage);


	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence1, timeBuffer, 60, 50, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence4, m_Network->myLocalIP, 60, 110, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence5, m_Network->myPublicIP, 60, 130, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence6, fpsBuffer, 60, 150, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence7, cpuBuffer, 60, 170, 1.0f, 1.0f, 1.0f, m_Graphics->m_D3D->GetDeviceContext());
	return true;
}

bool Game::MultiplayerSetUoFrame()
{
	if (menuWasNumPressed == false) {
		m_Input->CheckNumKeyPress(*acceptInputBuffer, bufferSize);
		menuWasNumPressed = true;
	} 

	if (m_Input->CheckNumKeyUp() == false) {
		menuWasNumPressed = false;
	}

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->acceptInput, acceptInputBuffer, enterIP2->width_in + 196, pointer2->height_in + 2, 0.0f, 1.0f, 0.0f, m_Graphics->m_D3D->GetDeviceContext());

	char displayIPBuffer[32];
	sprintf_s(displayIPBuffer, "Your IP = %s", m_Network->myPublicIP);
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->displayIP, displayIPBuffer, enterIP2->width_in + 196, enterIP2->height_in + 156, 1.0f, 1.0f, 0.0f, m_Graphics->m_D3D->GetDeviceContext());

	return false;
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
