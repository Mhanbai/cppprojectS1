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

	//Add the racetrack model to the graphics pipeline
	result = m_Graphics->AddToPipeline(m_Model, hwnd, "data/racetrack.txt", L"data/road.dds");
	if (!result) {
		return false;
	}
	
	//Add the finnish line banner model to the graphics pipeline
	result = m_Graphics->AddToPipeline(finishBanner, hwnd, "data/finish.txt", L"data/finish.dds");
	if (!result) {
		return false;
	}

	finishBanner->SetPosition(0.0f, 4.0f, 17.479f);
	finishBanner->SetRotation(90.0f);

	m_Model->SetPosition(0.0f, 4.0f, 0.0f);

	//Get a list of vertex positions from the racetrack model.
	m_Model->GetCollisionMesh(mesh);
	noList.push_back(0.0f);

	//Sort through the list of vertext positions and discard any that have a Y value above 0.0f, 
	//this leaves us with only the bottom vertices, which form a perfect line around each side of the track
	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		if (mesh[i].y < 0.0f) {
			float no = D3DXVec3Length(&mesh[i]);
			if (std::find(noList.begin(), noList.end(), no) == noList.end()) {
				noList.push_back(no);
				relVertex.push_back(D3DXVECTOR3(-mesh[i].x, 0.0f, -mesh[i].z));
			}
		}
	}

	//Use nodes to create checkpoints and the finish line
	CP1 = CheckPoint(relVertex[18], relVertex[16]);
	CP2 = CheckPoint(relVertex[198], relVertex[197]);
	CP3 = CheckPoint(relVertex[73], relVertex[72]);
	FL = CheckPoint(relVertex[140], relVertex[138]);

	return true;
}

void RaceTrack::Shutdown()
{
	m_Model = 0;
}
