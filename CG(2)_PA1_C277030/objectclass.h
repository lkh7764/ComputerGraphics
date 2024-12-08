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
	// 버텍스가 가질 정보, 더 추가해줄 수 있다

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
	int				o_vertexCount;			// vertex 개수
	VertexType_2d*	o_vertexBuffer;			// vertex 정보 array

	// index buffers
	int				o_indexCount;			// draw 시 필요한 index 개수, t_indexCount 또는 l_indexCount 값이 들어감
	int				t_indexCount;			// draw mode triangle 시 필요한 index 개수
	int				l_indexCount;			// draw mode line 시 필요한 index 개수

	unsigned long*	o_indexBuffer;			// draw 시 필요한 index array, t 또는 l을 가리킴
	unsigned long*	t_indexBuffer;			// draw mode trianlge 시 필요한 index array
	unsigned long*	l_indexBuffer;			// draw mode line 시 필요한 index array
};


#endif

