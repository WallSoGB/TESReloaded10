// Image space shadows shader for Oblivion Reloaded

float4x4 TESR_WorldViewProjectionTransform;
float4x4 TESR_ShadowCameraToLightTransformNear;
float4x4 TESR_ShadowCameraToLightTransformMiddle;
float4x4 TESR_ShadowCameraToLightTransformFar;
float4x4 TESR_ShadowCameraToLightTransformLod;
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
float4 TESR_ReciprocalResolution;
float4 TESR_ViewSpaceLightDir;
float4 TESR_WaterSettings; //x: water height in the cell, y: water depth darkness, z: is camera underwater
float4 TESR_ShadowData; // x: quality, y: darkness, z: nearmap resolution, w: farmap resolution
float4 TESR_ShadowScreenSpaceData; // x: Enabled, y: blurRadius, z: renderDistance
float4 TESR_ShadowFade;
float4 TESR_ShadowRadius; // radius of the 4 cascades
float4 TESR_SkyColor;

sampler2D TESR_RenderedBuffer : register(s0) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_DepthBuffer : register(s1) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = ANISOTROPIC; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferNear : register(s2) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = ANISOTROPIC; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferMiddle : register(s3) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = ANISOTROPIC; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferFar : register(s4) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = ANISOTROPIC; MIPFILTER = LINEAR; };
sampler2D TESR_ShadowMapBufferLod : register(s5) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = ANISOTROPIC; MIPFILTER = LINEAR; };
sampler2D TESR_SourceBuffer : register(s6) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = LINEAR; MINFILTER = LINEAR; MIPFILTER = LINEAR; };
sampler2D TESR_NoiseSampler : register(s7) < string ResourceName = "Effects\bluenoise256.dds"; > = sampler_state { ADDRESSU = WRAP; ADDRESSV = WRAP; MAGFILTER = NONE; MINFILTER = NONE; MIPFILTER = NONE; };
// sampler2D TESR_NormalsBuffer : register(s8) = sampler_state { ADDRESSU = CLAMP; ADDRESSV = CLAMP; MAGFILTER = NONE; MINFILTER = NONE; MIPFILTER = NONE; };;

#define SSS_STEPNUM 10

static const float DARKNESS = 1-TESR_ShadowData.y;
static const float MIN_VARIANCE = 0.000005;
static const float BLEED_CORRECTION = 0.4;
static const float SSS_DIST = 20;
static const float SSS_THICKNESS = 20;
static const float SSS_MAXDEPTH = TESR_ShadowScreenSpaceData.z * TESR_ShadowScreenSpaceData.x;

struct VSOUT
{
	float4 vertPos : POSITION;
	float4 normal : TEXCOORD1;
	float2 UVCoord : TEXCOORD0;
};

struct VSIN
{
	float4 vertPos : POSITION0;
	float2 UVCoord : TEXCOORD0;
};

#include "Includes/Helpers.hlsl"
#include "Includes/Depth.hlsl"
#include "Includes/BlurDepth.hlsl"
#include "Includes/Blending.hlsl"
// #include "Includes/Normals.hlsl"

VSOUT FrameVS(VSIN IN)
{
	VSOUT OUT = (VSOUT)0.0f;
	OUT.vertPos = IN.vertPos;
	OUT.UVCoord = IN.UVCoord;
	return OUT;
}


float ChebyshevUpperBound(float2 moments, float distance)
{
	// get traditional shadow value
	float p = (moments.x > distance); //0: in shadow, 1: in light

	// Compute variance.    
	float Variance = moments.y - moments.x * moments.x;
	Variance = max(Variance, MIN_VARIANCE);

	// Compute the Chebyshev upper bound.
	float d = distance - moments.x;
	float p_max = invlerps(BLEED_CORRECTION, 1.0, Variance / (Variance + d*d));
	return max(p, p_max);
}

float4 ScreenCoordToTexCoord(float4 coord){
	// apply perspective (perspective division) and convert from -1/1 to range to 0/1 (shadowMap range);
	coord.xyz /= coord.w;
	coord.x = coord.x * 0.5f + 0.5f;
	coord.y = coord.y * -0.5f + 0.5f;

	return coord;
}

