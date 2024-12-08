#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "objectclass.h"
#include <directxmath.h>
#include "textureclass.h"
#include "path.h"

#include <fstream>

using namespace std;
using namespace DirectX;

#define MODEL_MAX	6



class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};


public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// [PA1]
	void SetDrawType(D3D_PRIMITIVE_TOPOLOGY, ID3D11Device*);
	void ReverseObjs(ID3D11Device*);
	int GetObjCount();
	// void Render(int, ID3D11DeviceContext*, ID3D11Device*);

	// PA2
	ID3D11ShaderResourceView* GetTexture();

	void SetScale(XMFLOAT3);
	void SetPosition(XMFLOAT3);
	void SetRotationSpeed(float);
	const XMFLOAT3 GetScale();
	const XMFLOAT3 GetPosition();
	const float GetRotationSpeed();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// [PA1]
	bool CreateObjects(int cnt = 3);
	bool InitializeSigleObjBuf(int, ID3D11Device*);

	// PA2
	bool LoadModel(const WCHAR*);
	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();
	void ReleaseModel();

	// bool Load3DModels();
	// bool LoadTextures(ID3D11Device*);
	// bool InitializeModelObjBuf(int, ID3D11Device*);


private:
	// ID3D11Buffer, 그냥 버퍼 말고 DIRECTX가 제공하는 버퍼 자료형을 사용해야 한다
	ID3D11Buffer	*m_vertexBuffer;
	ID3D11Buffer	*m_indexBuffer;
	int				m_vertexCount;
	int				m_indexCount;

	// [PA1]
	Object*			*objs;
	int				objCount;
	D3D_PRIMITIVE_TOPOLOGY drawType;

	// PA2
	int				m_textureCount;
	int				m_normalCount;
	int				m_faceCount;
	ModelType*		m_model;
	TextureClass*	m_texture;

	XMFLOAT3		m_scale;
	XMFLOAT3		m_position;
	float			m_rotspeed;
};


#endif