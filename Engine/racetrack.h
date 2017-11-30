#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: racetrack.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RACETRACK_H_
#define _RACETRACK_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "graphicsclass.h"
#include <vector>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
// Class name: RaceTrack
////////////////////////////////////////////////////////////////////////////////
class RaceTrack
{
public:
	RaceTrack();
	RaceTrack(const RaceTrack&);
	~RaceTrack();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, D3DClass* &D3D);
	void Shutdown();
	void Frame();

	//Variables and lists for collision
	D3DXVECTOR3* mesh; //Collision mesh
	std::vector<D3DXVECTOR3> relVertex;
	std::vector<float> noList;

	//Model for graphics
	ModelClass* m_Model;

private:
	//References to system objects
	GraphicsClass* m_Graphics;
	D3DClass* m_D3D;

};

#endif