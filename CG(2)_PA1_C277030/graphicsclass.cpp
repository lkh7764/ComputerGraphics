#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
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
	m_Camera->SetPosition(0.0f, 15.0f, -30.0f);
	m_Camera->SetRotation(25.0f, 0.0f, 0.0f);
	
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

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

	// [PA1] backcolor setting: defualt
	SetBackcolor(0.95f, 0.95f, 0.95f, 1.0f);

	return true;
}


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
	m_Model->SetDrawType(type, m_D3D->GetDevice());
}
void GraphicsClass::ReverseObjs()
{
	m_Model->ReverseObjs(m_D3D->GetDevice());
}
void GraphicsClass::SetColorBrightness(ColorBrightness b, HWND hwnd)
{
	m_ColorShader->SetColorBrightness(b, m_D3D->GetDevice(), hwnd);
}


void GraphicsClass::Shutdown()
{
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
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
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
	if(!result) return false;

	return true;
}



bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	frame += 0.01f*(float)XM_PI;
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

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::Render3DModel(int modelNum, const XMMATRIX& wm, const XMMATRIX& vm, const XMMATRIX& pm)
{
	int result;
	XMMATRIX sm, tm, rm;
	XMMATRIX worldMatrix = wm;

	if (modelNum != 0)
	{
		XMFLOAT3 scale, position;
		scale = m_Model->GetModelScale(modelNum);
		position = m_Model->GetModelPosition(modelNum);

		sm = XMMatrixScaling(scale.x, scale.y, scale.z);
		tm = XMMatrixTranslation(position.x, position.y, position.z);
		rm = XMMatrixRotationY(frame * speed_y);

		worldMatrix = wm * sm * rm * tm;
	}

	m_Model->Render(modelNum, m_D3D->GetDeviceContext(), m_D3D->GetDevice());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, vm, pm, m_Model->GetTexture(modelNum));
	if (!result) return false;
}


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

void GraphicsClass::SetFillMode(D3D11_FILL_MODE mode)
{
	m_D3D->SetFillMode(mode);
}
void GraphicsClass::SetCullMode()
{
	m_D3D->SetCullMode();
}


void GraphicsClass::ChangeFilterMode(D3D11_FILTER fmode, HWND hwnd)
{
	m_TextureShader->ChangeFilterMode(fmode, m_D3D->GetDevice(), hwnd);
}
