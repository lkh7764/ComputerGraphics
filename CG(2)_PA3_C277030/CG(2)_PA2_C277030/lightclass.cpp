#include "lightclass.h"


LightClass::LightClass()
{
}
LightClass::LightClass(const LightClass& other)
{
}
LightClass::~LightClass()
{
}


void LightClass::SetAmbientColor(XMFLOAT4 color)
{
	m_ambientColor = color;
}
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}
void LightClass::SetDiffuseColor(XMFLOAT4 color)
{
	m_diffuseColor = color;
	return;
}
void LightClass::SetSpecularColor(XMFLOAT4 color)
{
	m_specularColor = color;
}
void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}
void LightClass::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT4(x, y, z, 1.0f);
	return;
}
void LightClass::SetIntensity(float value)
{
	m_intensity = value;
	return;
}
void LightClass::SetSpecularPower(float value)
{
	m_specularPower = value;
}


XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}
XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}
XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}
XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}
XMFLOAT4 LightClass::GetPosition()
{
	return m_position;
}
float LightClass::GetIntensity() 
{ 
	return m_intensity; 
}
float LightClass::GetSpecularPower()
{
	return m_specularPower;
}
