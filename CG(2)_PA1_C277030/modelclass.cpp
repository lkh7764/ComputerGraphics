#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	objs = 0;

	// pa2
	m_model = 0;

	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;

	models = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	// [PA1], Create objects
	drawType = TRIANGLE;
	// result = CreateObjects();

	// pa2, create 3dmodels
	modelCnt = MODEL_MAX;
	result = Load3DModels();
	if(!result) return false;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result) return false;

	result = LoadTextures(device);

	return true;
}


bool ModelClass::Load3DModels()
{
	bool result;

	if (modelCnt < 4) return false;
	models = new Model3DObject*[modelCnt];

	// model0, plane
	models[0] = new Model3DObject(O_PATH_PLANE, T_PATH_PLANE);
	result= models[0]->LoadModel_obj();
	if (!result) return false;
	models[0]->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	models[0]->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

	// model1
	if (modelCnt < 1) return false;
	models[1] = new Model3DObject(O_PATH1, T_PATH1);
	result = models[1]->LoadModel_obj();
	if (!result) return false;
	models[1]->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	models[1]->SetPosition(XMFLOAT3(8.0f, 3.0f, 5.0f));

	// model2
	if (modelCnt < 2) return false;
	models[2] = nullptr;
	models[2] = new Model3DObject(O_PATH2, T_PATH2);
	result = models[2]->LoadModel_obj();
	if (!result) return false;
	models[2]->SetScale(XMFLOAT3(0.15f, 0.15f, 0.15f));
	models[2]->SetPosition(XMFLOAT3(-15.0f, 0.0f, 15.0f));

	// model3
	if (modelCnt < 3) return false;
	models[3] = nullptr;
	models[3] = new Model3DObject(O_PATH3, T_PATH3);
	result = models[3]->LoadModel_obj();
	if (!result) return false;
	models[3]->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
	models[3]->SetPosition(XMFLOAT3(3.0f, 0.0f, -5.0f));
}
bool ModelClass::LoadTextures(ID3D11Device* device)
{
	bool result;

	TextureClass* m_Texture = nullptr;

	for (int i = 0; i < modelCnt; i++)
	{
		m_Texture = models[i]->GetTexture();
		if (!m_Texture) return false;

		result = m_Texture->Initialize(device, models[i]->GetTexturePath());
		if (!result) return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

void ModelClass::Render(int objNum, ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	//InitializeSigleObjBuf(objNum, device);
	InitializeModelObjBuf(objNum, device);
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	bool result;

	// PA1
	/*
	for (int i = 0; i < objCount; i++)
	{
		result = InitializeSigleObjBuf(i, device);
		if (!result) return false;
	}
	*/

	// PA2
	for (int i = 0; i < modelCnt; i++)
	{
		result = InitializeModelObjBuf(i, device);
		if (!result) return false;
	}

	return true;
}
bool ModelClass::InitializeSigleObjBuf(int objNum, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;

	// 실제 넣을 데이터 >> D3D11_BUFFER_DESC, 
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = objs[objNum]->GetVertexCount();
	m_indexCount = objs[objNum]->GetIndexCount();

	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	int len = sizeof(VertexType) * m_vertexCount;
	memcpy(vertices, objs[objNum]->GetVertexBuffer(), len);
	len = sizeof(unsigned long) * m_indexCount;
	memcpy(indices, objs[objNum]->GetIndexBuffer(), len);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;						// 중요1, USAGE: 기본적인 버퍼 쓸거임
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;	// 중요2, BYTEWIDTH: 버퍼 사이즈
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// 중요3, BINDFLAG: 버퍼용으로 쓸거임
	vertexBufferDesc.CPUAccessFlags = 0;								// 안 알아도 됨
	vertexBufferDesc.MiscFlags = 0;										// 안 알아도 됨
	vertexBufferDesc.StructureByteStride = 0;							// 안 알아도 됨

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;		// 버텍스 연결
	vertexData.SysMemPitch = 0;			// 위치 관련: 신경 안 써도 됨
	vertexData.SysMemSlicePitch = 0;	// 위치 관련: 신경 안 써도 됨

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


bool ModelClass::InitializeModelObjBuf(int modelNum, ID3D11Device* device)
{
	if (modelNum >= MODEL_MAX) return false;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = models[modelNum]->GetVertexCnt();
	m_indexCount = models[modelNum]->GetIndexCnt();

	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	const ModelType* m_model = models[modelNum]->GetModelType();

	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ReleaseTexture()
{
	for (int i = 0; i < modelCnt; i++) 
	{
		if (!models[i]->GetTexture()) continue;
		models[i]->ReleaseTexture();
	}

	return;
}
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	for (int i = 0; i < modelCnt; i++)
	{
		if (models[i]->GetIndexBuffer())
			models[i]->ReleaseIndexBuffer();
		if (models[i]->GetVertexBuffer())
			models[i]->ReleaseVertexBuffer();
	}

	return;
}
void ModelClass::ReleaseModel()
{
	for (int i = 0; i < modelCnt; i++)
	{
		if (!models[i]->GetModelType()) continue;
		models[i]->ReleaseModel();
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(drawType);
	// 우리는 triangle polygon 쓸거라 안에 D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST로 설정해서 실행해야함

	return;
}


// PA1
void ModelClass::SetDrawType(D3D_PRIMITIVE_TOPOLOGY type, ID3D11Device* device)
{
	drawType = type;
	for (int i = 0; i < objCount; i++) objs[i]->SetIndexBuffer(drawType);
}
void ModelClass::ReverseObjs(ID3D11Device* device)
{
	for (int i = 0; i < objCount; i++) objs[i]->Reverse();
}
bool ModelClass::CreateObjects(int cnt)
{
	// default 3
	if (cnt < 3) cnt = 3;

	// draw할 object 개수 설정
	objCount = cnt;

	// 개수만큼 오브젝트 저장할 공간 생성
	objs = new Object*[objCount];
	if (objs == nullptr) return false;


	// obj 0, 보석
	int vCnt0 = 8, pCnt0 = 8, lCnt0 = 12;
	VertexType_2d* vBuf = new VertexType_2d[vCnt0];
	vBuf[0].position = XMFLOAT3(-0.6f, 1.0f, 0.0f);		vBuf[0].color = XMFLOAT4(0.6f, 0.75f, 0.9f, 1.0f);
	vBuf[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);		vBuf[1].color = XMFLOAT4(0.6f, 0.75f, 0.9f, 1.0f);
	vBuf[2].position = XMFLOAT3(0.6f, 1.0f, 0.0f);		vBuf[2].color = XMFLOAT4(0.6f, 0.75f, 0.9f, 1.0f);
	vBuf[3].position = XMFLOAT3(-1.0f, 0.3f, 0.0f);		vBuf[3].color = XMFLOAT4(0.75f, 0.9f, 1.0f, 1.0f);
	vBuf[4].position = XMFLOAT3(-0.33f, 0.3f, 0.0f);	vBuf[4].color = XMFLOAT4(0.9f, 0.99f, 1.0f, 1.0f);
	vBuf[5].position = XMFLOAT3(0.33f, 0.3f, 0.0f);		vBuf[5].color = XMFLOAT4(0.9f, 0.99f, 1.0f, 1.0f);
	vBuf[6].position = XMFLOAT3(1.0f, 0.3f, 0.0f);		vBuf[6].color = XMFLOAT4(0.75f, 0.9f, 1.0f, 1.0f);
	vBuf[7].position = XMFLOAT3(0.0f, -1.0f, 0.0f);		vBuf[7].color = XMFLOAT4(0.6f, 0.8f, 0.9f, 1.0f);

	unsigned long* t_iBuf = new unsigned long[pCnt0 * 3];
	t_iBuf[0] = 0;	t_iBuf[1] = 4;	t_iBuf[2] = 3;	t_iBuf[3] = 0;	t_iBuf[4] = 1;	t_iBuf[5] = 4;
	t_iBuf[6] = 1;	t_iBuf[7] = 5;	t_iBuf[8] = 4;	t_iBuf[9] = 1;	t_iBuf[10] = 2;	t_iBuf[11] = 5;
	t_iBuf[12] = 5;	t_iBuf[13] = 2;	t_iBuf[14] = 6;	t_iBuf[15] = 3;	t_iBuf[16] = 4;	t_iBuf[17] = 7;
	t_iBuf[18] = 4;	t_iBuf[19] = 5;	t_iBuf[20] = 7;	t_iBuf[21] = 5;	t_iBuf[22] = 6;	t_iBuf[23] = 7;

	unsigned long* l_iBuf = new unsigned long[lCnt0 * 2];
	l_iBuf[0] = 0;	l_iBuf[1] = 2;	l_iBuf[2] = 3;	l_iBuf[3] = 6;	l_iBuf[4] = 3;	l_iBuf[5] = 0;
	l_iBuf[6] = 0;	l_iBuf[7] = 4;	l_iBuf[8] = 4;	l_iBuf[9] = 1;	l_iBuf[10] = 1;	l_iBuf[11] = 5;
	l_iBuf[12] = 5;	l_iBuf[13] = 2;	l_iBuf[14] = 2;	l_iBuf[15] = 6;	l_iBuf[16] = 3;	l_iBuf[17] = 7;
	l_iBuf[18] = 7;	l_iBuf[19] = 4;	l_iBuf[20] = 5;	l_iBuf[21] = 7;	l_iBuf[22] = 7;	l_iBuf[23] = 6;
	
	objs[0] = new Object(vCnt0, vBuf, pCnt0, t_iBuf, lCnt0, l_iBuf);
	if (objs[0] == nullptr) return false;


	// obj 1, 별
	int vCnt1 = 10, pCnt1 = 8, lCnt1 = 10;
	VertexType_2d* vBuf1 = new VertexType_2d[vCnt1];
	vBuf1[0].position = XMFLOAT3(4.0f, 3.0f, 2.0f);		vBuf1[0].color = XMFLOAT4(1.0f, 0.98f, 0.8f, 1.0f);
	vBuf1[1].position = XMFLOAT3(3.7f, 2.2f, 2.0f);		vBuf1[1].color = XMFLOAT4(1.0f, 0.8f, 0.3f, 1.0f);
	vBuf1[2].position = XMFLOAT3(3.0f, 2.2f, 2.0f);		vBuf1[2].color = XMFLOAT4(1.0f, 0.98f, 0.8f, 1.0f);
	vBuf1[3].position = XMFLOAT3(3.5f, 1.7f, 2.0f);		vBuf1[3].color = XMFLOAT4(1.0f, 0.8f, 0.3f, 1.0f);
	vBuf1[4].position = XMFLOAT3(3.2f, 1.0f, 2.0f);		vBuf1[4].color = XMFLOAT4(1.0f, 0.98f, 0.8f, 1.0f);
	vBuf1[5].position = XMFLOAT3(4.0f, 1.4f, 2.0f);		vBuf1[5].color = XMFLOAT4(1.0f, 0.8f, 0.3f, 1.0f);
	vBuf1[6].position = XMFLOAT3(4.8f, 1.0f, 2.0f);		vBuf1[6].color = XMFLOAT4(1.0f, 0.98f, 0.8f, 1.0f);
	vBuf1[7].position = XMFLOAT3(4.5f, 1.7f, 2.0f);		vBuf1[7].color = XMFLOAT4(1.0f, 0.8f, 0.3f, 1.0f);
	vBuf1[8].position = XMFLOAT3(5.0f, 2.2f, 2.0f);		vBuf1[8].color = XMFLOAT4(1.0f, 0.98f, 0.8f, 1.0f);
	vBuf1[9].position = XMFLOAT3(4.3f, 2.2f, 2.0f);		vBuf1[9].color = XMFLOAT4(1.0f, 0.8f, 0.3f, 1.0f);

	unsigned long* t_iBuf1 = new unsigned long[pCnt1 * 3];
	t_iBuf1[0] = 0;		t_iBuf1[1] = 9;		t_iBuf1[2] = 1;	
	t_iBuf1[3] = 1;		t_iBuf1[4] = 3;		t_iBuf1[5] = 2;
	t_iBuf1[6] = 1;		t_iBuf1[7] = 5;		t_iBuf1[8] = 3;	
	t_iBuf1[9] = 3;		t_iBuf1[10] = 5;	t_iBuf1[11] = 4;
	t_iBuf1[12] = 5;	t_iBuf1[13] = 7;	t_iBuf1[14] = 6;
	t_iBuf1[15] = 5;	t_iBuf1[16] = 9;	t_iBuf1[17] = 7;
	t_iBuf1[18] = 9;	t_iBuf1[19] = 8;	t_iBuf1[20] = 7;
	t_iBuf1[21] = 9;	t_iBuf1[22] = 5;	t_iBuf1[23] = 1;

	unsigned long* l_iBuf1 = new unsigned long[lCnt1 * 2];
	l_iBuf1[0] = 0;		l_iBuf1[1] = 1;		l_iBuf1[2] = 1;		l_iBuf1[3] = 2;
	l_iBuf1[4] = 2;		l_iBuf1[5] = 3;		l_iBuf1[6] = 3;		l_iBuf1[7] = 4;
	l_iBuf1[8] = 4;		l_iBuf1[9] = 5;		l_iBuf1[10] = 5;	l_iBuf1[11] = 6;
	l_iBuf1[12] = 6;	l_iBuf1[13] = 7;	l_iBuf1[14] = 7;	l_iBuf1[15] = 8;
	l_iBuf1[16] = 8;	l_iBuf1[17] = 9;	l_iBuf1[18] = 9;	l_iBuf1[19] = 0;

	objs[1] = new Object(vCnt1, vBuf1, pCnt1, t_iBuf1, lCnt1, l_iBuf1);
	if (objs[1] == nullptr) return false;


	// obj 2, 종이비행기
	int vCnt2 = 9, pCnt2 = 6, lCnt2 = 10;
	VertexType_2d* vBuf2 = new VertexType_2d[vCnt2];
	vBuf2[0].position = XMFLOAT3(-4.0f, 3.5f, 2.0f);	vBuf2[0].color = XMFLOAT4(1.0f, 0.95f, 0.9f, 1.0f);
	vBuf2[1].position = XMFLOAT3(-5.0f, 1.5f, 2.0f);	vBuf2[1].color = XMFLOAT4(1.0f, 0.95f, 0.9f, 1.0f);
	vBuf2[2].position = XMFLOAT3(-4.5f, 0.5f, 2.0f);	vBuf2[2].color = XMFLOAT4(1.0f, 0.95f, 0.9f, 1.0f);
	vBuf2[3].position = XMFLOAT3(-4.0f, 1.2f, 2.0f);	vBuf2[3].color = XMFLOAT4(1.0f, 0.95f, 0.9f, 1.0f);
	vBuf2[4].position = XMFLOAT3(-3.5f, 0.5f, 2.0f);	vBuf2[4].color = XMFLOAT4(0.93f, 0.93f, 0.85f, 1.0f);
	vBuf2[5].position = XMFLOAT3(-3.0f, 1.5f, 2.0f);	vBuf2[5].color = XMFLOAT4(0.85f, 0.85f, 0.8f, 1.0f);
	vBuf2[6].position = XMFLOAT3(-4.1f, 1.1f, 2.0f);	vBuf2[6].color = XMFLOAT4(0.85f, 0.85f, 0.8f, 1.0f);
	vBuf2[7].position = XMFLOAT3(-4.0f, 0.8f, 2.0f);	vBuf2[7].color = XMFLOAT4(0.85f, 0.85f, 0.8f, 1.0f);
	vBuf2[8].position = XMFLOAT3(-3.9f, 1.1f, 2.0f);	vBuf2[8].color = XMFLOAT4(0.85f, 0.85f, 0.8f, 1.0f);

	unsigned long* t_iBuf2 = new unsigned long[pCnt2 * 3];
	t_iBuf2[0] = 0;		t_iBuf2[1] = 2;		t_iBuf2[2] = 1;
	t_iBuf2[3] = 0;		t_iBuf2[4] = 3;		t_iBuf2[5] = 2;
	t_iBuf2[6] = 0;		t_iBuf2[7] = 4;		t_iBuf2[8] = 3;
	t_iBuf2[9] = 0;		t_iBuf2[10] = 5;	t_iBuf2[11] = 4;
	t_iBuf2[12] = 3;	t_iBuf2[13] = 7;	t_iBuf2[14] = 6;
	t_iBuf2[15] = 3;	t_iBuf2[16] = 8;	t_iBuf2[17] = 7;

	unsigned long* l_iBuf2 = new unsigned long[lCnt2 * 2];
	l_iBuf2[0] = 0;		l_iBuf2[1] = 1;		l_iBuf2[2] = 1;		l_iBuf2[3] = 2;
	l_iBuf2[4] = 2;		l_iBuf2[5] = 3;		l_iBuf2[6] = 3;		l_iBuf2[7] = 0;
	l_iBuf2[8] = 0;		l_iBuf2[9] = 5;		l_iBuf2[10] = 5;	l_iBuf2[11] = 4;
	l_iBuf2[12] = 4;	l_iBuf2[13] = 3;	l_iBuf2[14] = 3;	l_iBuf2[15] = 7;
	l_iBuf2[16] = 7;	l_iBuf2[17] = 8;	l_iBuf2[18] = 7;	l_iBuf2[19] = 6;

	objs[2] = new Object(vCnt2, vBuf2, pCnt2, t_iBuf2, lCnt2, l_iBuf2);
	if (objs[2] == nullptr) return false;

	if (objCount == 3) return true;

	for (int i = 3; i < objCount; i++)
	{
		// defualt triangle
		int vCnt3 = 3, pCnt3 = 1, lCnt3 = 3;
		VertexType_2d* vBuf3 = new VertexType_2d[vCnt3];
		vBuf3[0].position = XMFLOAT3(0.0f, 1.0f, 0.0f);		vBuf3[0].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		vBuf3[1].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);	vBuf3[1].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		vBuf3[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);	vBuf3[2].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	
		unsigned long* t_iBuf3 = new unsigned long[pCnt3 * 3];
		t_iBuf3[0] = 0;		t_iBuf3[1] = 1;		t_iBuf3[2] = 2;

		unsigned long* l_iBuf3 = new unsigned long[lCnt3 * 2];
		l_iBuf3[0] = 0;		l_iBuf3[1] = 1;		l_iBuf3[2] = 1;		l_iBuf3[3] = 2;
		l_iBuf3[4] = 2;		l_iBuf3[5] = 3;

		objs[i] = new Object(vCnt3, vBuf3, pCnt3, t_iBuf3, lCnt3, l_iBuf3);
		if (objs[i] == nullptr) return false;
	}

	return true;
}
int ModelClass::GetObjCount() { return objCount; }


// PA2
ID3D11ShaderResourceView* ModelClass::GetTexture(int modelnum)
{
	return models[modelnum]->GetTexture()->GetTexture();
}
const XMFLOAT3 ModelClass::GetModelScale(int num) { return models[num]->GetScale(); }
const XMFLOAT3 ModelClass::GetModelPosition(int num) { return models[num]->GetPosition(); }
