#include "racetrack.h"

RaceTrack::RaceTrack()
{
	m_Model = 0;
}

RaceTrack::RaceTrack(const RaceTrack &)
{
}

RaceTrack::~RaceTrack()
{
}



bool RaceTrack::Initialize(GraphicsClass *& graphics, HWND & hwnd, D3DClass* &D3D)
{
	bool result;
	m_Graphics = graphics;
	m_D3D = D3D;

	result = m_Graphics->AddToPipeline(m_Model, hwnd, "../Engine/data/racetrack.txt", L"../Engine/data/road.dds");
	if (!result) {
		return false;
	}

	/*for (int i = 0; i < 20; i++) {
		result = m_Graphics->AddToPipeline(points[i], hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
		if (!result) {
			return false;
		}
	}*/
									 

	m_Model->SetPosition(0.0f, 4.0f, 0.0f);

	m_Model->GetCollisionMesh(mesh);
	noList.push_back(0.0f);

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		if (mesh[i].y < 0.0f) {
			float no = D3DXVec3Length(&mesh[i]);
			if (std::find(noList.begin(), noList.end(), no) == noList.end()) {
				noList.push_back(no);
				relVertex.push_back(D3DXVECTOR3(-mesh[i].x, 0.0f, -mesh[i].z));
			}
		}
	}

	return true;
}

void RaceTrack::Shutdown()
{
	m_Model = 0;
}

void RaceTrack::Frame()
{

}
