#pragma once
#include <stack>

class ShadowManager { // Never disposed
public:
	static void Initialize();
    
	enum ShadowMapTypeEnum {
		MapNear = 0,
		MapMiddle = 1,
		MapFar = 2,
		MapLod = 3,
		MapOrtho = 4,
	};

	enum PlaneEnum {
		PlaneNear	= 0,
		PlaneFar	= 1,
		PlaneLeft	= 2,
		PlaneRight	= 3,
		PlaneTop	= 4,
		PlaneBottom	= 5,
	};

	void					SetFrustum(ShadowMapTypeEnum ShadowMapType, D3DMATRIX* Matrix);
	bool					InFrustum(ShadowMapTypeEnum ShadowMapType, NiNode* Node);
	TESObjectREFR*			GetRef(TESObjectREFR* Ref, SettingsShadowStruct::FormsStruct* Forms, SettingsShadowStruct::ExcludedFormsList* ExcludedForms);
	void					AccumChildren(NiAVObject* NiObject, float MinRadius);
	void					AccumObject(std::stack<NiAVObject*>* containersAccum, NiAVObject* NiObject);
	void					RenderAccums();
	void					RenderGeometry(NiGeometry* Geo);
	void					RenderSkinnedGeometry(NiGeometry* Geo);
	void					RenderSpeedTreeGeometry(NiGeometry* Geo);
	void					DrawGeometryBuffer(NiGeometryBufferData* GeoData, UINT verticesCount);
	void					RenderShadowMap(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXVECTOR3* At, D3DXVECTOR4* SunDir);
	void					RenderExteriorCell(TESObjectCELL* Cell, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, ShadowMapTypeEnum ShadowMapType);
	void					RenderShadowCubeMap(ShadowSceneLight** Lights, UInt32 LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors);
	void					RenderShadowMaps();
	//void					GetNearbyLights(NiPointLight* ShadowLightsList[], NiPointLight* LightsList[]);
	void					CalculateBlend(NiPointLight** Lights, int LightIndex);
    void                    BlurShadowMap(ShadowMapTypeEnum ShadowMapType);    
	D3DXMATRIX				GetCascadeViewProj(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXMATRIX View);
	static void				GetCascadeDepths();
	static float			lerp(float a, float b, float t);

	std::stack<NiGeometry*> geometryAccum;
	std::stack<NiGeometry*> skinnedGeoAccum;
	std::stack<NiGeometry*> speedTreeAccum;

    ShaderRecordVertex*		ShadowMapVertex;
	ShaderRecordPixel*		ShadowMapPixel;
	D3DVIEWPORT9			ShadowMapViewPort[5];
	D3DXPLANE				ShadowMapFrustum[5][6];
	NiVector4				BillboardRight;
	NiVector4				BillboardUp;
	ShaderRecordVertex*		ShadowCubeMapVertex;
	ShaderRecordPixel*		ShadowCubeMapPixel;
	int						PointLightsNum;
    
	float					ShadowCascadesDepth[3];

    ShaderRecordVertex*		ShadowMapBlurVertex;
	ShaderRecordPixel*		ShadowMapBlurPixel;
    IDirect3DVertexBuffer9* BlurShadowVertex[4];
    float                   ShadowMapInverseResolution[5];
    
	D3DVIEWPORT9			ShadowCubeMapViewPort;
	NiPointLight*			ShadowCubeMapLights[ShadowCubeMapsMax];
	ShaderRecordVertex*		CurrentVertex;
	ShaderRecordPixel*		CurrentPixel;
	bool					AlphaEnabled;
};