//
// Generated by Microsoft (R) D3DX9 Shader Compiler 9.08.299.0000
//
// Parameters:

float4 AmbientColor : register(c1);
float4 PSLightColor[4] : register(c2);
float4 TESR_TerrainData : register(c6);
float4 TESR_ShadowData : register(c7);

sampler2D BaseMap : register(s0);
sampler2D NormalMap : register(s1);
sampler2D TESR_ShadowMapBufferNear : register(s2) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferFar : register(s3) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };

// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   AmbientColor const_1       1
//   PSLightColor[0] const_2        1
//   BaseMap      texture_0       1
//   NormalMap    texture_1       1
//


// Structures:

struct VS_INPUT {
    float2 BaseUV : TEXCOORD0;
    float2 NormalUV : TEXCOORD1;
    float3 texcoord_2 : TEXCOORD2_centroid;
    float3 texcoord_3 : TEXCOORD3_centroid;
    float4 texcoord_6 : TEXCOORD6;
	float4 texcoord_7 : TEXCOORD7;
	float4 color_0 : COLOR0;
};

struct VS_OUTPUT {
    float4 color_0 : COLOR0;
};

#include "Includes\Shadow.hlsl"

VS_OUTPUT main(VS_INPUT IN) {
    VS_OUTPUT OUT;

#define	expand(v)		(((v) - 0.5) / 0.5)
#define	compress(v)		(((v) * 0.5) + 0.5)
#define	shade(n, l)		max(dot(n, l), 0)
#define	shades(n, l)	saturate(dot(n, l))

    float3 noxel0;
    float3 q1;
    float4 r0;
	float spclr;
	float3 q5;
	
	r0.xyzw = tex2D(BaseMap, IN.BaseUV.xy);
    noxel0.xyz = tex2D(NormalMap, IN.NormalUV.xy).xyz;
    q1.xyz = GetLightAmount(IN.texcoord_6, IN.texcoord_7) * shades(normalize(expand(noxel0.xyz)), expand(IN.texcoord_3.xyz)) * PSLightColor[0].rgb + AmbientColor.rgb;
	spclr = smoothstep(0.0f, 0.25f, length(r0.rgb)) * (r0.b * 2.0f * TESR_TerrainData.z) + 1.0f;
	q5.xyz = q1.xyz * r0.xyz * IN.texcoord_2.xyz * spclr;
    OUT.color_0.a = 1;
    OUT.color_0.rgb = (IN.color_0.w * (IN.color_0.xyz - q5.xyz)) + q5.xyz;
    return OUT;
	
};

// approximately 16 instruction slots used (2 texture, 14 arithmetic)