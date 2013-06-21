// world x view x projection
cbuffer cbMatrixWVP : register( b0 )
{
   matrix WorldViewProjection;
};


// vertex shader input
struct VS_IN
{
   float3 pos   : POSITION;   // vertex position
   float4 color : COLOR;      // vertex color
};

// vertex shader output
struct VS_OUT
{
   float4 pos   : SV_POSITION;
   float4 color : COLOR0;
};