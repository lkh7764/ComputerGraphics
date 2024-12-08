#include "graphicsclass.h"
#include <string>
using namespace std;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Models = 0;
	m_ColorShader = 0;

	m_TextureShader = 0;

	m_Light = 0;
	m_LightShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
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
	// movable caemra -> frame
	m_Camera->SetPosition(0.0f, 1.5f, -20.0f);
	
	// Create the model objects.
	m_Models = new ModelClass*[MODEL_MAX];
	if (!m_Models) return false;

	// path
	const WCHAR* o_paths[MODEL_MAX];	const WCHAR* t_paths[MODEL_MAX];
	o_paths[0] = O_PATH_PLANE;			t_paths[0] = T_PATH_PLANE;
	o_paths[1] = O_PATH5;				t_paths[1] = T_PATH5;
	//o_paths[1] = O_PATH_PLANE;			t_paths[1] = T_PATH_PLANE;
	//o_paths[2] = O_PATH_PLANE;			t_paths[2] = T_PATH_PLANE;

	// scale&position
	XMFLOAT3 scales[MODEL_MAX];	XMFLOAT3 poses[MODEL_MAX];	float rotspeeds[MODEL_MAX];
	scales[0] = SCALE_DEFAULT;	poses[0] = POSITION_PLANE;		rotspeeds[0] = 0.0f;
	scales[1] = SCALE_DEFAULT;	poses[1] = POSITION_DEFAULT;	rotspeeds[1] = 0.0f;
	//scales[1] = SCALE_DEFAULT;	poses[1] = POSITION_PLANE1;		rotspeeds[1] = 0.0f;
	//scales[2] = SCALE_DEFAULT;	poses[2] = POSITION_PLANE2;		rotspeeds[2] = 0.0f;

	// PA3, INSTANCE COUNT
	// plane도 instance로
	int instanceCnt[MODEL_MAX];
	instanceCnt[0] = 1;			instanceCnt[1] = 10;
	//instanceCnt[2] = 1;			instanceCnt[3] = 10;

	int objCnt = 0, polygonCnt = 0;
	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_Models[i] = new ModelClass();
		if (!m_Models[i]) return false;

		result = m_Models[i]->Initialize(m_D3D->GetDevice(), o_paths[i], t_paths[i], instanceCnt[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models[i]->SetScale(scales[i]);
		m_Models[i]->SetPosition(poses[i]);
		m_Models[i]->SetRotationSpeed(rotspeeds[i]);

		objCnt += instanceCnt[i];
		polygonCnt += instanceCnt[i]*(m_Models[i]->GetFaceCount());
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

	// textureshader
	/*
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	*/


	m_LightShader = new LightShaderClass;
	if (!m_LightShader) return false;

	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light) return false;

	m_Light->SetDiffuseColor(COLOR_DEFAULT);
	m_Light->SetPosition(0.0f, 3.0f, -20.0f);
	m_Light->SetIntensity(0.5f);
	m_Light->SetAmbientColor(COLOR_AMBIENT);
	m_Light->SetSpecularColor(COLOR_DEFAULT);
	m_Light->SetSpecularPower(10.0f);

	// [PA1] backcolor setting: defualt
	// SetBackcolor(0.95f, 0.95f, 0.95f, 1.0f);

	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) return false;

	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, T_PATH_BACK, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	m_Text = new TextClass;
	if (!m_Text) return false;

	XMMATRIX baseViewMatrix;
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	string sentense1 = "object count: ";
	sentense1.append(to_string(objCnt));
	const char* s1 = sentense1.c_str();
	result = m_Text->SetSetense1(s1, 10, 10, 1.0f, 1.0f, 1.0f, m_D3D->GetDeviceContext());
	if (!result) return false;

	string sentense2 = "polygon count: ";
	sentense2.append(to_string(polygonCnt));
	const char* s2 = sentense2.c_str();
	result = m_Text->SetSetense2(s2, 10, 50, 1.0f, 1.0f, 0.0f, m_D3D->GetDeviceContext());
	if (!result) return false;

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

	// texture sahder
	/*
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	*/

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

	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
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
	XMFLOAT4 diffuseColor;
	XMFLOAT4 lightPosition;
	float lightIntensity;
	bool result;

	//frame += 0.005f*(float)XM_PI;
	//if (frame > 360.0f) frame -= 360.0f;

	// set light informations
	diffuseColor = m_Light->GetDiffuseColor();
	lightPosition = m_Light->GetPosition();
	lightIntensity = m_Light->GetIntensity();

	m_D3D->BeginScene(backcolor.x, backcolor.y, backcolor.z, backcolor.w);
	
	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// pa3 - bitmap: 평행투영
	XMMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->TurnZBufferOff();

	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);	// background 위치
	if (!result) return false;

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, 
		m_Bitmap->GetTexture(), diffuseColor, lightPosition, lightIntensity, m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result) return false;


	m_D3D->TurnOnAlphaBlending();
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result) return false;

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();


	for (int i = 0; i < MODEL_MAX; i++)
	{
		result = Render3DModel(i, worldMatrix, viewMatrix, projectionMatrix, diffuseColor, lightPosition, lightIntensity);
		if (!result) return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
bool GraphicsClass::Render3DModel(int modelNum, const XMMATRIX& wm, const XMMATRIX& vm, const XMMATRIX& pm, 
	XMFLOAT4 diffuseColor, XMFLOAT4 lightPosition, float lightIntensity)
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

	// use texture shader
	/*
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Models[modelNum]->GetVertexCount(),
		m_Models[modelNum]->GetInstanceCount(), worldMatrix, vm, pm, m_Models[modelNum]->GetTexture());
		*/

	// use light shader
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[modelNum]->GetVertexCount(), m_Models[modelNum]->GetInstanceCount(), 
		worldMatrix, vm, pm, m_Models[modelNum]->GetTexture(), diffuseColor, lightPosition, lightIntensity,
		m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

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
	//m_D3D->SetFillMode(mode);
}
void GraphicsClass::SetCullMode()
{
	//m_D3D->SetCullMode();
}


// pa2
void GraphicsClass::ChangeFilterMode(D3D11_FILTER fmode, HWND hwnd)
{
	m_LightShader->ChangeFilterMode(fmode, m_D3D->GetDevice(), hwnd);
}


// pa3
void GraphicsClass::ChangeLightColor(XMFLOAT4 color)
{
	m_Light->SetDiffuseColor(color);
}
void GraphicsClass::AddLightIntensity()
{
	float value = m_Light->GetIntensity() + 0.1f;
	if (value > 1.0f) return;

	m_Light->SetIntensity(value);
}
void GraphicsClass::ReduceLightIntensity()
{
	float value = m_Light->GetIntensity() - 0.1f;
	if (value < 0.0f) return;

	m_Light->SetIntensity(value);
}
void GraphicsClass::ChangeActive_Ambient()
{
	m_LightShader->ChangeActive_ambient();
}
void GraphicsClass::ChangeActive_Diffuse()
{
	m_LightShader->ChangeActive_diffuse();
}
void GraphicsClass::ChangeActive_Specular()
{
	m_LightShader->ChangeActive_specular();
}

