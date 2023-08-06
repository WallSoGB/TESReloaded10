// Snow accumulation fullscreen shader for Oblivion Reloaded

float4x4 TESR_WorldViewProjectionTransform;
float4x4 TESR_ShadowCameraToLightTransformOrtho;
float4 TESR_SunDirection;
float4 TESR_SunColor;
float4 TESR_SunAmbient;
float4 TESR_ReciprocalResolution;
float4 TESR_SnowAccumulationParams;
float4 TESR_WaterSettings;
float4 TESR_OrthoData;
float4 TESR_DebugVar;

float4 TESR_ShadowLightPosition0;
float4 TESR_ShadowLightPosition1;
float4 TESR_ShadowLightPosition2;
float4 TESR_ShadowLightPosition3;
float4 TESR_ShadowLightPosition4;
float4 TESR_ShadowLightPosition5;
float4 TESR_ShadowLightPosition6;
float4 TESR_ShadowLightPosition7;
float4 TESR_ShadowLightPosition8;
float4 TESR_ShadowLightPosition9;
float4 TESR_ShadowLightPosition10;
float4 TESR_ShadowLightPosition11;
float4 TESR_LightPosition0;
float4 TESR_LightPosition1;
float4 TESR_LightPosition2;
float4 TESR_LightPosition3;
float4 TESR_LightPosition4;
float4 TESR_LightPosition5;
float4 TESR_LightPosition6;
float4 TESR_LightPosition7;
float4 TESR_LightPosition8;
float4 TESR_LightPosition9;
float4 TESR_LightPosition10;
float4 TESR_LightPosition11;


