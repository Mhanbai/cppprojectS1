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
	bool Frame();
	int gameState;

private:
	int menuState;
	bool InitializeMenuScreen();
	bool InitializeMainGame();
	bool MenuFrame();
	bool GameFrame();
	bool MultiplayerGameFrame();
	bool CameraFrame();
	bool menuWasUpPressed = false;
	bool menuWasDownPressed = false;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	NetworkClass* m_Network;
	RaceTrack* m_raceTrack;
	HWND m_hwnd;
	Car* mainPlayer;
	BitmapClass* pointer;
	BitmapClass* menuTitle;
	BitmapClass* menuScreen;
	BitmapClass* menuBackground;
	TextClass* m_Text;
	time_t gameStart;
	double timeSinceStart;
};

#endif