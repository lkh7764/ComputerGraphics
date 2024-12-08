#ifndef _OBJCLASS_H_
#define _OBJCLASS_H_

#pragma once

#include <d3d11.h>
#include <directxmath.h>

#define TRIANGLE	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
#define LINE		D3D11_PRIMITIVE_TOPOLOGY_LINELIST

using namespace DirectX;


struct VertexType_2d
{
	// ���ؽ��� ���� ����, �� �߰����� �� �ִ�

	XMFLOAT3 position;
	XMFLOAT4 color;
};


class Object
{
public:
	Object(int, VertexType_2d*, int, unsigned long*, int, unsigned long*);
	~Object();

	int GetVertexCount();
	int GetIndexCount();

	const VertexType_2d* GetVertexBuffer();
	const unsigned long* GetIndexBuffer();

	void SetIndexBuffer(D3D_PRIMITIVE_TOPOLOGY);
	void Reverse();


private:
	// vertex buffer
	int				o_vertexCount;			// vertex ����
	VertexType_2d*	o_vertexBuffer;			// vertex ���� array

	// index buffers
	int				o_indexCount;			// draw �� �ʿ��� index ����, t_indexCount �Ǵ� l_indexCount ���� ��
	int				t_indexCount;			// draw mode triangle �� �ʿ��� index ����
	int				l_indexCount;			// draw mode line �� �ʿ��� index ����

	unsigned long*	o_indexBuffer;			// draw �� �ʿ��� index array, t �Ǵ� l�� ����Ŵ
	unsigned long*	t_indexBuffer;			// draw mode trianlge �� �ʿ��� index array
	unsigned long*	l_indexBuffer;			// draw mode line �� �ʿ��� index array
};


#endif

