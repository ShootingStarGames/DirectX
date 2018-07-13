cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrx;
}

struct VS_INPUT
{
	float4 position :POSITON;
	float2 tex : TEXCOORD0;
	float3 normal :NORMAL;
}

struct VS_OUTPUT
{
	float4 position :SV_POSITION;
	float2 tex :TEXCOORD0;
	float3 normal :NORMAL;
}

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUPUT ouput;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position,worldMatrix);
	output.position = mul(output.position,viewMatrix);
	output.position = mul(output.position,projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal,(float3x3)worldMatrix);

	output.normal = nomalize(output.normal);

	return output;
}