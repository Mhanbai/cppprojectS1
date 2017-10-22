#include "modelcodex.h"

ModelCodex::ModelCodex()
{
	modelList[0] = new ModelClass;
	positionList[0] = new D3DXVECTOR4(-10.0f, 0.0f, 0.0f, 0.0f);
	modelList[1] = new ModelClass;
	positionList[1] = new D3DXVECTOR4(-5.0f, 0.0f, 0.0f, 1.0f);
	modelList[2] = new ModelClass;
	positionList[2] = new D3DXVECTOR4(0.0f, 0.0f, 0.0f, 2.0f);
	modelList[3] = new ModelClass;
	positionList[3] = new D3DXVECTOR4(5.0f, 0.0f, 0.0f, 3.0f);
}

ModelCodex::ModelCodex(const ModelCodex& other)
{
}


ModelCodex::~ModelCodex()
{
}

bool ModelCodex::Initialize(D3DClass* m_D3D, HWND hwnd)
{
	bool result;

	// Initialize the model object.
	result = modelList[0]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the main Player object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[1]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the opponent 1 object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[2]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the opponent 2 object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[3]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the opponent 3 object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ModelCodex::Shutdown()
{
	for (ModelClass* m : modelList) {
		if (m)
		{
			m->Shutdown();
			delete m;
			m = 0;
		}
	}
}

void ModelCodex::Frame()
{
	for (int i = 0; i < modelCount; i++) {
		modelList[i]->pos.x = positionList[i]->x;
		modelList[i]->pos.y = positionList[i]->y;
		modelList[i]->pos.z = positionList[i]->z;
	}
}
