#pragma once
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <directxmath.h>

using namespace DirectX;


class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(XMFLOAT4);	
	void SetDiffuseColor(float, float, float, float);
	void SetDiffuseColor(XMFLOAT4);
	void SetSpecularColor(XMFLOAT4);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetIntensity(float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetSpecularColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetPosition();
	float GetIntensity();
	float GetSpecularPower();


private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_specularColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_position;
	float m_intensity;
	float m_specularPower;
};


#endif