#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	// [PA1]
	void SetBackcolor(float, float, float, float);
	void SetDrawType(D3D_PRIMITIVE_TOPOLOGY);
	void ReverseObjs();
	void SetColorBrightness(ColorBrightness, HWND);
	void SetFillMode(D3D11_FILL_MODE);
	void SetCullMode();

	// PA2 - #3
	void ChangeFilterMode(D3D11_FILTER, HWND);


private:
	bool Render();

	// [PA1]
	bool RenderObj(int, const char, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool Render3DModel(int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);


private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;

	// [PA2]
	TextureShaderClass* m_TextureShader;

	// [PA1]
	XMFLOAT4 backcolor;
	float	frame;
	float	speed_x;
	float	speed_y;
	float	speed_z;

	float	_screenWidth;
	float	_screenHeight;
};

#endif