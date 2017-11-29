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
#include "opponent.h"
#include "racetrack.h"
#include "networkclass.h"
#include "soundclass.h"
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
	bool Initialize(InputClass* &input, GraphicsClass* &graphics, NetworkClass* &network, SoundClass* &sound, bool connected, TextClass* &text, HWND &hwnd, D3DClass* &d3d);
	void Shutdown();
	void SetOnlineMode(bool mode);
	bool Frame(int fpsOutput, int cpuOutput, float timerOutput);
	int gameState;

private:
	int menuState;
	bool InitializeMenuScreen();
	bool InitializeMainGame(bool multiplayer);
	bool MenuFrame();
	bool GameFrame();
	bool MultiplayerSetUpFrame();
	bool menuWasUpPressed;
	bool menuWasDownPressed;
	bool menuWasEnterPressed;
	bool menuWasNumPressed;
	int framesPerSec;
	int cpuUsage;
	float totalGameTime;
	float deltaTime;

	//Values for countdown
	bool gameStarted = false;
	bool three = false;
	bool two = false;
	bool one = false;

	bool onlineMode;

	// References to System Objects
	D3DClass* m_D3D;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	NetworkClass* m_Network;
	SoundClass* m_Sound;
	TextClass* m_Text;
	HWND m_hwnd;

	// References to Main Game Assets
	Car* mainPlayer;
	Opponent* opponent;
	RaceTrack* m_raceTrack;

	// References to Menu Screen One Assets
	BitmapClass* pointer;
	BitmapClass* menuBackground;
	BitmapClass* menuTitle;
	BitmapClass* menuScreen;
	BitmapClass* threeDisplay;
	BitmapClass* twoDisplay;
	BitmapClass* oneDisplay;
	BitmapClass* goDisplay;

	// References to Menu Screen Two Assets
	BitmapClass* pointer2;
	BitmapClass* menuBackground2;
	BitmapClass* acceptButton2;
	BitmapClass* backButton2;
	BitmapClass* enterIP2;
	char acceptInputBuffer[32];
	unsigned long bufferSize;
	bool typing;

	//Debug
	int x = 0;
	float lastDisplayTime = 0.0f;
};

#endif