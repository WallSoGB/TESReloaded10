// Volumetric Fog fullscreen shader for Oblivion/Skyrim Reloaded

float4 TESR_FogColor;
float4 TESR_FogData;
float4 TESR_VolumetricFogData;
float4 TESR_ReciprocalResolution;
float4 TESR_SunDirection;
float4 TESR_SunColor;
float4 TESR_SunAmbient;
float4 TESR_HorizonColor;

sampler2D TESR_RenderedBuffer : register(s0) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_DepthBuffer : register(s1) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };

/*Height-based fog settings*/
static const float fogHeight = 4096; 
static const float NOISE_SCALE = 4.2;
static const float sunScatter = 8.0; //raise to decrease the intensity of sun fog
static const float FOG_GROUND =	15000;
static const float FOG_HEIGHT = 25000;


#include "Includes/Depth.hlsl"

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

// returns a value from 0 to 1 based on the positions of a value between a min/max 
float invLerp(float from, float to, float value){
  return (value - from) / (to - from);
}


float ExponentialFog(float posHeight, float distance) {
	float fogAmount = exp(-posHeight) * (1.0 - exp(-distance)) * TESR_VolumetricFogData.z;
	return fogAmount;
}

float4 VolumetricFog(VSOUT IN) : COLOR0 {

	float SunExponent = min(TESR_VolumetricFogData.x, 1); // 8
	float SunGlareCoeff = TESR_VolumetricFogData.y; // 100
	float FogStrength = TESR_VolumetricFogData.z; // 1
	float MaxFogHeight = TESR_VolumetricFogData.w; // 80000

	float3 color = tex2D(TESR_RenderedBuffer, IN.UVCoord).rgb;
    float height = reconstructWorldPosition(IN.UVCoord).z;
    float depth = readDepth(IN.UVCoord);
	float3 eyeVector = normalize(toWorld(IN.UVCoord));

	// quadratic fog based on linear distance in fog range with fog power
	float distance = invLerp(TESR_FogData.x, TESR_FogData.y, depth);
	float fogAmount = saturate(pow(distance, TESR_FogData.w) * FogStrength);
	fogAmount = fogAmount * saturate(exp( - height/MaxFogHeight)); // fade with height

	// calculate color
	float4 fogColor  = lerp(TESR_FogColor, TESR_HorizonColor, distance); // fade color between fog to horizon based on depth
	float sunAmount = pow(saturate(dot(eyeVector, TESR_SunDirection)), SunExponent) * TESR_FogData.z/SunGlareCoeff; //sun influence
	fogColor  = TESR_HorizonColor + lerp(0, TESR_SunColor, sunAmount); // add sun color to the fog

	color = lerp( color.rgb , fogColor.rgb, fogAmount);

	return float4(color, 1.0f);
}

technique
{
	pass
	{
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader  = compile ps_3_0 VolumetricFog();
	}
}