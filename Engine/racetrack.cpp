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

	m_Model->SetPosition(0.0f, 4.0f, 0.0f);

	mesh = m_Model->GetCollisionMesh();

	D3DXVECTOR4 product;
	D3DXMATRIX worldMatrix;
	m_D3D->GetWorldMatrix(worldMatrix);

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		D3DXVec3Transform(&product, &mesh[i], &worldMatrix);
		mesh[i] = D3DXVECTOR3(product.x, product.y + 4.0f, product.z);
	}

	return true;
}

void RaceTrack::Shutdown()
{
	m_Model = 0;
}
