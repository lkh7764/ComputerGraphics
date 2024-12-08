#pragma once
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

const int NUM_LIGHTS = 1;

#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>

#include "path.h"

using namespace std;
using namespace DirectX;

class LightShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct LightColorBufferType
	{
		XMFLOAT4 diffuseColor[NUM_LIGHTS];
	};
	struct LightPositionBufferType
	{
		XMFLOAT4 lightPosition[NUM_LIGHTS];
	};
	struct LightIntensityBufferType
	{
		XMFLOAT4 lightIntensity[NUM_LIGHTS];
	};
	struct LightSettingBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 specularColor;
		float	specularPower;
		float	active_ambient;		// 0: true | 1: false
		float	active_diffuse;		// 0: true | 1: false
		float	active_specular;	// 0: true | 1: false
	};
	

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, 
		XMFLOAT4, XMFLOAT4, float, XMFLOAT4, XMFLOAT4, float);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		XMFLOAT4, XMFLOAT4, float, XMFLOAT4, XMFLOAT4, float);

	void ChangeActive_ambient();
	void ChangeActive_diffuse();
	void ChangeActive_specular();

	void ChangeFilterMode(D3D11_FILTER, ID3D11Device*, HWND);


private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, 
		XMFLOAT4, XMFLOAT4, float, XMFLOAT4, XMFLOAT4, float);
	void RenderShader(ID3D11DeviceContext*, int, int);
	void RenderShader(ID3D11DeviceContext*, int);


private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
	ID3D11Buffer* m_lightIntensityBuffer;
	ID3D11Buffer* m_lightSettingBuffer;

	// pa3
	bool m_active_ambient;
	bool m_active_diffuse;
	bool m_active_specular;

	D3D11_FILTER m_filterMode;
};

#endif
