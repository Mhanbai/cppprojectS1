////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	modelList = 0;
	modelCount = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.

	//m_Camera->SetPosition(0.0f, 170.0f, -30.0f);
	//m_Camera->SetRotation(-90.0f, 0.0f, 0.0f);

	m_Camera->SetPosition(0.0f, 7.0f, 10.0f);
	m_Camera->SetRotation(-10.0f, 0.0f, 0.0f);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Create dynamic array of models
	modelList = new ModelClass*[10];

	// Initialize the light object.
	m_Light->SetAmbientColor(0.4f, 0.4f, 0.4f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(10.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model objects.
	if (modelList) {
		for (int i = 0; i < modelCount; i++) {
			if (modelList[i])
			{
				modelList[i]->Shutdown();
				delete modelList[i];
				modelList[i] = 0;
			}
		}

		// Release the model list
		delete modelList;
		modelList = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	
	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::AddToPipeline(ModelClass* &model, HWND hwnd, char* modelFilename, WCHAR* textureFilename)
{
	modelList[modelCount] = new ModelClass;
	bool result = modelList[modelCount]->Initialize(m_D3D->GetDevice(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model object.", L"Error", MB_OK);
		return false;
	}

	model = modelList[modelCount];
	modelCount++;

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	D3DXVECTOR3 test = m_Camera->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render(m_Codex->positionList[0], 4.0f, 31.5f, -10.0f);
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	for (int i = 0; i < modelCount; i++) {
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		modelList[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), modelList[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), modelList[i]->GetPositionMatrix(), modelList[i]->GetRotationMatrix(), modelList[i]->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	m_D3D->EndScene();

	return true;
}