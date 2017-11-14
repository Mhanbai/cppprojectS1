#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: game.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"
#include "car.h"
#include "racetrack.h"
#include "networkclass.h"
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Game
////////////////////////////////////////////////////////////////////////////////
class Game
{
public:
	Game();
	Game(const Game&);
	~Game();
	bool Initialize(InputClass* &input, GraphicsClass* &graphics, NetworkClass* &network, TextClass* &text, HWND &hwnd);
	void Shutdown();
	bool Frame(int fpsOutput, int cpuOutput, float timerOutput);
	int gameState;

private:
	int menuState;
	bool InitializeMenuScreen();
	bool InitializeMainGame();
	bool MenuFrame();
	bool GameFrame();
	bool MultiplayerGameFrame();
	bool MultiplayerSetUoFrame();
	bool CameraFrame();
	bool menuWasUpPressed;
	bool menuWasDownPressed;
	bool menuWasEnterPressed;
	int framesPerSec;
	int cpuUsage;
	float totalGameTime;

	// References to System Objects
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	NetworkClass* m_Network;
	TextClass* m_Text;
	HWND m_hwnd;

	// References to Main Game Assets
	Car* mainPlayer;
	RaceTrack* m_raceTrack;

	// References to Menu Screen One Assets
	BitmapClass* pointer;
	BitmapClass* menuBackground;
	BitmapClass* menuTitle;
	BitmapClass* menuScreen;

	// References to Menu Screen Two Assets
	BitmapClass* pointer2;
	BitmapClass* menuBackground2;
	BitmapClass* acceptButton2;
	BitmapClass* backButton2;
	BitmapClass* enterIP2;
	char acceptInputBuffer[32];
};

#endif