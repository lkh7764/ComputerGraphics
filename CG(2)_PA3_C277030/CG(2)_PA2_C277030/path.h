#pragma once

// shader path
#define PATH_VS			L"./data/texture.vs"
#define PATH_PS			L"./data/texture.ps"

// obj path
#define O_PATH1			L"./data/obj_banana/Banana_tris.obj"
#define T_PATH1			L"./data/obj_banana/b_basecolor.dds"

#define O_PATH2			L"./data/obj_mercedes/mercedes_tirs.obj"
#define T_PATH2			L"./data/obj_mercedes/m_texture.dds"

#define O_PATH3			L"./data/obj_tesla/tesla_tris.obj"
#define T_PATH3			L"./data/obj_tesla/t_texture.dds"

#define O_PATH4			L"./data/obj_bunny/bunny.obj"
#define T_PATH4			L"./data/obj_bunny/bunny_texture.dds"

#define O_PATH5			L"./data/obj_penguin/penguin.obj"
#define T_PATH5			L"./data/obj_penguin/penguin_texture.dds"

#define O_PATH_PLANE	L"./data/obj_plane/plane.obj"
#define T_PATH_PLANE	L"./data/obj_plane/w_texture.dds"

#define T_PATH_BACK		L"./data/obj_plane/back.dds"


// obj scale
#define SCALE1			XMFLOAT3(2.0f, 2.0f, 2.0f)
#define SCALE2			XMFLOAT3(0.15f, 0.15f, 0.15f)
#define SCALE3			XMFLOAT3(0.05f, 0.05f, 0.05f)
#define SCALE_DEFAULT	XMFLOAT3(1.0f, 1.0f, 1.0f)

// obj position
#define POSITION1		XMFLOAT3(8.0f, 5.0f, 5.0f)
#define POSITION2		XMFLOAT3(-15.0f, 0.0f, 15.0f)
#define POSITION3		XMFLOAT3(3.0f, 0.0f, -5.0f)

#define POSITION_DEFAULT	XMFLOAT3(0.0f, 0.0f, 0.0f)

#define POSITION_PLANE	XMFLOAT3(0.0f, 0.0f, -40.0f)
#define POSITION_PLANE1	XMFLOAT3(0.0f, 0.0f, 75.0f)
#define POSITION_PLANE2	XMFLOAT3(0.0f, 0.0f, 150.0f)


// pa3
#define COLOR_RED		XMFLOAT4(0.95f, 0.4f, 0.4f, 1.0f)
#define COLOR_GREEN		XMFLOAT4(0.5f, 0.7f, 0.2f, 1.0f)
#define COLOR_BLUE		XMFLOAT4(0.2f, 0.5f, 0.7f, 1.0f)
#define COLOR_DEFAULT	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_AMBIENT	XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f)


#define FMODE_POINT		D3D11_FILTER_MIN_MAG_MIP_POINT
#define FMODE_LINEAR	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT
#define FMODE_ANISO		D3D11_FILTER_ANISOTROPIC


#define FONT_DATA		L"./data/font/fontdata.txt"
#define FONT_DDS		L"./data/font/font.dds"
#define FONT_PS			L"./data/font/font.ps"
#define FONT_VS			L"./data/font/font.vs"