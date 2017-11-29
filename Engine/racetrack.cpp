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

	m_Model->GetCollisionMesh(mesh);

	D3DXVECTOR4 product;
	D3DXMATRIX worldMatrix;
	m_Graphics->m_D3D->GetWorldMatrix(worldMatrix);

	for (int i = 0; i < m_Model->GetVertexCount(); i++) {
		D3DXVec3Transform(&product, &mesh[i], &worldMatrix);
		mesh[i] = D3DXVECTOR3(product.x + 419.0f, product.y, product.z + 97.0f);
		if (mesh[i].y < 0.0f) {
			relVertex.push_back(mesh[i]);
		}


	}

	for (unsigned int i = 0; i < relVertex.size() - 3; i = i + 3) {
		collider.push_back(CollisionTriangle(relVertex[i], relVertex[i + 1], relVertex[i + 2]));
	}

	

	result = m_Graphics->AddToPipeline(point1, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point2, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point3, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point4, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point5, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point6, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point7, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point8, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point9, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point10, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point11, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	result = m_Graphics->AddToPipeline(point12, hwnd, "../Engine/data/skysphere.txt", L"../Engine/data/cars.dds");
	if (!result) {
		return false;
	}

	return true;
}

void RaceTrack::Shutdown()
{
	m_Model = 0;
}

void RaceTrack::ViewCollider(int x)
{
	point1->SetPosition(collider[x].point1.x, collider[x].point1.y, collider[x].point1.z);
	point2->SetPosition(collider[x].point2.x, collider[x].point2.y, collider[x].point2.z);
	point3->SetPosition(collider[x].point3.x, collider[x].point3.y, collider[x].point3.z);
	point4->SetPosition(collider[x+1].point1.x, collider[x+1].point1.y, collider[x+1].point1.z);
	point5->SetPosition(collider[x+1].point2.x, collider[x+1].point2.y, collider[x+1].point2.z);
	point6->SetPosition(collider[x+1].point3.x, collider[x+1].point3.y, collider[x+1].point3.z);
	point7->SetPosition(collider[x+2].point1.x, collider[x+2].point1.y, collider[x+2].point1.z);
	point8->SetPosition(collider[x+2].point2.x, collider[x+2].point2.y, collider[x+2].point2.z);
	point9->SetPosition(collider[x+2].point3.x, collider[x+2].point3.y, collider[x+2].point3.z);
	point10->SetPosition(collider[x+3].point1.x, collider[x+3].point1.y, collider[x+3].point1.z);
	point11->SetPosition(collider[x+3].point2.x, collider[x+3].point2.y, collider[x+3].point2.z);
	point12->SetPosition(collider[x+3].point3.x, collider[x+3].point3.y, collider[x+3].point3.z);
}