sampler2D TESR_RenderedBuffer : register(s0) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_SourceBuffer : register(s1) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_DepthBuffer : register(s2) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_OrthoMapBuffer : register(s3) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_NormalsBuffer : register(s4) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_PointShadowBuffer : register(s5) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_SnowNormSampler : register(s6) < string ResourceName = "Precipitations\snow_NRM.dds"; > = sampler_state { ADDRESSU = WRAP; ADDRESSV = WRAP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_BlueNoiseSampler : register(s7) < string ResourceName = "Effects\bluenoise256.dds"; > = sampler_state { ADDRESSU = WRAP; ADDRESSV = WRAP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };

static const int KernelSize = 24;
static const float BIAS = 0.003f;
static const float diffusePower = 0.3f;
static const float specularPower = 0.5f;
static const float fresnelPower = 0.2f;

static const float BlurNormalsWeights[KernelSize] = 
{
	0.019956226f,
	0.021463016f,
	0.032969806f,
	0.044476596f,
	0.055983386f,
	0.067490176f,
	0.078996966f,
	0.080503756f,
	0.092010546f,
	0.105024126f,
	0.116530916f,
	0.128037706f,
	0.128037706f,
	0.116530916f,
	0.105024126f,
	0.092010546f,
	0.080503756f,
	0.078996966f,
	0.067490176f,
	0.055983386f,
	0.044476596f,
	0.032969806f,
	0.021463016f,
	0.019956226f
};

static const float2 BlurNormalsOffsets[KernelSize] = 
{
	float2(-12.0f * TESR_ReciprocalResolution.x, -12.0f * TESR_ReciprocalResolution.y),
	float2(-11.0f * TESR_ReciprocalResolution.x, -11.0f * TESR_ReciprocalResolution.y),
	float2(-10.0f * TESR_ReciprocalResolution.x, -10.0f * TESR_ReciprocalResolution.y),
	float2( -9.0f * TESR_ReciprocalResolution.x,  -9.0f * TESR_ReciprocalResolution.y),
	float2( -8.0f * TESR_ReciprocalResolution.x,  -8.0f * TESR_ReciprocalResolution.y),
	float2( -7.0f * TESR_ReciprocalResolution.x,  -7.0f * TESR_ReciprocalResolution.y),
	float2( -6.0f * TESR_ReciprocalResolution.x,  -6.0f * TESR_ReciprocalResolution.y),
	float2( -5.0f * TESR_ReciprocalResolution.x,  -5.0f * TESR_ReciprocalResolution.y),
	float2( -4.0f * TESR_ReciprocalResolution.x,  -4.0f * TESR_ReciprocalResolution.y),
	float2( -3.0f * TESR_ReciprocalResolution.x,  -3.0f * TESR_ReciprocalResolution.y),
	float2( -2.0f * TESR_ReciprocalResolution.x,  -2.0f * TESR_ReciprocalResolution.y),
	float2( -1.0f * TESR_ReciprocalResolution.x,  -1.0f * TESR_ReciprocalResolution.y),
	float2(  1.0f * TESR_ReciprocalResolution.x,   1.0f * TESR_ReciprocalResolution.y),
	float2(  2.0f * TESR_ReciprocalResolution.x,   2.0f * TESR_ReciprocalResolution.y),
	float2(  3.0f * TESR_ReciprocalResolution.x,   3.0f * TESR_ReciprocalResolution.y),
	float2(  4.0f * TESR_ReciprocalResolution.x,   4.0f * TESR_ReciprocalResolution.y),
	float2(  5.0f * TESR_ReciprocalResolution.x,   5.0f * TESR_ReciprocalResolution.y),
	float2(  6.0f * TESR_ReciprocalResolution.x,   6.0f * TESR_ReciprocalResolution.y),
	float2(  7.0f * TESR_ReciprocalResolution.x,   7.0f * TESR_ReciprocalResolution.y),
	float2(  8.0f * TESR_ReciprocalResolution.x,   8.0f * TESR_ReciprocalResolution.y),
	float2(  9.0f * TESR_ReciprocalResolution.x,   9.0f * TESR_ReciprocalResolution.y),
	float2( 10.0f * TESR_ReciprocalResolution.x,  10.0f * TESR_ReciprocalResolution.y),
	float2( 11.0f * TESR_ReciprocalResolution.x,  11.0f * TESR_ReciprocalResolution.y),
	float2( 12.0f * TESR_ReciprocalResolution.x,  12.0f * TESR_ReciprocalResolution.y)
};

struct VSOUT
{
	float4 vertPos : POSITION;
	float2 UVCoord : TEXCOORD0;
};

struct VSIN
{
	float4 vertPos : POSITION0;
	float2 UVCoord : TEXCOORD0;
};

VSOUT FrameVS(VSIN IN)
{
	VSOUT OUT = (VSOUT)0.0f;
	OUT.vertPos = IN.vertPos;
	OUT.UVCoord = IN.UVCoord;
	return OUT;
}

#include "Includes/Helpers.hlsl"
#include "Includes/Depth.hlsl"
#include "Includes/BlurDepth.hlsl"
#include "Includes/Normals.hlsl"

float4 GetNormals(VSOUT IN) : COLOR0
{
	return float4(compress(GetWorldNormal(IN.UVCoord)), 1.0);
}


float GetOrtho(float4 worldPos){
	float thickness = BIAS; // thickness of the valid areas around the ortho map depth that will receive the effect (cancels out too far above or below ortho value)

	// get puddle mask from ortho map
	float4 pos = mul(worldPos, TESR_WorldViewProjectionTransform);
	float4 ortho_pos = mul(pos, TESR_ShadowCameraToLightTransformOrtho);

	// apply perspective (perspective division) and convert from -1/1 to range to 0/1 (shadowMap range);
	ortho_pos.xyz /= ortho_pos.w;
	ortho_pos.x = ortho_pos.x * 0.5f + 0.5f;
	ortho_pos.y = ortho_pos.y * -0.5f + 0.5f;

	float outOfBounds = (saturate(ortho_pos.x) != ortho_pos) || (saturate(ortho_pos.y) != ortho_pos.y);

	float ortho = tex2D(TESR_OrthoMapBuffer, ortho_pos.xy).r; // ortho height

	float aboveGround = ortho_pos.z < ortho + thickness;
	float belowGround = ortho_pos.z > ortho - thickness;

	return outOfBounds || belowGround;
}


float4 BlurNormals(VSOUT IN, uniform float2 OffsetMask) : COLOR0
{
	float WeightSum = 0.12f * saturate(1 - TESR_SnowAccumulationParams.x);
	float3 normal = tex2D(TESR_RenderedBuffer,IN.UVCoord).rgb;
	float3 finalNormal = normal * WeightSum;
	float depth = readDepth(IN.UVCoord)/farZ;
	
	for (int i = 0; i < KernelSize; i++) {
		float2 uvOff = (BlurNormalsOffsets[i] * OffsetMask) * TESR_SnowAccumulationParams.y * abs(log(depth));
		float3 newNormal = tex2D(TESR_RenderedBuffer, IN.UVCoord + uvOff).rgb;
		float weight = BlurNormalsWeights[i] * saturate(dot(expand(newNormal), expand(normal)) - TESR_SnowAccumulationParams.x * 0.75f);
		finalNormal += weight * newNormal;
		WeightSum += weight;
	}
	
	finalNormal /= WeightSum;
    return float4(finalNormal, 1.0f);
}


// returns a point light contribution with no shadow map sampling
float GetPointLightContribution(float4 worldPos, float4 LightPos, float4 normal){

	float3 LightDir = LightPos.xyz - worldPos.xyz;
	float Distance = length(LightDir) / LightPos.w; // normalize distance over light range
	float4 light = float4(LightDir, Distance);

	// radius based attenuation based on https://lisyarus.github.io/blog/graphics/2022/07/30/point-light-attenuation.html
	float s = Distance * Distance; 
	float atten = saturate((1 - s) / (1 + s));

	LightDir = normalize(LightDir); // normalize
	float diffuse = dot(LightDir, normal.xyz);

	return saturate(diffuse * atten);
}

float4 Snow( VSOUT IN ) : COLOR0
{
	float4 color = tex2D(TESR_SourceBuffer, IN.UVCoord);
	float3 world = toWorld(IN.UVCoord);
	
	float depth = readDepth(IN.UVCoord);
	float3 camera_vector = world * depth;
	float3 eyeDirection = -1 * normalize(world);
	float4 worldPos = float4(TESR_CameraPosition.xyz + camera_vector, 1.0f);

	// early out for the character gun, water surfaces/areas and 
	if (depth < 50 || worldPos.z <= (TESR_WaterSettings.x + BIAS) || !GetOrtho(worldPos)) return color;
	
	float2 uv = worldPos.xy / 200.0f;
	float3 norm = normalize(expand(tex2D(TESR_RenderedBuffer, IN.UVCoord).rgb));
	float3 localNorm = expand(tex2D(TESR_SnowNormSampler, uv).xyz);
	float3 surfaceNormal = normalize(float3(localNorm.xy + norm.xy, localNorm.z * norm.z));

	float3 snow_tex = float3(1, 1, 1);

	float fresnelCoeff = saturate(pow(1 - shade(eyeDirection, surfaceNormal), 5));
	float3 snowSpec = pows(shades(normalize(TESR_SunDirection.xyz + eyeDirection), surfaceNormal), 20) * fresnelCoeff;

	float3 ambient = snow_tex * TESR_SunAmbient.rgb;
	float shadow = tex2D(TESR_PointShadowBuffer, IN.UVCoord);
	float3 diffuse = snow_tex * shade(TESR_SunDirection.xyz, surfaceNormal) * TESR_SunColor.rgb * diffusePower * shadow;
	float3 spec = snowSpec * TESR_SunColor.rgb * specularPower * shadow;
	float3 fresnel = fresnelCoeff * TESR_SunColor.rgb * fresnelPower;
	float3 sparkles = pows(shades(eyeDirection, normalize(expand(tex2D(TESR_BlueNoiseSampler, worldPos.xy / 200).rgb))), 1000) * 0.4;

	float4 snowColor = float4(ambient + diffuse + spec + fresnel + sparkles, 1);

	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition0, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition1, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition2, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition3, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition4, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition5, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition6, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition7, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition8, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition9, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_ShadowLightPosition10, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition0, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition1, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition2, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition3, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition4, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition5, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition6, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition7, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition8, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition9, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition10, float4(surfaceNormal, 1)) * shadow;
	snowColor += GetPointLightContribution(worldPos, TESR_LightPosition11, float4(surfaceNormal, 1)) * shadow;

	color = lerp(color, snowColor, shade(float3(0, 0, 1), norm));

    return color;
}

technique
{
	pass
	{
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 GetNormals();
	}
	pass
	{ 
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 BlurNormals(OffsetMaskH);
	}
	pass
	{ 
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 BlurNormals(OffsetMaskV);
	}
	pass
	{
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 Snow();
	}
}
