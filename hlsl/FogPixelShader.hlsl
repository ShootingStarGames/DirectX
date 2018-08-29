Texture2D shaderTextures;
SamplerState SampleType;

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 textureColor;
	float4 fogColor;
	float4 finalColor;

	textureColor = shaderTextures.Sample(SampleType, input.tex);
	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

	finalColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;

	return finalColor;
}