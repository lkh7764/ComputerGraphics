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
		3d model이 가져야하는 정보
		> model load에 필요한 것
			- obj 파일의 주소
			- texture 파일의 주소
		> render에 필요한 것: modelclass에서 얘를 합쳐서 하나의 정보로 가질 것
			- 해당 obj의 버텍스 정보
			- 해당 obj의 버텍스 카운트
			- 해당 obj의 인덱스 정보
			- 해당 obj의 인덱스 카운트
			- 해당 obj에 대한 texture 정보 (TextureClass)
			- 해당 obj의 model type
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