// Exponential Soft Shadow Maps
float GetLightAmountValueESSM(float depth, float depthCompare){
	return exp(-80 * depthCompare) * depth;
}

// Exponential Shadow Maps
float GetLightAmountValueESM(float depth, float depthCompare){
	return exp(-500 * (depthCompare - depth));
}

float GetLightAmountValueVSM(float2 moments, float depthCompare)
{
	//returns wether the coordinates are in shadow (0), light (1) or penumbra.
	return ChebyshevUpperBound(moments, depthCompare);
}

float GetLightAmountValue(sampler2D shadowBuffer, float4x4 lightTransform, float4 coord){
	// Quality : shadow technique
	// 0: disabled
	// 1: VSM
	// 2: simple ESM
	// 3: filtered ESM

	float4 LightSpaceCoord = ScreenCoordToTexCoord(mul(coord, lightTransform));
	float4 shadowBufferValue = tex2D(shadowBuffer, LightSpaceCoord.xy);

	float4 shadowMode = {TESR_ShadowData.w == 1.0f, TESR_ShadowData.w == 2.0f, TESR_ShadowData.w == 3.0f, TESR_ShadowData.w == 4.0f};
	float4 shadows = {
		GetLightAmountValueVSM(shadowBufferValue.xy, LightSpaceCoord.z),
		GetLightAmountValueESM(shadowBufferValue.x, LightSpaceCoord.z),
		GetLightAmountValueESSM(shadowBufferValue.x, LightSpaceCoord.z),
		0.0,
	};
	return dot(shadows, shadowMode);
}

float GetLightAmount(float4 coord, float depth)
{
	float blendArea = 0.8;
	float shadow;

	// getting all shadow values from cascades as negative (to be able to use the dot product to chose the correct one)
	// shadows are inverted to mean 1 if in shadow to allow dot product filtering
	float4 shadows = {
		1 - GetLightAmountValue(TESR_ShadowMapBufferNear, TESR_ShadowCameraToLightTransformNear, coord),
		1 - GetLightAmountValue(TESR_ShadowMapBufferMiddle, TESR_ShadowCameraToLightTransformMiddle, coord),
		1 - GetLightAmountValue(TESR_ShadowMapBufferFar, TESR_ShadowCameraToLightTransformFar, coord),
		1 - GetLightAmountValue(TESR_ShadowMapBufferLod, TESR_ShadowCameraToLightTransformLod, coord),
	};

	float4 cascade = {
		depth < TESR_ShadowRadius.x,
		depth >= TESR_ShadowRadius.x * blendArea && depth < TESR_ShadowRadius.y,
		depth >= TESR_ShadowRadius.y * blendArea && depth < TESR_ShadowRadius.z,
		depth >= TESR_ShadowRadius.z * blendArea && depth < TESR_ShadowRadius.w,
	};

	// calculate blending areas coefficients between cascades
	float4 fadeIn = {
		1.0,
		clamp(0.0, 1.0, invlerp(blendArea * TESR_ShadowRadius.x, TESR_ShadowRadius.x, depth)),
		clamp(0.0, 1.0, invlerp(blendArea * TESR_ShadowRadius.y, TESR_ShadowRadius.y, depth)),
		clamp(0.0, 1.0, invlerp(blendArea * TESR_ShadowRadius.z, TESR_ShadowRadius.z, depth)),
	};

	float4 fadeOut = {
		clamp(0.0, 1.0, 1 - invlerp(blendArea * TESR_ShadowRadius.x, TESR_ShadowRadius.x, depth)),
		clamp(0.0, 1.0, 1 - invlerp(blendArea * TESR_ShadowRadius.y, TESR_ShadowRadius.y, depth)),
		clamp(0.0, 1.0, 1 - invlerp(blendArea * TESR_ShadowRadius.z, TESR_ShadowRadius.z, depth)),
		clamp(0.0, 1.0, 1 - invlerp(TESR_ShadowRadius.z, TESR_ShadowRadius.w, depth)),
	};

	// apply blending to each cascade shadow
	shadows *= fadeIn * fadeOut;

	// filter the shadow based on the current valid cascades
	return 1 - dot(shadows, cascade);
}



