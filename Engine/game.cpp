#include "game.h"

Game::Game()
{
	m_Input = 0;
	m_Graphics = 0;
	mainPlayer = 0;
	opponent = 0;
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
	threeDisplay = 0;
	twoDisplay = 0;
	oneDisplay = 0;
	goDisplay = 0;

	typing = false;
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

bool Game::Initialize(InputClass* &input, GraphicsClass* &graphics, NetworkClass* &network, SoundClass* &sound, bool connected, TextClass* &text, HWND &hwnd, D3DClass* &d3d)
{
	bool result;
	m_D3D = d3d;
	m_Input = input;
	m_Graphics = graphics;
	m_hwnd = hwnd;
	m_Network = network;
	m_Sound = sound;
	m_Text = text;
	sprintf_s(acceptInputBuffer, "");
	bufferSize = sizeof(acceptInputBuffer);
	onlineMode = connected;

	m_Graphics->SetGameState(gameState);
	result = InitializeMenuScreen();
	if (!result) {
		MessageBox(m_hwnd, L"Could not initialize menu screen.", L"Error", MB_OK);
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

	if (opponent) {
		opponent->Shutdown();
		delete opponent;
		opponent = 0;
	}

}

void Game::SetOnlineMode(bool mode)
{
	onlineMode = mode;
}

bool Game::Frame(int fpsOutput, int cpuOutput, float timerOutput)
{
	bool result;

	framesPerSec = fpsOutput;
	cpuUsage = cpuOutput;
	totalGameTime += (timerOutput / 1000);
	deltaTime = timerOutput;

	switch (gameState) {
	case 0: //Call menu logic if gamestate is set to menu
		result = MenuFrame();
		return result;
	case 1: //Call game logic if gamestate is set to game
		result = GameFrame();
		return result;
	case 2: //Call multiplayer logic if gamestate is set to multiplayer mode
		result = GameFrame();
		opponent->Frame(deltaTime / 1000, totalGameTime);
		return result;
	default:
		return false;
	}
}

bool Game::InitializeMenuScreen()
{
	bool result;

	m_Sound->LoopSound("../Engine/data/menumusic.wav");

	// Set up Menu Screen 1 assets

	result = m_Graphics->AddBitmapToPipeline(0, menuBackground, m_hwnd, L"../Engine/data/mainmenubackground.dds", m_Graphics->GetScreenWidth(), m_Graphics->GetScreenHeight());
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	result = m_Graphics->AddBitmapToPipeline(0, menuScreen, m_hwnd, L"../Engine/data/mainmenu.dds", 256, 192);
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

	result = m_Graphics->AddBitmapToPipeline(2, threeDisplay, m_hwnd, L"../Engine/data/three.dds", 79, 110);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	threeDisplay->width_in = (m_Graphics->GetScreenWidth() / 2) - 40;
	threeDisplay->height_in = (m_Graphics->GetScreenHeight() / 2) - 55;

	result = m_Graphics->AddBitmapToPipeline(2, twoDisplay, m_hwnd, L"../Engine/data/two.dds", 77, 110);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	twoDisplay->width_in = (m_Graphics->GetScreenWidth() / 2) - 38;
	twoDisplay->height_in = (m_Graphics->GetScreenHeight() / 2) - 55;

	result = m_Graphics->AddBitmapToPipeline(2, oneDisplay, m_hwnd, L"../Engine/data/one.dds", 69, 110);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	oneDisplay->width_in = (m_Graphics->GetScreenWidth() / 2) - 35;
	oneDisplay->height_in = (m_Graphics->GetScreenHeight() / 2) - 55;

	result = m_Graphics->AddBitmapToPipeline(2, goDisplay, m_hwnd, L"../Engine/data/go.dds", 193, 110);
	if (!result) {
		MessageBox(m_hwnd, L"Could not add bitmap to pipeline.", L"Error", MB_OK);
		return false;
	}

	goDisplay->width_in = (m_Graphics->GetScreenWidth() / 2) - 96;
	goDisplay->height_in = (m_Graphics->GetScreenHeight() / 2) - 55;

	return true;
}

bool Game::InitializeMainGame(bool multiplayer)
{
	bool result;
	mainPlayer = new Car();
	if (!mainPlayer)
	{
		return false;
	}

	result = mainPlayer->Initialize(m_Graphics, m_Sound, m_hwnd, m_Network, "../Engine/data/c_main.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	if (multiplayer == false) {
		mainPlayer->SetPosition(-12.0f, 2.0f, 0.0f, 0.0f);
	}
	else {
		opponent = new Opponent();
		if (!opponent)
		{
			return false;
		}

		opponent->Initialize(m_Graphics, m_hwnd, m_Network, m_Text, "../Engine/data/c_main.txt", L"../Engine/data/cars.dds");
		if (!result) {
			return false;
		}

		if (m_Network->trackPosition == 0) {
			mainPlayer->SetPosition(-12.0f, 2.0f, 0.0f, 0.0f);
			opponent->SetPosition(-12.0f, 2.0f, 0.0f, 0.0f);
		}
		else if (m_Network->trackPosition == 1) {
			mainPlayer->SetPosition(-12.0f, 2.0f, 0.0f, 0.0f);
			opponent->SetPosition(-12.0f, 2.0f, 0.0f, 0.0f);
		}
	}

	m_raceTrack = new RaceTrack();
	if (!m_raceTrack)
	{
		return false;
	}

	result = m_raceTrack->Initialize(m_Graphics, m_hwnd, m_D3D);
	if (!result) {
		return false;
	}

	return true;
}

bool Game::MenuFrame()
{
	////////////////////////////////////////////////////////////////////////////////////
	// ARROW KEY CONTROL //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	if (menuState < 3) {
		if ((m_Input->IsDownPressed() == true) && (menuWasDownPressed == false)) {
			m_Sound->PlaySoundOnce("../Engine/data/cursor.wav");
			if (menuState != 2) {
				menuState++;
			}
			else {
				menuState = 0;
			}
			menuWasDownPressed = true;
		}
		else if ((m_Input->IsUpPressed() == true) && (menuWasUpPressed == false)) {
			m_Sound->PlaySoundOnce("../Engine/data/cursor.wav");
			if (menuState != 0) {
				menuState--;
			}
			else {
				menuState = 2;
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

	if ((menuState >= 3) && (menuState < 6)) {
		if ((m_Input->IsDownPressed() == true) && (menuWasDownPressed == false)) {
			m_Sound->PlaySoundOnce("../Engine/data/cursor.wav");
			if (menuState != 5) {
				menuState++;
			}
			else {
				menuState = 3;
			}
			menuWasDownPressed = true;
		}
		else if ((m_Input->IsUpPressed() == true) && (menuWasUpPressed == false)) {
			m_Sound->PlaySoundOnce("../Engine/data/cursor.wav");
			if (menuState != 3) {
				menuState--;
			}
			else {
				menuState = 5;
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

	////////////////////////////////////////////////////////////////////////////////////
	// UPDATE MENU ASSETS /////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	switch (menuState) {
	case 0:
		pointer->height_in = menuScreen->height_in + 25;
		break;
	case 1:
		pointer->height_in = menuScreen->height_in + 89;
		break;
	case 2:
		pointer->height_in = menuScreen->height_in + 153;
		break;
	case 3:
		pointer2->height_in = enterIP2->height_in + 68;
		pointer2->width_in = enterIP2->width_in - 19;
		MultiplayerSetUpFrame();
		break;
	case 4:
		pointer2->height_in = acceptButton2->height_in + 25;
		pointer2->width_in = acceptButton2->width_in - 19;
		break;
	case 5:
		pointer2->height_in = backButton2->height_in + 25;
		pointer2->width_in = backButton2->width_in - 19;
		break;
	default:
		break;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// BUTTON FUNCTIONALITY ON ENTER PRESS/////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	// Changes the gamestate based on what the user has selected when enter is pressed
	if ((m_Input->IsEnterPressed() == true) && (menuWasEnterPressed == false)) {
		menuWasEnterPressed = true;
		bool result;

		switch (menuState) {
		case 0:
			m_Sound->PlaySoundOnce("../Engine/data/select.wav");
			m_Sound->StopLooping();
			m_Sound->LoopSound("../Engine/data/wind.wav");
			m_Sound->PlaySoundOnce("../Engine/data/carstart.wav");
			totalGameTime = 0.0f;
			result = InitializeMainGame(false);
			if (!result) {
				MessageBox(m_hwnd, L"Could not initialize gameplay scene.", L"Error", MB_OK);
				return false;
			}
			gameState = 1; // Starts a normal game
			m_Graphics->SetGameState(gameState); //Ensure the correct graphics are rendering for the game state
			break;
		case 1:
			m_Sound->PlaySoundOnce("../Engine/data/select.wav");
			m_Graphics->SetMenuState(1);
			menuState = 3;
			break;
		case 2:
			m_Sound->PlaySoundOnce("../Engine/data/menuback.wav");
			return false; // Quits the game
		case 3:
			m_Sound->PlaySoundOnce("../Engine/data/select.wav");
			menuState = 4;
			break;
		case 4:
			m_Sound->PlaySoundOnce("../Engine/data/select.wav");
			m_Network->EstablishConnection(acceptInputBuffer);
			menuState = 6;
			break;
		case 5:
			// Go back to main menu
			m_Sound->PlaySoundOnce("../Engine/data/menuback.wav");
			m_Graphics->SetMenuState(0);
			menuState = 1;
			break;
		case 6:
			m_Sound->PlaySoundOnce("../Engine/data/menuback.wav");
			m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->networkStatus, "Connection attempt cancelled", 10, 10, 1.0f, 1.0f, 1.0f);
			m_Network->establishingConnection = false;
			menuState = 4;
			break;
		}
	}

	if (m_Input->IsEnterPressed() == false) {
		menuWasEnterPressed = false;
	}

	if ((menuState == 6) && (m_Network->connectionEstablished == true)) {
		bool result = InitializeMainGame(true);
		if (!result) {
			MessageBox(m_hwnd, L"Could not initialize gameplay scene.", L"Error", MB_OK);
			return false;
		}
		m_Sound->StopLooping();
		gameState = 2;
		totalGameTime = 0.0f;
		m_Sound->PlaySoundOnce("../Engine/data/carstart.wav");
		m_Sound->LoopSound("../Engine/data/wind.wav");
		m_Graphics->SetGameState(gameState);
	}

	return true;
}

bool Game::GameFrame()
{
	mainPlayer->Frame(deltaTime / 1000, totalGameTime);
	m_Graphics->m_Camera->Follow(mainPlayer->GetPosition(), mainPlayer->GetForwardVector(), deltaTime / 1000);

	if ((totalGameTime - lastDisplayTime) > 1.0f) {
		lastDisplayTime = totalGameTime;
		if (x < 641) {
			x++;
		}
		else {
			x = 0;
		}
		m_raceTrack->ViewCollider(x);
	}

	if (gameStarted == false) {
		if ((totalGameTime >= 1.0f) && (totalGameTime < 2.0f)) {
			if (three == false) {
				three = true;
				m_Sound->PlaySoundOnce("../Engine/data/321.wav");
				m_Graphics->countdown = 0;
			}
		}
		else if ((totalGameTime >= 2.0f) && (totalGameTime < 3.0f)) {
			if (two == false) {
				two = true;
				m_Sound->PlaySoundOnce("../Engine/data/321.wav");
				m_Graphics->countdown = 1;
			}
		}
		else if ((totalGameTime >= 3.0f) && (totalGameTime < 4.0f)) {
			if (one == false) {
				one = true;
				m_Sound->PlaySoundOnce("../Engine/data/321.wav");
				m_Graphics->countdown = 2;
			}
		}
		else if (totalGameTime >= 4.0f){
			m_Graphics->countdown = 3;
			totalGameTime = 0.0f;
			m_Sound->PlaySoundOnce("../Engine/data/go.wav");
			gameStarted = true;
		}
	}

	if (totalGameTime > 1.0f) {
		m_Graphics->countdown = -1;
	}

	if (gameStarted == true) {
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

		char fpsBuffer[32];
		sprintf_s(fpsBuffer, "FPS: %i", framesPerSec);

		char cpuBuffer[32];
		sprintf_s(cpuBuffer, "CPU: %i%%", cpuUsage);

		m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence2, fpsBuffer, 60, 90, 1.0f, 1.0f, 1.0f);
		m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence3, cpuBuffer, 60, 110, 1.0f, 1.0f, 1.0f);
	}

	char timeBuffer[32];
	sprintf_s(timeBuffer, "%.0f:%.2f", floor((totalGameTime / 60)), fmod(totalGameTime, 60));
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->m_sentence1, timeBuffer, 60, 70, 1.0f, 1.0f, 1.0f);

	return true;
}

bool Game::MultiplayerSetUpFrame()
{
	if (menuWasNumPressed == false) {
		m_Input->CheckNumKeyPress(*acceptInputBuffer, bufferSize);
		menuWasNumPressed = true;
	}

	if (m_Input->CheckNumKeyUp() == false) {
		menuWasNumPressed = false;
	}

	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->acceptInput, acceptInputBuffer, enterIP2->width_in + 216, pointer2->height_in + 2, 0.0f, 1.0f, 0.0f);

	char displayLocalIPBuffer[32];
	sprintf_s(displayLocalIPBuffer, "Your Local IP = %s", m_Network->myLocalIP);
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->displayLocalIP, displayLocalIPBuffer, enterIP2->width_in + 196, enterIP2->height_in + 156, 1.0f, 1.0f, 0.0f);

	char displayPublicIPBuffer[32];
	sprintf_s(displayPublicIPBuffer, "Your Public IP = %s", m_Network->myPublicIP);
	m_Graphics->m_Text->UpdateSentence(m_Graphics->m_Text->displayPublicIP, displayPublicIPBuffer, enterIP2->width_in + 181, enterIP2->height_in + 176, 1.0f, 1.0f, 0.0f);


	return false;
}
