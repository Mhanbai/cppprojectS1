////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_Light = 0;
	m_Text = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_Terrain = 0;
	m_TerrainShader = 0;
	screenViewMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -4.0f, 4.0f, 1.0f);
	mainGameAssetCount = 0;
	menuScreenOneAssetCount = 0;
	menuScreenTwoAssetCount = 0;
	gameState = 0;
	menuState = 0;
	m_Foliage = 0;
	m_FoliageShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, TextClass* &text, HWND hwnd)
{
	bool result;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_Text = text;

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, screenViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass(m_Text, m_D3D);
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.

	//m_Camera->SetPosition(0.0f, 170.0f, -30.0f);
	//m_Camera->SetRotation(-90.0f, 0.0f, 0.0f);

	m_Camera->SetPosition(0.0f, -850.0f, 0.0f);
	m_Camera->SetRotation(-90.0f, 0.0f, 0.0f);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_D3D->GetDevice(), "../Engine/data/heightmap01.bmp", L"../Engine/data/dirt01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.1f, -0.9f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(10.0f);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the foliage object.
	m_Foliage = new FoliageClass;
	if (!m_Foliage)
	{
		return false;
	}

	// Initialize the foliage object.
	result = m_Foliage->Initialize(m_D3D->GetDevice(), L"../Engine/data/grass.dds", 40000);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the foliage object.", L"Error", MB_OK);
		return false;
	}

	// Create the foliage object.
	m_FoliageShader = new FoliageShaderClass;
	if (!m_FoliageShader)
	{
		return false;
	}

	// Initialize the foliage object.
	result = m_FoliageShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the foliage shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the foliage shader object.
	if (m_FoliageShader)
	{
		m_FoliageShader->Shutdown();
		delete m_FoliageShader;
		m_FoliageShader = 0;
	}

	// Release the foliage object.
	if (m_Foliage)
	{
		m_Foliage->Shutdown();
		delete m_Foliage;
		m_Foliage = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release models used for the main game loop.
	for (int i = 0; i < mainGameAssetCount; i++) {
		if (mainGameAssets[i])
		{
			mainGameAssets[i]->Shutdown();
			delete mainGameAssets[i];
			mainGameAssets[i] = 0;
		}
	}

	// Release bitmaps used for the first menu screen.
	for (int i = 0; i < menuScreenOneAssetCount; i++) {
		if (menuScreenOneAssets[i])
		{
			menuScreenOneAssets[i]->Shutdown();
			delete menuScreenOneAssets[i];
			menuScreenOneAssets[i] = 0;
		}
	}

	// Release bitmaps used for the second menu screen.
	for (int i = 0; i < menuScreenTwoAssetCount; i++) {
		if (menuScreenTwoAssets[i])
		{
			menuScreenTwoAssets[i]->Shutdown();
			delete menuScreenTwoAssets[i];
			menuScreenTwoAssets[i] = 0;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
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
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::AddToPipeline(ModelClass* &model, HWND hwnd, char* modelFilename, WCHAR* textureFilename)
{
	mainGameAssets[mainGameAssetCount] = new ModelClass;
	bool result = mainGameAssets[mainGameAssetCount]->Initialize(m_D3D->GetDevice(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model object.", L"Error", MB_OK);
		return false;
	}

	model = mainGameAssets[mainGameAssetCount];
	mainGameAssetCount++;

	return true;
}

bool GraphicsClass::AddBitmapToPipeline(int screenNo, BitmapClass* &bitmap, HWND hwnd, WCHAR* bitmapFilename, int width, int height)
{
	bool result;

	switch (screenNo) {
	case 0:
		// Initialize the bitmap object.
		menuScreenOneAssets[menuScreenOneAssetCount] = new BitmapClass;
		result = menuScreenOneAssets[menuScreenOneAssetCount]->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, bitmapFilename, width, height);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		bitmap = menuScreenOneAssets[menuScreenOneAssetCount];
		menuScreenOneAssetCount++;
		break;
	case 1:
		// Initialize the bitmap object.
		menuScreenTwoAssets[menuScreenTwoAssetCount] = new BitmapClass;
		result = menuScreenTwoAssets[menuScreenTwoAssetCount]->Initialize(m_D3D->GetDevice(), m_screenWidth, m_screenHeight, bitmapFilename, width, height);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		bitmap = menuScreenTwoAssets[menuScreenTwoAssetCount];
		menuScreenTwoAssetCount++;
		break;
	}

	return true;
}


void GraphicsClass::SetGameState(int gameState_in)
{
	gameState = gameState_in;
}

void GraphicsClass::SetMenuState(int menuState_in)
{
	menuState = menuState_in;
}

int GraphicsClass::GetScreenWidth()
{
	return m_screenWidth;
}

int GraphicsClass::GetScreenHeight()
{
	return m_screenHeight;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render(m_Codex->positionList[0], 4.0f, 31.5f, -10.0f);
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	switch (gameState) {
	case 0:
		RenderMainMenu(worldMatrix, viewMatrix, projectionMatrix, cameraPosition);
		break;
	case 1:
	case 2:
	case 3:
		RenderSkyDome(worldMatrix, viewMatrix, projectionMatrix, cameraPosition);
		// Render the text strings.
		m_D3D->TurnOnAlphaBlending();
		RenderText(1, worldMatrix);


		// Render the terrain buffers.
		m_Terrain->Render(m_D3D->GetDeviceContext());

		// Render the terrain using the terrain shader.
		result = m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTexture());
		if (!result)
		{
			return false;
		}

		for (int i = 0; i < mainGameAssetCount; i++) {
			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			mainGameAssets[i]->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), mainGameAssets[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(),
				m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), mainGameAssets[i]->GetPositionMatrix(), mainGameAssets[i]->GetRotationMatrix(), mainGameAssets[i]->GetTexture());
			if (!result)
			{
				return false;
			}
		}

		// Do the frame processing for the foliage.
		result = m_Foliage->Frame(cameraPosition, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		// Render the foliage.
		m_Foliage->Render(m_D3D->GetDeviceContext());
		result = m_FoliageShader->Render(m_D3D->GetDeviceContext(), m_Foliage->GetVertexCount(), m_Foliage->GetInstanceCount(), viewMatrix, projectionMatrix, m_Foliage->GetTexture());

		m_D3D->TurnOffAlphaBlending();
		break;
	}

	m_D3D->EndScene();

	return true;
}

void GraphicsClass::RenderSkyDome(D3DXMATRIX &worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition)
{
	m_D3D->TurnZBufferOff();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_D3D->TurnOffCulling();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	m_D3D->TurnZBufferOn();
}

bool GraphicsClass::RenderMainMenu(D3DXMATRIX & worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition)
{
	bool result;
	D3DXMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	switch (menuState) {
	case 0:
		for (int i = 0; i < menuScreenOneAssetCount; i++) {
			result = menuScreenOneAssets[i]->Render(m_D3D->GetDeviceContext(), menuScreenOneAssets[i]->width_in, menuScreenOneAssets[i]->height_in);
			if (!result)
			{
				return false;
			}

			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), menuScreenOneAssets[i]->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, menuScreenOneAssets[i]->GetTexture());
			if (!result)
			{
				return false;
			}
		}

		m_Text->RenderNetworkText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		break;
	case 1:
		for (int i = 0; i < menuScreenTwoAssetCount; i++) {
			result = menuScreenTwoAssets[i]->Render(m_D3D->GetDeviceContext(), menuScreenTwoAssets[i]->width_in, menuScreenTwoAssets[i]->height_in);
			if (!result)
			{
				return false;
			}

			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), menuScreenTwoAssets[i]->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, menuScreenTwoAssets[i]->GetTexture());
			if (!result)
			{
				return false;
			}

			RenderText(0, worldMatrix);
		}
		break;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnZBufferOn();

	return true;
}

void GraphicsClass::RenderText(int mode, D3DXMATRIX &worldMatrix)
{
	D3DXMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->TurnZBufferOff();
	if (mode == 0) {
		m_Text->RenderMenuText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		m_Text->RenderNetworkText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	}
	else if (mode == 1) {
		m_Text->RenderDebugText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		m_Text->RenderNetworkText(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	}
	m_D3D->TurnZBufferOn();
}