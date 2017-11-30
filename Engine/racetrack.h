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
	struct CollisionTriangle {
		D3DXVECTOR3 point1;
		D3DXVECTOR3 point2;
		D3DXVECTOR3 point3;
		CollisionTriangle(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3) {
			point1 = v1;
			point2 = v2;
			point3 = v3;
		}
	};
	RaceTrack();
	RaceTrack(const RaceTrack&);
	~RaceTrack();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, D3DClass* &D3D);
	void Shutdown();
	void Frame();
	D3DXVECTOR3* mesh;
	ModelClass* m_Model;
	std::vector<D3DXVECTOR3> relVertex;
	std::vector<float> noList;
	std::vector<RaceTrack::CollisionTriangle> trackGrid;

private:
	GraphicsClass* m_Graphics;
	D3DClass* m_D3D;

};

#endif