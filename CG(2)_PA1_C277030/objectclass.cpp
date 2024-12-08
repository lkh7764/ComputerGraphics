#include "objectclass.h"


Object::Object(int vCnt, VertexType_2d* vBuf, int pCnt, unsigned long* t_iBuf, int lCnt, unsigned long* l_iBuf)
{
	// vCnt, 오브젝트를 구성하는 버텍스 개수
	// vBuf, 복사할 버텍스array의 주소값
	// pCnt, 오브젝트를 구성하는 폴리곤 개수, t_indexCnt <- pCnt*3
	// t_iBuf, 복사할 인덱스array의 주소값, drawMode Triangle 시 사용
	// lCnt, 오브젝트를 구성하는 선 개수, l_indexCnt <- vCnt*2
	// l_iBuf, 복사할 인덱스array의 주소값, drawMode Line 시 사용

	// set buffer - vertex
	o_vertexCount = vCnt;
	o_vertexBuffer = new VertexType_2d[o_vertexCount];
	memcpy(o_vertexBuffer, vBuf, sizeof(VertexType_2d) * o_vertexCount);

	// set buffer - index triangle(solid) 
	t_indexCount = pCnt * 3;
	t_indexBuffer = new unsigned long[t_indexCount];
	memcpy(t_indexBuffer, t_iBuf, sizeof(unsigned long) * t_indexCount);

	// set buffer - index line(wireframe)
	l_indexCount = vCnt * 2;
	l_indexBuffer = new unsigned long[l_indexCount];
	memcpy(l_indexBuffer, l_iBuf, sizeof(unsigned long) * l_indexCount);

	// set default draw mode - triangle
	SetIndexBuffer(TRIANGLE);
}

Object::~Object()
{
	delete o_vertexBuffer;
	delete o_indexBuffer;
	delete t_indexBuffer;
	delete l_indexBuffer;

	o_vertexBuffer = 0;
	o_indexBuffer = 0;
	t_indexBuffer = 0;
	l_indexBuffer = 0;
}


int Object::GetVertexCount() { return o_vertexCount; }
int Object::GetIndexCount() { return o_indexCount; }

const VertexType_2d* Object::GetVertexBuffer() { return o_vertexBuffer; }
const unsigned long* Object::GetIndexBuffer() { return o_indexBuffer; }


void Object::Reverse()
{
	if (o_indexBuffer == l_indexBuffer) return;

	int pCnt = o_indexCount / 3;
	for (int i = 0; i < pCnt; i++)
	{
		unsigned long temp = o_indexBuffer[3 * i + 1];
		o_indexBuffer[3 * i + 1] = o_indexBuffer[3 * i + 2];
		o_indexBuffer[3 * i + 2] = temp;
	}
}


void Object::SetIndexBuffer(D3D_PRIMITIVE_TOPOLOGY drawType)
{
	switch (drawType)
	{
	case TRIANGLE:
		// 1 triangle 3 vertex
		o_indexCount = t_indexCount;
		o_indexBuffer = t_indexBuffer;
		break;

	case LINE:
		o_indexCount = l_indexCount;
		o_indexBuffer = l_indexBuffer;
		break;
	}
}