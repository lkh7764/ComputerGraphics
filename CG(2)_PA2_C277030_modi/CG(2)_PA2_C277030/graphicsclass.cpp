#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Models = 0;
	m_ColorShader = 0;

	m_TextureShader = 0;
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

	frame = 0.0f;
	speed_x = 1.0f;
	speed_y = 2.0f;
	speed_z = -0.5f;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D) return false;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(_screenWidth, _screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera) return false;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 5.0f, -30.0f);
	m_Camera->SetRotation(5.0f, 0.0f, 0.0f);
	
	// Create the model objects.
	m_Models = new ModelClass*[MODEL_MAX];
	if (!m_Models) return false;

	// path
	const WCHAR* o_paths[MODEL_MAX];	const WCHAR* t_paths[MODEL_MAX];
	o_paths[0] = O_PATH_PLANE;			t_paths[0] = T_PATH_PLANE;
	o_paths[1] = O_PATH_PLANE;			t_paths[1] = T_PATH_PLANE;
	o_paths[2] = O_PATH1;				t_paths[2] = T_PATH1;
	o_paths[3] = O_PATH2;				t_paths[3] = T_PATH2;
	o_paths[4] = O_PATH3;				t_paths[4] = T_PATH3;
	o_paths[5] = O_PATH_PLANE;			t_paths[5] = T_PATH_PLANE;

	// scale&position
	XMFLOAT3 scales[MODEL_MAX];	XMFLOAT3 poses[MODEL_MAX];	float rotspeeds[MODEL_MAX];
	scales[0] = SCALE_PLANE;	poses[0] = POSITION_PLANE;	rotspeeds[0] = 0.0f;
	scales[1] = SCALE_PLANE;	poses[1] = POSITION_PLANE1;	rotspeeds[1] = 0.0f;
	scales[2] = SCALE1;			poses[2] = POSITION1;		rotspeeds[2] = 0.5f;
	scales[3] = SCALE2;			poses[3] = POSITION2;		rotspeeds[3] = 0.25f;
	scales[4] = SCALE3;			poses[4] = POSITION3;		rotspeeds[4] = 1.0f;
	scales[5] = SCALE_PLANE;	poses[5] = POSITION_PLANE2;	rotspeeds[5] = 0.0f;

	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_Models[i] = new ModelClass();
		if (!m_Models[i]) return false;

		result = m_Models[i]->Initialize(m_D3D->GetDevice(), o_paths[i], t_paths[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models[i]->SetScale(scales[i]);
		m_Models[i]->SetPosition(poses[i]);
		m_Models[i]->SetRotationSpeed(rotspeeds[i]);
	}

	// colorshader
	/*
	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader) return false;

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, (LPCWSTR)"Could not initialize the color shader object.", (LPCWSTR)"Error", MB_OK);
		return false;
	}
		*/

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// [PA1] backcolor setting: defualt
	SetBackcolor(0.95f, 0.95f, 0.95f, 1.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// color shader
	/*
	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	*/

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_Models)
	{
		for (int i = 0; i < MODEL_MAX; i++)
		{
			m_Models[i]->Shutdown();
		}

		delete[] m_Models;
		m_Models = 0;
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


	result = Render();
	if(!result) return false;

	return true;
}
bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	frame += 0.005f*(float)XM_PI;
	if (frame > 360.0f) frame -= 360.0f;


	m_D3D->BeginScene(backcolor.x, backcolor.y, backcolor.z, backcolor.w);
	
	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	Render3DModel(0, worldMatrix, viewMatrix, projectionMatrix);
	Render3DModel(1, worldMatrix, viewMatrix, projectionMatrix);
	Render3DModel(2, worldMatrix, viewMatrix, projectionMatrix);
	Render3DModel(3, worldMatrix, viewMatrix, projectionMatrix);
	Render3DModel(4, worldMatrix, viewMatrix, projectionMatrix);
	Render3DModel(5, worldMatrix, viewMatrix, projectionMatrix);

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
bool GraphicsClass::Render3DModel(int modelNum, const XMMATRIX& wm, const XMMATRIX& vm, const XMMATRIX& pm)
{
	int result;
	XMMATRIX sm, tm, rm;
	XMMATRIX worldMatrix = wm;

	XMFLOAT3 scale, position;
	scale = m_Models[modelNum]->GetScale();
	position = m_Models[modelNum]->GetPosition();
	float rotspeed = m_Models[modelNum]->GetRotationSpeed();

	sm = XMMatrixScaling(scale.x, scale.y, scale.z);
	tm = XMMatrixTranslation(position.x, position.y, position.z);
	rm = XMMatrixRotationY(frame * speed_y * rotspeed);

	worldMatrix = wm * sm * rm * tm;

	m_Models[modelNum]->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Models[modelNum]->GetIndexCount(),
		worldMatrix, vm, pm, m_Models[modelNum]->GetTexture());

	if (!result) return false;
}
/*
bool GraphicsClass::RenderObj(int objNum, char axis, const XMMATRIX& wm, const XMMATRIX& vm, const XMMATRIX& pm)
{
	int result;
	XMMATRIX rm;

	switch (axis)
	{
	case 'x':
		rm = XMMatrixRotationX(frame * speed_x);
		break;
	case 'z':
		rm = XMMatrixRotationZ(frame * speed_z);
		break;
	default:	// y
		rm = XMMatrixRotationY(frame * speed_y);
		break;
	}

	XMMATRIX worldMatrix = XMMatrixMultiply(wm, rm);

	m_Model->Render(objNum, m_D3D->GetDeviceContext(), m_D3D->GetDevice());

	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), 
		m_Model->GetIndexCount(), worldMatrix, vm, pm);
	if (!result) return false;
}
*/


// [PA1]
void GraphicsClass::SetBackcolor(float r, float g, float b, float a)
{
	backcolor.x = r;
	backcolor.y = g;
	backcolor.z = b;
	backcolor.w = a;
}
void GraphicsClass::SetDrawType(D3D_PRIMITIVE_TOPOLOGY type)
{
	for (int i = 0; i < MODEL_MAX; i++)
		m_Models[i]->SetDrawType(type, m_D3D->GetDevice());
}
void GraphicsClass::ReverseObjs()
{
	for (int i = 0; i < MODEL_MAX; i++)
		m_Models[i]->ReverseObjs(m_D3D->GetDevice());
}
void GraphicsClass::SetColorBrightness(ColorBrightness b, HWND hwnd)
{
	m_ColorShader->SetColorBrightness(b, m_D3D->GetDevice(), hwnd);
}
void GraphicsClass::SetFillMode(D3D11_FILL_MODE mode)
{
	m_D3D->SetFillMode(mode);
}
void GraphicsClass::SetCullMode()
{
	m_D3D->SetCullMode();
}


// pa2
void GraphicsClass::ChangeFilterMode(D3D11_FILTER fmode, HWND hwnd)
{
	m_TextureShader->ChangeFilterMode(fmode, m_D3D->GetDevice(), hwnd);
}
