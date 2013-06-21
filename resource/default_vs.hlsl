#include "common.hlsl"

// vertex shader
VS_OUT VS_Main( VS_IN In )
{
   VS_OUT Out;

   Out.pos = mul( float4( In.pos, 1 ), WorldViewProjection );

   Out.color = float4(0, 0, 1, 1);//In.color;

   return Out;
}