// function to generate growing samples for screen space shadows ray march
// uniform float raymarch_max_dist = SSS_DIST;
// uniform int raymarch_max_iter = SSS_STEPNUM;
// uniform int raymarch_num_bins = SSS_STEPNUM;

// int calc_ray_march_iters(float d) {
//     float bin_delta_d = raymarch_max_dist / raymarch_num_bins;

//     // Chunk d into bins such that the output is centered on x^2
//     float d_rounded = d + bin_delta_d/4;
//     d_rounded = floor(d_rounded);
//     d_rounded = d_rounded * bin_delta_d + bin_delta_d/4;
    
//     // Square the distance and scale to max out at raymarch_max_iter
//     float max_raw_iters = raymarch_max_dist * raymarch_max_dist;
//     float raw_iters = saturate(d_rounded * (d_rounded / max_raw_iters));
//     return int(raw_iters * raymarch_max_iter);
// }

// returns a semi random float3 between 0 and 1 based on the given seed.
// tailored to return a different value for each uv coord of the screen.
float3 random(float2 seed)
{
	return tex2D(TESR_NoiseSampler, (seed/256 + 0.5) / TESR_ReciprocalResolution.xy).xyz;
}

float4 ScreenSpaceShadow(VSOUT IN) : COLOR0
{	
	// calculates wether a point is in shadow based on screen depth
	float2 uv = IN.UVCoord;
	float rand = lerp(0.2, 1, random(uv).r); // some noise to vary the ray length
	float3 pos = reconstructPosition(uv) ; 

	float bias = 0.0;
	if (pos.z > SSS_MAXDEPTH) return float4 (1.0f, 1.0, 1.0, 1.0); // early out for pixels further away than the max render distance

	float occlusion = 0.0;
	[unroll]
	for (float i = 1; i < SSS_STEPNUM/2; i++){
		float4 step = SSS_DIST / SSS_STEPNUM * i * TESR_ViewSpaceLightDir * rand;

		float3 pos1 = pos + step.xyz; // we move to the light
		float3 pos2 = pos1 + step.xyz; // we move to the light
		float3 screen_pos1 = projectPosition(pos1);
		float3 screen_pos2 = projectPosition(pos2);
		
		// if (screen_pos.x > 0 && screen_pos.x < 1.0 && screen_pos.y > 0 && screen_pos.y <1){
		float2 depth = {pos1.z, pos2.z};
		float2 depthCompare = {
			readDepth(screen_pos1.xy),
			readDepth(screen_pos2.xy),
		};

		float2 depthDelta = depth - depthCompare;

		occlusion += depthDelta.x > bias && depthDelta.x < SSS_THICKNESS; // in Shadow
		occlusion += depthDelta.y > bias && depthDelta.y < SSS_THICKNESS; // in Shadow
		pos = pos2;
	}
	occlusion = saturate(occlusion);

	return lerp(1.0f - occlusion, 1.0, pos.z/SSS_MAXDEPTH);
}

float attenuation(float4 world_pos, float4 lightPosition){
	float strength = distance(lightPosition.xyz, world_pos.xyz) / lightPosition.w;
	strength *= strength;

	// radius based attenuation based on https://lisyarus.github.io/blog/graphics/2022/07/30/point-light-attenuation.html
	return saturate(( 1 - strength) / ( 1 + strength));
}

