#include "modelcodex.h"

ModelCodex::ModelCodex()
{
	modelList[0] = new ModelClass;
	modelList[1] = new ModelClass;
	modelList[2] = new ModelClass;
	modelList[3] = new ModelClass;

	modelList[1]->SetPosition(30.0f, 0.0f, 0.0f);
	modelList[1]->SetRotation(0.0f);
	modelList[2]->SetPosition(60.0f, 0.0f, 0.0f);
	modelList[2]->SetRotation(0.0f);
	modelList[3]->SetPosition(90.0f, 0.0f, 0.0f);
	modelList[3]->SetRotation(0.0f);
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
	result = modelList[0]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/Car1Texture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the main Player object.", L"Error", MB_OK);
		return false;
	}

	result = modelList[1]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/Car2Texture.dds");
	if (!result)																					 
	{																								 
		MessageBox(hwnd, L"Could not initialize the opponent 1 object.", L"Error", MB_OK);			 
		return false;																				 
	}																								 
																									 
	result = modelList[2]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/Car3Texture.dds");
	if (!result)																					 
	{																								 
		MessageBox(hwnd, L"Could not initialize the opponent 2 object.", L"Error", MB_OK);			 
		return false;																				 
	}																								 
																									 
	result = modelList[3]->Initialize(m_D3D->GetDevice(), "../Engine/data/car.txt", L"../Engine/data/Car4Texture.dds");
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

}

D3DXMATRIX ModelCodex::GetModelRotationMatrix(float rotation) {
	D3DXMATRIX toReturn;
	D3DXMatrixRotationY(&toReturn, rotation * 0.0174532925f);
	return toReturn;
}
