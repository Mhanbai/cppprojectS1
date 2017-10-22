#include "modelcodex.h"

ModelCodex::ModelCodex()
{
	modelList[0] = new ModelClass;
	modelList[1] = new ModelClass;
	modelList[2] = new ModelClass;
	modelList[3] = new ModelClass;
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
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[1]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[2]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[3]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/RaceC_red_diffuse.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
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
