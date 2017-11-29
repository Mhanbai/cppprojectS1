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
		CollisionTriangle(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c) {
			point1 = a;
			point2 = b;
			point3 = c;
		}
	};

	RaceTrack();
	RaceTrack(const RaceTrack&);
	~RaceTrack();
	bool Initialize(GraphicsClass* &graphics, HWND &hwnd, D3DClass* &D3D);
	void Shutdown();
	D3DXVECTOR3* mesh;
	ModelClass* m_Model;
	std::vector<D3DXVECTOR3> relVertex;
	std::vector<RaceTrack::CollisionTriangle> collider;
	void ViewCollider(int x);

private:
	GraphicsClass* m_Graphics;
	D3DClass* m_D3D;
	ModelClass* point1;
	ModelClass* point2;
	ModelClass* point3;

};

#endif