float4 Shadow(VSOUT IN) : COLOR0
{
	// returns a shadow value from darkness setting value (full shadow) to 1 (full light)
	float Shadow = 1.0f;
	float depth = readDepth(IN.UVCoord);
	float3 camera_vector = toWorld(IN.UVCoord) * depth;
	float4 world_pos = float4(TESR_CameraPosition.xyz + camera_vector, 1.0f);
	// float world_normal = GetWorldNormal(IN.UVCoord);

	// early out for underwater surface (if camera is underwater and surface to shade is close to water level with normal pointing downward)
	if (TESR_WaterSettings.z && world_pos.z < TESR_WaterSettings.x + 1.5 && world_pos.z > TESR_WaterSettings.x - 1.5) return float4 (1.0f, 1.0, 1.0, 1.0);
	// if (TESR_WaterSettings.z && world_pos.z < TESR_WaterSettings.x + 2 && world_pos.z > TESR_WaterSettings.x - 2 && dot(world_normal, float3(0, 0, -1)) > 0.999) return float4 (1.0f, 1.0, 1.0, 1.0);

	float4 pos = mul(world_pos, TESR_WorldViewProjectionTransform);

	Shadow = lerp(1, saturate(GetLightAmount(pos, depth)), TESR_ShadowFade.y); //turn off shadow maps if settings disabled
	Shadow = min(tex2D(TESR_RenderedBuffer, IN.UVCoord).r, Shadow);

	// fade shadows to light when sun is low
	Shadow = lerp(Shadow, 1.0f, TESR_ShadowFade.x);

	// fade shadow close to light sources
	Shadow += attenuation(world_pos, TESR_LightPosition0);
	Shadow += attenuation(world_pos, TESR_LightPosition1);
	Shadow += attenuation(world_pos, TESR_LightPosition2);
	Shadow += attenuation(world_pos, TESR_LightPosition3);
	Shadow += attenuation(world_pos, TESR_LightPosition4);
	Shadow += attenuation(world_pos, TESR_LightPosition5);
	Shadow += attenuation(world_pos, TESR_LightPosition6);
	Shadow += attenuation(world_pos, TESR_LightPosition7);
	Shadow += attenuation(world_pos, TESR_LightPosition8);
	Shadow += attenuation(world_pos, TESR_LightPosition9);
	Shadow += attenuation(world_pos, TESR_LightPosition10);
	Shadow += attenuation(world_pos, TESR_LightPosition11);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition0);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition1);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition2);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition3);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition4);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition5);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition6);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition7);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition8);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition9);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition10);
	Shadow += attenuation(world_pos, TESR_ShadowLightPosition11);

	// calculate fog impact
	// float fog = fogCoeff(depth);
	// float fogColor = Desaturate(TESR_FogColor).x;
	// float darkness = clamp(lerp(DARKNESS, fogColor, fog), DARKNESS, 1.0);
	float darkness = DARKNESS;

	// brighten shadow value from 0 to darkness from config value
	Shadow = lerp(darkness, 1.0f, Shadow);
	
	// No point for the shadow buffer to be beyond the 0-1 range
	Shadow = saturate(Shadow);

	// Shadow * (world_pos.z < TESR_WaterSettings.x); // cancel shadow value if under water
	return float4(Shadow, Shadow, Shadow, 1.0f);
}


float4 CombineShadow (VSOUT IN) : COLOR0 
{
	// old style multiply blending (for testing)
	float4 color = tex2D(TESR_SourceBuffer, IN.UVCoord);
	float Shadow = tex2D(TESR_RenderedBuffer, IN.UVCoord).r;

	float4 colorShadow = luma(color.rgb) * Shadow * TESR_SkyColor;

	return lerp(colorShadow, color * Shadow, saturate(Shadow + 0.2)); // bias the transition between the 2 colors to make it less noticeable
}

technique {

	pass {
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 ScreenSpaceShadow();
	}

	pass {
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 Shadow();
	}

	pass
	{ 
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 BlurRChannel(float2(1.0f, 0.0f), TESR_ShadowScreenSpaceData.y, 5, SSS_MAXDEPTH);
	}
	
	pass
	{ 
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 BlurRChannel(float2(0.0f, 1.0f), TESR_ShadowScreenSpaceData.y, 5, SSS_MAXDEPTH);
	}

	pass {
		VertexShader = compile vs_3_0 FrameVS();
	 	PixelShader = compile ps_3_0 CombineShadow();
	}

}
