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

////////////////////////////////////////////////////////////////////////////////
// Class name: Game
////////////////////////////////////////////////////////////////////////////////
class Game
{
public:
	Game();
	Game(const Game&);
	~Game();
	bool Initialize(InputClass* &input, GraphicsClass* &graphics, TextClass* &text, HWND &hwnd);
	void Shutdown();
	bool Frame();
	int gameState = 0;

private:
	int menuState = 0;
	bool InitializeMenuScreen();
	bool InitializeMainGame();
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	RaceTrack* m_raceTrack;
	HWND m_hwnd;
	Car* mainPlayer;
	BitmapClass* pointer;
	BitmapClass* menuTitle;
	BitmapClass* menuScreen;
	BitmapClass* menuBackground;
	TextClass* m_Text;
};

#endif