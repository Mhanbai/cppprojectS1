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

	result = m_Graphics->AddToPipeline(finishBanner, hwnd, "../Engine/data/finish.txt", L"../Engine/data/finish.dds");
	if (!result) {
		return false;
	}

	finishBanner->SetPosition(0.0f, 4.0f, 17.479f);
	finishBanner->SetRotation(90.0f);

	/*for (int i = 0; i < 8; i++) {
		result = m_Graphics->AddToPipeline(node[i], hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
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

	CP1 = CheckPoint(relVertex[18], relVertex[16]);
	CP2 = CheckPoint(relVertex[198], relVertex[197]);
	CP3 = CheckPoint(relVertex[73], relVertex[72]);
	FL = CheckPoint(relVertex[140], relVertex[138]);

	/*node[0]->SetPosition(FL.point1.x, FL.point1.y, FL.point1.z);
	node[1]->SetPosition(FL.point2.x, FL.point2.y, FL.point2.z);
	node[2]->SetPosition(CP1.point1.x, CP1.point1.y, CP1.point1.z);
	node[3]->SetPosition(CP1.point2.x, CP1.point2.y, CP1.point2.z);
	node[4]->SetPosition(CP2.point1.x, CP2.point1.y, CP2.point1.z);
	node[5]->SetPosition(CP2.point2.x, CP2.point2.y, CP2.point2.z);
	node[6]->SetPosition(CP3.point1.x, CP3.point1.y, CP3.point1.z);
	node[7]->SetPosition(CP3.point2.x, CP3.point2.y, CP3.point2.z);*/

	return true;
}

void RaceTrack::Shutdown()
{
	m_Model = 0;
}

void RaceTrack::Frame()
{

}
