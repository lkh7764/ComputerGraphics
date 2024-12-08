#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;


class D3DClass : public AlignedAllocationPolicy<16>
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	bool InitializeGraphicsHW(int screenWidth, int screenHeight, bool vsync);
	bool InitializeSwapBuffer(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen);
	bool InitializeDepthStencilBuffer(int screenWidth, int screenHeight);
	bool InitializeRasterizer();
	bool InitializeViewport(int screenWidth, int screenHeight);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	// [PA1]
	void SetFillMode(D3D11_FILL_MODE);
	void SetCullMode();

private:
	bool m_vsync_enabled;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	unsigned int m_refreshRateNumerator;
	unsigned int m_refreshRateDenominator;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	
	ID3D11RasterizerState* m_rasterState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	// [PA1]
	D3D11_FILL_MODE m_fillMode;
	D3D11_CULL_MODE m_cullMode;
};

#endif