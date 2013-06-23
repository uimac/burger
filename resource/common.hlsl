// world x view x projection
cbuffer cbMatrixWVP : register( b0 )
{
   matrix WorldViewProjection;
};

Texture2D g_texture : register( t0 );
SamplerState g_sampler : register( s0 );

// vertex shader input
struct VS_IN
{
	float3 pos : POSITION;   // vertex position
	//float4 color : COLOR;      // vertex color
	float2 uv : TEXCOORD;   // texture
};

// vertex shader output
struct VS_OUT
{
   float4 pos   : SV_POSITION;
   //float4 color : COLOR0;
   float2 uv : TEXCOORD0;
};