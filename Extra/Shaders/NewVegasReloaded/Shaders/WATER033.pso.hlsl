//
// Generated by Microsoft (R) HLSL Shader Compiler 9.23.949.2378
//
// Parameters:

// LOD autowater surface shader

float4 EyePos : register(c1);
float4 ShallowColor : register(c2);
float4 DeepColor : register(c3);
float4 ReflectionColor : register(c4);
float4 FresnelRI : register(c5);
float4 VarAmounts : register(c8);
float4 FogParam : register(c9);
float4 FogColor : register(c10);
float4 SunDir : register(c12);
float4 SunColor : register(c13);
float4 TESR_WaveParams : register(c14);

sampler2D ReflectionMap : register(s0);

// Registers:
//
//   Name            Reg   Size
//   --------------- ----- ----
//   EyePos          const_1       1
//   ShallowColor    const_2       1
//   DeepColor       const_3       1
//   ReflectionColor const_4       1
//   FresnelRI       const_5       1
//   VarAmounts      const_8       1
//   FogParam        const_9       1
//   FogColor        const_10      1
//   SunDir          const_12      1
//   SunColor        const_13      1
//   ReflectionMap   texture_0       1
//


// Structures:

struct VS_INPUT {
    float3 LTEXCOORD_0 : TEXCOORD0_centroid;
    float3 LTEXCOORD_1 : TEXCOORD1_centroid;
    float4 LTEXCOORD_2 : TEXCOORD2_centroid;
    float4 LTEXCOORD_3 : TEXCOORD3_centroid;
    float4 LTEXCOORD_4 : TEXCOORD4_centroid;
    float4 LTEXCOORD_5 : TEXCOORD5_centroid;
    float4 LTEXCOORD_6 : TEXCOORD6;
};

struct VS_OUTPUT {
    float4 color_0 : COLOR0;
};

// Code:

VS_OUTPUT main(VS_INPUT IN) {
    VS_OUTPUT OUT;

#define	shade(n, l)		max(dot(n, l), 0)
#define	shades(n, l)		saturate(dot(n, l))
#define	weight(v)		dot(v, 1)
#define	sqr(v)			((v) * (v))

    const float4 const_0 = {0, -4096, -0.000244140625, 1};
    const float4 const_6 = {0, 2, 2.40649434e-009, 0};

    float4 q0;
    float1 q1;
    float3 q10;
    float1 q104;
	float3 q21;
    float3 q16;
    float3 q3;
    float1 q4;
    float4 q6;
    float1 q9;
    float3 r0;
    float4 r1;
    float3 r2;
    float3 t7;
	float reflectivity = TESR_WaveParams.w;
	
    r1.yz = VarAmounts.yz;
    r0.xyz = EyePos.xyz - IN.LTEXCOORD_0.xyz;
    q1.x = 1 - saturate((length(r0.xy) - 4096) * 0.000244140625);
    q9.x = saturate(VarAmounts.z - (q1.x * (1 - r1.z)));
    q4.x = 1 - saturate((FogParam.x - length(r0.xyz)) / FogParam.y);
    r0.xyz = normalize(r0.xyz);
    r1.w = 1 - saturate(r0.z);
    r0.xyz = (r0.z * const_6.xxy) - r0.xyz;
    q104.x = q9.x * 0.5 * lerp(FresnelRI.x, 1, r1.w * sqr(sqr(r1.w)));
    q3.xyz = (pow(abs(shades(r0.xyz, SunDir.xyz)), VarAmounts.x) + 2.40649434e-009) * SunColor.rgb;
    q0.xyzw = (IN.LTEXCOORD_1.xyzx * const_0.wwwx) + const_0.xxxw;
    q6.xyzw = mul(float4x4(IN.LTEXCOORD_2.xyzw, IN.LTEXCOORD_3.xyzw, IN.LTEXCOORD_4.xyzw, IN.LTEXCOORD_5.xyzw), q0.xyzw);
    t7.xyz = tex2Dproj(ReflectionMap, q6.xyzw).xyz;
	q10.xyz = (q9.x * (DeepColor.rgb - ShallowColor.rgb)) + ShallowColor.rgb;
	r2.xyz = (r1.y * (t7.xyz - ReflectionColor.rgb)) + ReflectionColor.xyz;
	q21.xyz = (pow(abs(q4.x), FresnelRI.y) * (FogColor.rgb - r2.xyz)) + r2.xyz;
	q10.xyz = (q9.x * (q10.xyz - q21.xyz)) + q21.xyz;
    q16.xyz = (SunDir.w * q3.xyz) + ((q104.x * ((FresnelRI.w * reflectivity * r2.xyz) - q10.xyz)) + q10.xyz);
    OUT.color_0.a = q9.x * IN.LTEXCOORD_6.w;
    OUT.color_0.rgb = (pow(abs(q4.x), FresnelRI.y) * (FogColor.rgb - q16.xyz)) + q16.xyz;
    return OUT;
	
};