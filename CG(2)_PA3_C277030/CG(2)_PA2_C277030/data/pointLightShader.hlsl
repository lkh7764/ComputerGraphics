// DEFINES //
#define NUM_LIGHTS 1

// GLOBALS //
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

cbuffer LightColorBuffer
{
	float4 diffuseColor[NUM_LIGHTS];
};

cbuffer LightIntensityBuffer
{
    float4 intensity[NUM_LIGHTS];
};

cbuffer LightSettingBuffer
{
    float4	ambientColor;
    float4	specularColor;
    float	specularPower;
	float	active_ambient;
    float   active_diffuse;
    float   active_specular;
};

Texture2D shaderTexture;
SamplerState SampleType;

// TYPEDEFS //
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 instancePosition : TEXCOORD1;
    float  instanceRotation : TEXCOORD2;
};

struct PixelInputType
{
    float4	position : SV_POSITION;
    float2	tex : TEXCOORD0;
	float3	normal : NORMAL;
	float3	lightPos1 : TEXCOORD1;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
   	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;
	
    float r = radians(input.instanceRotation);
    float cosr = cos(r);
    float sinr = sin(r);
    float4x4 rotate4 = float4x4(
        cosr,   0,      sinr,   0, 
        0,      1,      0,      0, 
        -sinr,  0,      cosr,   0, 
        0,      0,      0,      1);
    float3x3 rotate3 = float3x3(
        cosr,   0,      sinr, 
        0,      1,      0, 
        -sinr,  0,      cosr);
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, rotate4);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, rotate3);
	output.normal = mul(output.normal, (float3x3)worldMatrix);
	
	// Normalize the normal vector.
	output.normal = normalize(output.normal);
	
    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;

    // Normalize the light position vectors.
    output.lightPos1 = normalize(output.lightPos1);

    return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	// Sample the texture pixel at this location.
    float4 textureColor;
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
    // ambient
    float4 color;
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (active_ambient == 0.0f) color = ambientColor;
    
    // specular
    float4 specular;
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
	// Calculate the different amounts of light on this pixel based on the positions of the lights.
    float lightIntensity;
    lightIntensity = saturate(dot(input.normal, input.lightPos1)) * intensity[0];
	
    float reflection;
    float lightDir = -normalize(input.lightPos1);
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        if (active_diffuse == 0.0f) color += (diffuseColor[0] * lightIntensity);

	    // Saturate the ambient and diffuse color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.lightPos1)), specularPower);
    }

    color = color * textureColor;

	// Multiply the texture pixel by the combination of all four light colors to get the final result.
    if (active_specular == 0.0f) color = saturate(color + specular);
	
    return color;
}
