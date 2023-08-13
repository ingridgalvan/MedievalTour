////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	
	
	resetGame = false;
	m_TextureShader = 0;


}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	
	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	// se inicializa la camara 
	m_Camera->SetPosition(0.0f, 5.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_baseStaticMatrix = baseViewMatrix;

	// Set the initial position of the camera.
	//posicion
	cameraX = 137.0f;
	cameraY = 5.0f;
	cameraZ = 0.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	//terreno 
	int textureCount = 5;
	WCHAR** filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/grass.jpg";
	filenames[1] = L"../Engine/data/soil.jpg";
	filenames[2] = L"../Engine/data/rock.jpg";
	filenames[3] = L"../Engine/data/snow.jpg";
	filenames[4] = L"../Engine/data/alpha.jpg";

	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/Heightmapuwu.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Water = new TerrainClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the terrain object.
	//awa de uwu
	textureCount = 3;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/waterDiffuse.jpg";
	filenames[1] = L"../Engine/data/waterNormal.png";
	filenames[2] = L"../Engine/data/waterSpecular.png";

	result = m_Water->Initialize(m_Direct3D->GetDevice(), "../Engine/data/waterHeightmap.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	// skydome shader del skydome 
	m_Skydome = new SkydomeClass;
	if (!m_Skydome)
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/skydome/Skydome01.jpg";
	filenames[1] = L"../Engine/data/skydome/Skydome02.jpg";

	// Initialize the model object.
	result = m_Skydome->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[0] = new BillboardClass;
	if (!m_Billboard[0])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_1.png";
	filenames[1] = L"../Engine/data/bush/Bush_1Op.png";

	// Initialize the model object.
	result = m_Billboard[0]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[1] = new BillboardClass;
	if (!m_Billboard[1])
	{
		return false;
	}
	//billbosrds
	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_2.png";
	filenames[1] = L"../Engine/data/bush/Bush_2Op.png";

	// Initialize the model object.
	result = m_Billboard[1]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[2] = new BillboardClass;
	if (!m_Billboard[2])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_3.png";
	filenames[1] = L"../Engine/data/bush/Bush_3Op.png";

	// Initialize the model object.
	result = m_Billboard[2]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);
	
	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}
	//checa si esta creado y sino regresa false si no lo carga correctamente lo regresa un msg 
	// Create the terrain shader object.
	m_SkydomeShader = new SkydomeShaderClass;
	if (!m_SkydomeShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_SkydomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_BillboardShader = new BillboardShaderClass;
	if (!m_BillboardShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_BillboardShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_GroupShader = new GroupShaderClass;
	if (!m_GroupShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_GroupShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_DiffuseShader = new DiffuseShaderClass;
	if (!m_DiffuseShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_DiffuseShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
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
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	
	textureCount = 1;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/waterDiffuse.jpg";

	

	// 01
	//cambiar las luces y poner variables 
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	
	m_Cup = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cup", "cup.obj");
	m_Principal = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/bike", "Bike.obj");
	
	m_car01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cars/01", "fCars.obj");
	m_car02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cars/02", "fCars01.obj");
	m_building01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/casa1", "casa1.obj");
	m_building01_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/casa2", "casa2.obj");
	m_building02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/casa3", "casa3.obj");
	m_building03 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/casa6", "casa6.obj");
	m_building04 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/Boat", "boat.obj");
	m_building04_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/Tree02", "Tree.obj");
	m_building05 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/pozo", "pozo.obj");
	m_building06 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/Tree02", "Tree.obj");
	m_building06_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/Tree02", "Tree.obj");
	m_building06_2 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/casa4", "casa4.obj");
	m_building06A = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/casas/bike", "Bike.obj");
	m_floor01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/floor/01", "floor.obj");
	m_floor02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/floor/02", "floor02.obj");
	
	
	

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	globalSphere = new BoundingSphere(D3DXVECTOR3(500.0f, 0.0f, 500.0f), 1000.0f);
	
	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if(!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result, type;
	//auto son el carrito 
	float posX, posY, posZ, rotX, rotY, rotZ, AuposX, AuposY, AuposZ;

	int mouseX, mouseY;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsLeftPressed(); //A
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed(); //D
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed(); //W
	m_Position->MoveForward(keyDown);
	/*m_Position->MoveUpward(keyDown)*/;

	keyDown = m_Input->IsDownPressed(); //S
	m_Position->MoveBackward(keyDown);
	/*m_Position->MoveDownward(keyDown)*/;

	keyDown = m_Input->IsResetPressed();
	resetGame = keyDown;

	keyDown = m_Input->IsXPressed();
	//m_Position->MoveUpward(keyDown);
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsZPressed();
	//m_Position->MoveDownward(keyDown);
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);
	
	keyDown = m_Input->IsCPressed();
	m_Position->ChangeCameraType(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	m_Position->GetCameraType(type);
	m_Position->GetPosition(AuposX, AuposY, AuposZ);



	m_Input->GetMouseLocation(mouseX, mouseY);

	// Set the position of the camera.
	m_Terrain->GetHeightAtPosition(AuposX, AuposZ, AuposY);

	if (posY <= AuposY) {
		m_Camera->SetPosition(posX, AuposY+7, posZ);
	}
	else {
		m_Camera->SetPosition(posX, posY+7, posZ);
	}
	//m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(mouseY, mouseX, 0);
	m_Camera->SetCameraType(type);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}


	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	D3DXVECTOR3 cameraPosition;
	cameraPosition = m_Camera->GetPosition();
	//movimientos del Skydome
	// se lo asigno 
	D3DXMATRIX SkyScale, SkyTrans, SkyDomeMatrix;
	D3DXMatrixScaling(&SkyScale, 1, 1, 1);
	//D3DXMatrixRotationAxis(&SkyRot, &D3DXVECTOR3(1, 1, 0), rotation * 0.5);
	D3DXMatrixTranslation(&SkyTrans, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	SkyDomeMatrix = SkyScale * SkyTrans;
	m_Skydome->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	
	//02 
	result = m_SkydomeShader->Render(m_Direct3D->GetDeviceContext(), m_Skydome->GetIndexCount(), SkyDomeMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Skydome->GetTexture(), m_Skydome->GetTextureCount());
	if (!result)
	{
		return false;
	}
	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();


	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTextures(), m_Terrain->GetTextureCount());

	if (!result)
	{
		return false;
	}

	// Render the billboard buffers.
	m_Billboard[0]->Render(m_Direct3D->GetDeviceContext());

	// Render the billboard using the billboard shader.
	for (int i = 263; i < 310; i += 10) {

		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[0]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(263, 7, i)), viewMatrix, projectionMatrix,
			m_Billboard[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}
	for (int i = 268; i < 310; i += 20) {
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[0]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 7,150 )), viewMatrix, projectionMatrix,
			m_Billboard[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}


	// Render the terrain buffers.
	m_Billboard[1]->Render(m_Direct3D->GetDeviceContext());

	for (int i = 640; i < 730; i += 40) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[1]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(479, 0, i)), viewMatrix, projectionMatrix,
			m_Billboard[1]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}
	for (int i = 530; i < 800; i += 30) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[1]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 0, 908)), viewMatrix, projectionMatrix,
			m_Billboard[1]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}

	// Render the terrain buffers.
	m_Billboard[2]->Render(m_Direct3D->GetDeviceContext());

	for (int i = 550; i < 760; i += 30) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[2]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 0, 200)), viewMatrix, projectionMatrix,
			m_Billboard[2]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}



	D3DXMATRIX trans, scale, cupWorldMatrix;
	D3DXMatrixTranslation(&trans, 80, 5, 20);
	D3DXMatrixScaling(&scale, 5, 5, 5);
	cupWorldMatrix = scale * trans;
	m_Cup->UpdateColliders(cupWorldMatrix);
	for (auto &group : *m_Cup->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cupWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Buildngs
	//matriz de mundo para los colaiders de capsula se las asigna 
	D3DXMATRIX b01trans, b01scale, b01WorldMatrix;
	D3DXMatrixTranslation(&b01trans, 16, -7, 431);
	D3DXMatrixScaling(&b01scale, 0.6, 0.6, 0.6);
	b01WorldMatrix = b01scale * b01trans;
	//modelo
	m_building01->UpdateColliders(b01WorldMatrix);
	for (auto &group : *m_building01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b01_1trans, b01_1scale, b01_1WorldMatrix;
	D3DXMatrixTranslation(&b01_1trans, 208, 5, 320);
	D3DXMatrixScaling(&b01_1scale, 2.2, 2.2, 2.2);
	b01_1WorldMatrix = b01_1scale * b01_1trans;
	m_building01_1->UpdateColliders(b01_1WorldMatrix);
	for (auto &group : *m_building01_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b01_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b02trans, b02scale, b02WorldMatrix;
	D3DXMatrixTranslation(&b02trans, 126, 5, 381);
	D3DXMatrixScaling(&b02scale, 1, 1, 1);
	b02WorldMatrix = b02scale * b02trans;
	m_building02->UpdateColliders(b02WorldMatrix);
	for (auto &group : *m_building02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b02WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}


	D3DXMATRIX b03trans, b03scale, b03WorldMatrix;
	D3DXMatrixTranslation(&b03trans, 465, 60, 820);
	D3DXMatrixScaling(&b03scale, 1.4, 1.4, 1.4);
	b03WorldMatrix = b03scale * b03trans;
	m_building03->UpdateColliders(b03WorldMatrix);
	for (auto &group : *m_building03->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b03WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b04trans, b04scale, b04WorldMatrix;
	D3DXMatrixTranslation(&b04trans, 329, 4, 306);
	D3DXMatrixScaling(&b04scale, 0.04, 0.04, 0.04);
	b04WorldMatrix = b04scale * b04trans;
	m_building04->UpdateColliders(b04WorldMatrix);
	for (auto &group : *m_building04->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b04WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b04_1trans, b04_1scale, b04_1WorldMatrix;
	D3DXMatrixTranslation(&b04_1trans, 171, 16, 381);
	D3DXMatrixScaling(&b04_1scale, 0.7, 0.7, 0.7);
	b04_1WorldMatrix = b04_1scale * b04_1trans;
	m_building04_1->UpdateColliders(b04_1WorldMatrix);
	for (auto &group : *m_building04_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b04_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b05trans, b05scale, b05WorldMatrix;
	D3DXMatrixTranslation(&b05trans, 153, 7, 261);
	D3DXMatrixScaling(&b05scale, .5, .5, .5);
	b05WorldMatrix = b05scale * b05trans;
	m_building05->UpdateColliders(b05WorldMatrix);
	for (auto &group : *m_building05->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b05WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06trans, b06scale, b06WorldMatrix;
	D3DXMatrixTranslation(&b06trans, 33, 16, 233);
	D3DXMatrixScaling(&b06scale, 1, 1, 1);
	b06WorldMatrix = b06scale * b06trans;
	m_building06->UpdateColliders(b06WorldMatrix);
	for (auto &group : *m_building06->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06_1trans, b06_1scale, b06_1WorldMatrix;
	D3DXMatrixTranslation(&b06_1trans, 208, 15, 278);
	D3DXMatrixScaling(&b06_1scale, 0.7, 0.7, 0.7);
	b06_1WorldMatrix = b06_1scale * b06_1trans;
	m_building06_1->UpdateColliders(b06_1WorldMatrix);
	for (auto &group : *m_building06_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06_2trans, b06_2scale, b06_2WorldMatrix;
	D3DXMatrixTranslation(&b06_2trans, 109, 5, 189);
	D3DXMatrixScaling(&b06_2scale, 1, 1, 1);
	b06_2WorldMatrix = b06_2scale * b06_2trans;
	m_building06_2->UpdateColliders(b06_2WorldMatrix);
	for (auto &group : *m_building06_2->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06_2WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06Atrans, b06Ascale, b06AWorldMatrix;
	D3DXMatrixTranslation(&b06Atrans, 600, 17, 380);
	D3DXMatrixScaling(&b06Ascale, 1, 1, 1);
	b06AWorldMatrix = b06Ascale * b06Atrans;
	m_building06A->UpdateColliders(b06AWorldMatrix);
	for (auto &group : *m_building06A->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06AWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Floor piso de modelo
	D3DXMATRIX f02trans, f02scale, f02WorldMatrix;
	D3DXMatrixTranslation(&f02trans, 500, 1.2, 210);
	D3DXMatrixScaling(&f02scale, 1, 1, 1);
	f02WorldMatrix = f02scale * f02trans;
	m_floor02->UpdateColliders(f02WorldMatrix);
	for (auto &group : *m_floor02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f02WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f01trans, f01scale, f01WorldMatrix;
	D3DXMatrixTranslation(&f01trans, 500, 1.2, 350);
	D3DXMatrixScaling(&f01scale, 1, 1, 1);
	f01WorldMatrix = f01scale * f01trans;
	m_floor01->UpdateColliders(f01WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f03trans, f03scale, f03WorldMatrix;
	D3DXMatrixTranslation(&f03trans, 500, 1.2, 490);
	D3DXMatrixScaling(&f03scale, 1, 1, 1);
	f03WorldMatrix = f03scale * f03trans;
	m_floor01->UpdateColliders(f03WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f03WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f04trans, f04scale, f04WorldMatrix;
	D3DXMatrixTranslation(&f04trans, 500, 1.2, 630);
	D3DXMatrixScaling(&f04scale, 1, 1, 1);
	f04WorldMatrix = f04scale * f04trans;
	m_floor01->UpdateColliders(f04WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f04WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f05trans, f05scale, f05WorldMatrix;
	D3DXMatrixTranslation(&f05trans, 500, 1.2, 770);
	D3DXMatrixScaling(&f05scale, 1, 1, 1);
	f05WorldMatrix = f05scale * f05trans;
	m_floor01->UpdateColliders(f05WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f05WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f06trans, f06scale, f06WorldMatrix;
	D3DXMatrixTranslation(&f06trans, 500, 1.2, 910);
	D3DXMatrixScaling(&f06scale, 1, 1, 1);
	f06WorldMatrix = f06scale * f06trans;
	m_floor01->UpdateColliders(f06WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f06WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Floor Special f07 es el modelo
	D3DXMATRIX f07trans, f07scale, f07rotate, f07WorldMatrix;
	D3DXMatrixTranslation(&f07trans, 587, 1.2, 350);
	D3DXMatrixScaling(&f07scale, 1, 1, 1);
	D3DXMatrixRotationY(&f07rotate, 80.1);
	f07WorldMatrix = f07scale * f07rotate * f07trans;
	m_floor02->UpdateColliders(f07WorldMatrix);
	for (auto &group : *m_floor02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f07WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	
	//other cars
	//carrito que se mueve 
	//D3DXMATRIX car01trans, car01scale, car01rotate, car01WorldMatrix;
	static float posXcar, posYcar, posZcar ;
	if ( !m_Position->GetAnimationCarControl(1) ) {
		m_Position->GetPositionCar(posXcar, posYcar, posZcar, 3);
	}
	if (posXcar <= -67 && !m_Position->GetAnimationCarControl(1)) {
		m_Position->ChangeAnimationCarControl(1);
		m_Position->ChangeAnimationCarControl(0);
	}
	D3DXMATRIX car01trans, car01scale, car01WorldMatrix;
	D3DXMatrixTranslation(&car01trans, 590 + posXcar, 3 + posYcar, 352 + posZcar);
	D3DXMatrixScaling(&car01scale, 1, 1, 1);
	//D3DXMatrixRotationY(&car01rotate, 80.1);
	//car01WorldMatrix = car01scale * car01rotate * car01trans;
	car01WorldMatrix = car01scale * car01trans;

	if (!m_Position->GetAnimationCarControl(0)) {
		
		m_car01->UpdateColliders(car01WorldMatrix);
		for (auto &group : *m_car01->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), car01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	else {
		m_car02->UpdateColliders(car01WorldMatrix);
		for (auto &group : *m_car02->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), car01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}


	
	

	
	
	//Principal carrito
	//m_Position->MoveForward(true);
	static bool defeatRender = true;
	D3DXMATRIX transla, rotat;
	
	D3DXVECTOR3 HPosition;
	HPosition = m_Camera->GetPosition();

	static D3DXMATRIX tempprincipalWorldMatrix, principalWorldMatrix;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	float yaw, pitch, roll;

	m_Position->GetPosition(position.x, position.y, position.z);
	m_Position->GetRotation(rotation.x, rotation.y, rotation.z);

	pitch = rotation.x * 0.0174532925f;
	yaw = (rotation.y + 180.0) * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	m_Terrain->GetHeightAtPosition(position.x, position.z, position.y);
	if (HPosition.y <= position.y) {
		D3DXMatrixTranslation(&transla, position.x, position.y, position.z);
	}
	else {
		D3DXMatrixTranslation(&transla, position.x, HPosition.y, position.z);
	}

	D3DXMatrixRotationYawPitchRoll(&rotat, yaw, 0, 0);


	//colisiones
	tempprincipalWorldMatrix = rotat * transla;

	m_Principal->UpdateColliders(tempprincipalWorldMatrix);

	if (!m_Principal->Collides(m_building05)) {
		principalWorldMatrix = tempprincipalWorldMatrix;
	}
	else
	{
		tempprincipalWorldMatrix = tempprincipalWorldMatrix;
	}

	for (auto &group : *m_Principal->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), principalWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}
















	static float waterMov = 0;
	waterMov += .1;
	float A = sin(waterMov) * 20;
	D3DXMATRIX watertrans, waterscale, waterWorldMatrix;
	D3DXMatrixTranslation(&watertrans, 0, 2, -100 + A);
	D3DXMatrixScaling(&waterscale, 1, 1, 1);
	waterWorldMatrix = waterscale * watertrans;
	// Render the terrain buffers.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), waterWorldMatrix, viewMatrix, projectionMatrix,
		m_Water->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(0.9, 0.3, 0.3, 1), 10);

	if (!result)
	{
		return false;
	}

	D3DXMATRIX water01trans, water01scale, water01WorldMatrix;
	D3DXMatrixTranslation(&water01trans, 0, -2, 970 + A);
	D3DXMatrixScaling(&water01scale, 1, 1, 1);
	water01WorldMatrix = water01scale * water01trans;
	// Render the terrain buffers.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), water01WorldMatrix, viewMatrix, projectionMatrix,
		m_Water->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(0.9, 0.3, 0.3, 1), 10);

	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Render the text user interface elements.
	/*result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}*/


	if (resetGame) {
		//resetear el simulador 
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
