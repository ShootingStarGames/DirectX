cbuffer MatrixBuffer
{
   matrix worldMatrix;
   matrix viewMatrix;
   matrix projectionMatrix;
};

struct VS_INPUT
{
   float4 position :POSITION;
   float2 tex : TEXCOORD0;
   float3 normal :NORMAL;
};

struct VS_OUTPUT
{
   float4 position :SV_POSITION;
   float2 tex :TEXCOORD0;
   float3 normal :NORMAL;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
   VS_OUTPUT output;
   
   input.position.w = 1.0f;
   
   output.position = mul(input.position,worldMatrix);
   output.position = mul(output.position,viewMatrix);
   output.position = mul(output.position,projectionMatrix);

   output.tex = input.tex;

   output.normal = mul(input.normal,(float3x3)worldMatrix);

   output.normal = normalize(output.normal);

   return output;
}