#ifndef _3DOBJCLASS_H_
#define _3DOBJCLASS_H_

#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "textureclass.h"
#include "path.h"

#include <fstream>

using namespace std;
using namespace DirectX;


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

class Model3DObject
{
	/*
		3d model�� �������ϴ� ����
		> model load�� �ʿ��� ��
			- obj ������ �ּ�
			- texture ������ �ּ�
		> render�� �ʿ��� ��: modelclass���� �긦 ���ļ� �ϳ��� ������ ���� ��
			- �ش� obj�� ���ؽ� ����
			- �ش� obj�� ���ؽ� ī��Ʈ
			- �ش� obj�� �ε��� ����
			- �ش� obj�� �ε��� ī��Ʈ
			- �ش� obj�� ���� texture ���� (TextureClass)
			- �ش� obj�� model type
	*/

public:
	Model3DObject(const WCHAR*, const WCHAR*);
	~Model3DObject();

	const WCHAR* GetObjPath();
	const WCHAR* GetTexturePath();

	int GetVertexCnt();
	int GetIndexCnt();
	int GetTextureCnt();
	int GetNormalCnt();
	int GetFaceCnt();

	const ID3D11Buffer* GetVertexBuffer();
	const ID3D11Buffer* GetIndexBuffer();
	const ModelType* GetModelType();
	TextureClass* GetTexture();

	bool LoadModel_obj();

	void ReleaseTexture();
	void ReleaseIndexBuffer();
	void ReleaseVertexBuffer();
	void ReleaseModel();

	bool IsExistTexture() { return o_texture != nullptr; }
	bool IsExistVertexBuf() { return o_vertexBuf != nullptr; }
	bool IsExistIndexBuf() { return o_indexBuf != nullptr; }
	bool IsExistModel() { return o_model != nullptr; }

	void SetScale(XMFLOAT3);
	void SetPosition(XMFLOAT3);
	const XMFLOAT3 GetScale();
	const XMFLOAT3 GetPosition();


private:
	bool ReadFileCounts();
	bool LoadDataStructures(int, int, int, int);


private:
	// load
	const WCHAR*	path_obj;
	const WCHAR*	path_texture;

	// render
	int				o_vertexCnt;
	ID3D11Buffer*	o_vertexBuf;
	int				o_indexCnt;
	ID3D11Buffer*	o_indexBuf;

	int				o_textureCnt;
	int				o_normalCnt;
	int				o_faceCnt;
	TextureClass*	o_texture;
	ModelType*		o_model;

	XMFLOAT3		scale;
	XMFLOAT3		position;
};



#endif