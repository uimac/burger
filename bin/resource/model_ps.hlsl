#include "common.hlsl"

// pixel shader
float4 PS_Main( VS_MODEL_OUT In ) : SV_TARGET
{
	// todo
	float4 uv_color = float4(1, 1, 1, 1);
	if (material.shader_flags.x > 0)
	{
		uv_color = g_texture.Sample( g_sampler, In.uv );
	}
	
	float3 nl = saturate(dot(In.normal, -In.light_direction));
	float3 reflection = normalize(2 * nl * In.normal + In.light_direction);
	// todo
	float3 view_direction = normalize(float3 (0, 0, 0));
	return uv_color * phong(material, nl, reflection, view_direction);
}
