#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "objectclass.h"
#include "3dmodelobjclass.h"
#include "textureclass.h"

#define MODEL_MAX	4



class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// [PA1]
	void SetDrawType(D3D_PRIMITIVE_TOPOLOGY, ID3D11Device*);
	void ReverseObjs(ID3D11Device*);
	int GetObjCount();

	void Render(int, ID3D11DeviceContext*, ID3D11Device*);

	// PA2
	ID3D11ShaderResourceView* GetTexture(int);

	bool Load3DModels();
	void ReleaseModel();
	const XMFLOAT3 GetModelScale(int);
	const XMFLOAT3 GetModelPosition(int);


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// [PA1]
	bool CreateObjects(int cnt = 3);
	bool InitializeSigleObjBuf(int, ID3D11Device*);

	// PA2
	bool LoadTextures(ID3D11Device*);
	void ReleaseTexture();
	bool InitializeModelObjBuf(int, ID3D11Device*);


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

	Model3DObject** models;
	int				modelCnt;
};


#endif