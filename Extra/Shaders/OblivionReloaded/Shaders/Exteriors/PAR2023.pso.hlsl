//
// Generated by Microsoft (R) D3DX9 Shader Compiler 9.08.299.0000
//
// Parameters:

float4 PSLightColor[4] : register(c2);
float4 Toggles : register(c7);
float4 TESR_ShadowData : register(c8);
float4 TESR_ParallaxData : register(c9);

sampler2D NormalMap : register(s0);
sampler2D BaseMap : register(s1);
sampler2D TESR_ShadowMapBufferNear : register(s6) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferFar : register(s7) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };

// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   PSLightColor[0] const_2        1
//   Toggles      const_7       1
//   NormalMap    texture_0       1
//   BaseMap      texture_1       1
//


// Structures:

struct VS_INPUT {
    float2 BaseUV : TEXCOORD0;
    float3 texcoord_1 : TEXCOORD1_centroid;
    float3 texcoord_3 : TEXCOORD3_centroid;
	float4 texcoord_5 : TEXCOORD5;
    float3 texcoord_6 : TEXCOORD6_centroid;
	float4 texcoord_7 : TEXCOORD7;
};

struct VS_OUTPUT {
    float4 color_0 : COLOR0;
};

#include "..\Includes\PAR.hlsl"
#include "..\Includes\Shadow.hlsl"

VS_OUTPUT main(VS_INPUT IN) {
    VS_OUTPUT OUT;

#define	expand(v)		(((v) - 0.5) / 0.5)
#define	uvtile(w)		(((w) * 0.04) - 0.02)
#define	shades(n, l)	saturate(dot(n, l))
#define	weight(v)		dot(v, 1)

	float3 q3;
    float1 q4;
	float1 q5;
    float3 q6;
    float4 r0;
    float3 noxel2;
	float2 uv;
	
	uv.xy = ParallaxMapping(IN.BaseUV, IN.texcoord_6);
    r0.xyzw = tex2D(BaseMap, uv.xy);
	noxel2.xyz = tex2D(NormalMap, uv.xy).xyz;
	q3.xyz = normalize(expand(noxel2.xyz));
    q5.x = pow(abs(shades(q3.xyz, normalize(IN.texcoord_3.xyz))), Toggles.z);
    q4.x = dot(q3.xyz, IN.texcoord_1.xyz);
    q6.xyz = GetLightAmount(IN.texcoord_5, IN.texcoord_7) * (0.2 >= q4.x ? (q5.x * max(q4.x + 0.5, 0)) : q5.x) * PSLightColor[0].rgb;
    OUT.color_0.a = weight(q6.xyz);
    OUT.color_0.rgb = saturate(q6.xyz);
    return OUT;
	
};

// approximately 33 instruction slots used (2 texture, 31 arithmetic)