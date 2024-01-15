#define RESZ_CODE 0x7FA05000
#define BSVertexShaders ((BSShader**)0x011F9548)
#define BSPixelShaders ((BSShader**)0x011F9548)
#define ShadowLightVertexShaders ((NiD3DVertexShader**)0x011FDE5C)
#define ShadowLightPixelShaders ((NiD3DPixelShader**)0x011FDB08)

/**
* Initializes the Shader Manager Singleton.
* The Shader Manager creates and holds onto the Effects activated in the Settings Manager, and sets the constants.
*/
void ShaderManager::Initialize() {

	auto timer = TimeLogger();

	Logger::Log("Starting the shaders manager...");
	TheShaderManager = new ShaderManager();
	TheShaderManager->FrameVertex = NULL;

	memset(TheShaderManager->WaterVertexShaders, NULL, sizeof(WaterVertexShaders));
	memset(TheShaderManager->WaterPixelShaders, NULL, sizeof(WaterPixelShaders));

	TheShaderManager->CreateFrameVertex(TheRenderManager->width, TheRenderManager->height, &TheShaderManager->FrameVertex);

	TheShaderManager->PreviousCell = nullptr;
	TheShaderManager->IsMenuSwitch = false;

	// initializing the list of effect names
	TheShaderManager->RegisterEffect<AvgLumaEffect>(&TheShaderManager->Effects.AvgLuma);
	TheShaderManager->RegisterEffect<AmbientOcclusionEffect>(&TheShaderManager->Effects.AmbientOcclusion);
	TheShaderManager->RegisterEffect<BloodLensEffect>(&TheShaderManager->Effects.BloodLens);
	TheShaderManager->RegisterEffect<BloomLegacyEffect>(&TheShaderManager->Effects.BloomLegacy);
	TheShaderManager->RegisterEffect<ColoringEffect>(&TheShaderManager->Effects.Coloring);
	TheShaderManager->RegisterEffect<CinemaEffect>(&TheShaderManager->Effects.Cinema);
	TheShaderManager->RegisterEffect<DepthOfFieldEffect>(&TheShaderManager->Effects.DepthOfField);
	TheShaderManager->RegisterEffect<DebugEffect>(&TheShaderManager->Effects.Debug);
	TheShaderManager->RegisterEffect<ExposureEffect>(&TheShaderManager->Effects.Exposure);
	TheShaderManager->RegisterEffect<GodRaysEffect>(&TheShaderManager->Effects.GodRays);
	TheShaderManager->RegisterEffect<ImageAdjustEffect>(&TheShaderManager->Effects.ImageAdjust);
	TheShaderManager->RegisterEffect<LensEffect>(&TheShaderManager->Effects.Lens);
	TheShaderManager->RegisterEffect<LowHFEffect>(&TheShaderManager->Effects.LowHF);
	TheShaderManager->RegisterEffect<MotionBlurEffect>(&TheShaderManager->Effects.MotionBlur);
	TheShaderManager->RegisterEffect<NormalsEffect>(&TheShaderManager->Effects.Normals);
	TheShaderManager->RegisterEffect<PretonemapperEffect>(&TheShaderManager->Effects.Pretonemapper);
	TheShaderManager->RegisterEffect<RainEffect>(&TheShaderManager->Effects.Rain);
	TheShaderManager->RegisterEffect<SharpeningEffect>(&TheShaderManager->Effects.Sharpening);
	TheShaderManager->RegisterEffect<ShadowsExteriorEffect>(&TheShaderManager->Effects.ShadowsExteriors);
	TheShaderManager->RegisterEffect<ShadowsInteriorsEffect>(&TheShaderManager->Effects.ShadowsInteriors);
	TheShaderManager->RegisterEffect<PointShadowsEffect>(&TheShaderManager->Effects.PointShadows);
	TheShaderManager->RegisterEffect<PointShadows2Effect>(&TheShaderManager->Effects.PointShadows2);
	TheShaderManager->RegisterEffect<SunShadowsEffect>(&TheShaderManager->Effects.SunShadows);
	TheShaderManager->RegisterEffect<SpecularEffect>(&TheShaderManager->Effects.Specular);
	TheShaderManager->RegisterEffect<SnowEffect>(&TheShaderManager->Effects.Snow);
	TheShaderManager->RegisterEffect<SnowAccumulationEffect>(&TheShaderManager->Effects.SnowAccumulation);
	TheShaderManager->RegisterEffect<UnderwaterEffect>(&TheShaderManager->Effects.Underwater);
	TheShaderManager->RegisterEffect<VolumetricFogEffect>(&TheShaderManager->Effects.VolumetricFog);
	TheShaderManager->RegisterEffect<WaterLensEffect>(&TheShaderManager->Effects.WaterLens);
	TheShaderManager->RegisterEffect<WetWorldEffect>(&TheShaderManager->Effects.WetWorld);

	TheShaderManager->RegisterShaderCollection<TonemappingShaders>(&TheShaderManager->Shaders.Tonemapping);
	TheShaderManager->RegisterShaderCollection<POMShaders>(&TheShaderManager->Shaders.POM);
	TheShaderManager->RegisterShaderCollection<WaterShaders>(&TheShaderManager->Shaders.Water);
	TheShaderManager->RegisterShaderCollection<SkyShaders>(&TheShaderManager->Shaders.Sky);
	TheShaderManager->RegisterShaderCollection<SkinShaders>(&TheShaderManager->Shaders.Skin);
	TheShaderManager->RegisterShaderCollection<GrassShaders>(&TheShaderManager->Shaders.Grass);
	TheShaderManager->RegisterShaderCollection<TerrainShaders>(&TheShaderManager->Shaders.Terrain);
	TheShaderManager->RegisterShaderCollection<ExtraShaders>(&TheShaderManager->Shaders.ExtraShaders);
	
	//setup map of constant names
	TheShaderManager->RegisterConstant("TESR_WorldTransform", (D3DXVECTOR4*)&TheRenderManager->worldMatrix);
	TheShaderManager->RegisterConstant("TESR_ViewTransform", (D3DXVECTOR4*)&TheRenderManager->viewMatrix);
	TheShaderManager->RegisterConstant("TESR_ProjectionTransform", (D3DXVECTOR4*)&TheRenderManager->projMatrix);
	TheShaderManager->RegisterConstant("TESR_InvProjectionTransform",  (D3DXVECTOR4*)&TheRenderManager->InvProjMatrix);
	TheShaderManager->RegisterConstant("TESR_WorldViewProjectionTransform",  (D3DXVECTOR4*)&TheRenderManager->WorldViewProjMatrix);
	TheShaderManager->RegisterConstant("TESR_InvViewProjectionTransform", (D3DXVECTOR4*)&TheRenderManager->InvViewProjMatrix);
	TheShaderManager->RegisterConstant("TESR_ViewProjectionTransform", (D3DXVECTOR4*)&TheRenderManager->ViewProjMatrix);
	TheShaderManager->RegisterConstant("TESR_ShadowRadius", &TheShaderManager->ShaderConst.ShadowMap.ShadowMapRadius);
	TheShaderManager->RegisterConstant("TESR_ShadowCubeMapBlend", &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend);
	TheShaderManager->RegisterConstant("TESR_ShadowWorldTransform", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld);
	TheShaderManager->RegisterConstant("TESR_ShadowViewProjTransform", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransform", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransformNear", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[0]);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransformMiddle", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[1]);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransformFar", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[2]);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransformLod", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[3]);
	TheShaderManager->RegisterConstant("TESR_ShadowCameraToLightTransformOrtho", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[4]);
	TheShaderManager->RegisterConstant("TESR_OcclusionWorldViewProjTransform", (D3DXVECTOR4*)&TheShaderManager->ShaderConst.OcclusionMap.OcclusionWorldViewProj);
	TheShaderManager->RegisterConstant("TESR_ShadowCubeMapLightPosition", &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition0", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[0]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition1", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[1]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition2", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[2]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition3", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[3]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition4", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[4]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition5", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[5]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition6", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[6]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition7", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[7]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition8", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[8]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition9", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[9]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition10", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[10]);
	TheShaderManager->RegisterConstant("TESR_ShadowLightPosition11", &TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[11]);
	TheShaderManager->RegisterConstant("TESR_LightPosition0", &TheShaderManager->LightPosition[0]);
	TheShaderManager->RegisterConstant("TESR_LightPosition1", &TheShaderManager->LightPosition[1]);
	TheShaderManager->RegisterConstant("TESR_LightPosition2", &TheShaderManager->LightPosition[2]);
	TheShaderManager->RegisterConstant("TESR_LightPosition3", &TheShaderManager->LightPosition[3]);
	TheShaderManager->RegisterConstant("TESR_LightPosition4", &TheShaderManager->LightPosition[4]);
	TheShaderManager->RegisterConstant("TESR_LightPosition5", &TheShaderManager->LightPosition[5]);
	TheShaderManager->RegisterConstant("TESR_LightPosition6", &TheShaderManager->LightPosition[6]);
	TheShaderManager->RegisterConstant("TESR_LightPosition7", &TheShaderManager->LightPosition[7]);
	TheShaderManager->RegisterConstant("TESR_LightPosition8", &TheShaderManager->LightPosition[8]);
	TheShaderManager->RegisterConstant("TESR_LightPosition9", &TheShaderManager->LightPosition[9]);
	TheShaderManager->RegisterConstant("TESR_LightPosition10", &TheShaderManager->LightPosition[10]);
	TheShaderManager->RegisterConstant("TESR_LightPosition11", &TheShaderManager->LightPosition[11]);
	TheShaderManager->RegisterConstant("TESR_ViewSpaceLightDir", &TheShaderManager->ShaderConst.ViewSpaceLightDir);
	TheShaderManager->RegisterConstant("TESR_ScreenSpaceLightDir", &TheShaderManager->ShaderConst.ScreenSpaceLightDir);
	TheShaderManager->RegisterConstant("TESR_ReciprocalResolution", &TheShaderManager->ShaderConst.ReciprocalResolution);
	TheShaderManager->RegisterConstant("TESR_CameraForward", &TheRenderManager->CameraForward);
	TheShaderManager->RegisterConstant("TESR_DepthConstants", &TheRenderManager->DepthConstants);
	TheShaderManager->RegisterConstant("TESR_CameraData", &TheRenderManager->CameraData);
	TheShaderManager->RegisterConstant("TESR_CameraPosition", &TheRenderManager->CameraPosition);
	TheShaderManager->RegisterConstant("TESR_SunDirection", &TheShaderManager->ShaderConst.SunDir);
	TheShaderManager->RegisterConstant("TESR_SunPosition", &TheShaderManager->ShaderConst.SunPosition);
	TheShaderManager->RegisterConstant("TESR_SunTiming", &TheShaderManager->ShaderConst.SunTiming);
	TheShaderManager->RegisterConstant("TESR_SunAmount", &TheShaderManager->ShaderConst.SunAmount);
	TheShaderManager->RegisterConstant("TESR_GameTime", &TheShaderManager->ShaderConst.GameTime);
	TheShaderManager->RegisterConstant("TESR_FogData", &TheShaderManager->ShaderConst.fogData);
	TheShaderManager->RegisterConstant("TESR_FogDistance", &TheShaderManager->ShaderConst.fogDistance);
	TheShaderManager->RegisterConstant("TESR_FogColor", &TheShaderManager->ShaderConst.fogColor);
	TheShaderManager->RegisterConstant("TESR_SunColor", &TheShaderManager->ShaderConst.sunColor);
	TheShaderManager->RegisterConstant("TESR_SunAmbient", &TheShaderManager->ShaderConst.sunAmbient);
	TheShaderManager->RegisterConstant("TESR_SkyColor", &TheShaderManager->ShaderConst.skyColor);
	TheShaderManager->RegisterConstant("TESR_SkyLowColor", &TheShaderManager->ShaderConst.skyLowColor);
	TheShaderManager->RegisterConstant("TESR_HorizonColor", &TheShaderManager->ShaderConst.horizonColor);

	TheShaderManager->InitializeConstants();

	timer.LogTime("ShaderManager::Initialize");
}

void ShaderManager::CreateFrameVertex(UInt32 Width, UInt32 Height, IDirect3DVertexBuffer9** FrameVertex) {
	
	void* VertexData = NULL;
	float OffsetX = (1.0f / (float)Width) * 0.5f;
	float OffsetY = (1.0f / (float)Height) * 0.5f;
	
	FrameVS FrameVertices[] = {
		{ -1.0f,  1.0f, 1.0f, 0.0f + OffsetX, 0.0f + OffsetY },
		{ -1.0f, -1.0f, 1.0f, 0.0f + OffsetX, 1.0f + OffsetY },
		{  1.0f,  1.0f, 1.0f, 1.0f + OffsetX, 0.0f + OffsetY },
		{  1.0f, -1.0f, 1.0f, 1.0f + OffsetX, 1.0f + OffsetY }
	};
	TheRenderManager->device->CreateVertexBuffer(4 * sizeof(FrameVS), D3DUSAGE_WRITEONLY, FrameFVF, D3DPOOL_DEFAULT, FrameVertex, NULL);
	(*FrameVertex)->Lock(0, 0, &VertexData, NULL);
	memcpy(VertexData, FrameVertices, sizeof(FrameVertices));
	(*FrameVertex)->Unlock();

}


/*
* Initializes and register an effect and its constants
*/
template <typename T> void ShaderManager::RegisterEffect(T** Pointer)
{
	T* effect = new T();
	*Pointer = effect;

	//Logger::Log("Registering effect %i %s", (int)EffectsNames.size(), effect->Name);
	//if (!effect->Name) return;

	EffectsNames[effect->Name] = (EffectRecord**)Pointer;
	effect->RegisterConstants();
	effect->RegisterTextures();
	effect->LoadEffect();
}


template <typename T> void ShaderManager::RegisterShaderCollection(T** Pointer)
{
	T* collection = new T();
	*Pointer = collection;

	//if (!collection->Name) return;
	
	ShaderNames[collection->Name] = (ShaderCollection**)Pointer;
	collection->RegisterConstants();
}


void ShaderManager::RegisterConstant(const char* Name, D3DXVECTOR4* FloatValue)
{
	ConstantsTable[Name] = FloatValue;
}


void ShaderManager::InitializeConstants() {

	ShaderConst.pWeather = NULL;

	ShaderConst.ReciprocalResolution.x = 1.0f / (float)TheRenderManager->width;
	ShaderConst.ReciprocalResolution.y = 1.0f / (float)TheRenderManager->height;
	ShaderConst.ReciprocalResolution.z = (float)TheRenderManager->width / (float)TheRenderManager->height;
	ShaderConst.ReciprocalResolution.w = 0.0f; // Reserved to store the FoV
}


/*
Updates the values of the constants that can be accessed from shader code, with values representing the state of the game's elements.
*/
void ShaderManager::UpdateConstants() {
	
	auto timer = TimeLogger();

	bool IsThirdPersonView = !TheCameraManager->IsFirstPerson();
	Sky* WorldSky = Tes->sky;
	NiNode* SunRoot = WorldSky->sun->RootNode;
	TESClimate* currentClimate = WorldSky->firstClimate;
	TESWeather* currentWeather = WorldSky->firstWeather;
	TESWeather* previousWeather = WorldSky->secondWeather;
	TESObjectCELL* currentCell = Player->parentCell;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();
	TESRegion* currentRegion = Player->GetRegion();
	float weatherPercent = WorldSky->weatherPercent;
	float lastGameTime = ShaderConst.GameTime.y;
	const char* sectionName = NULL;
	avglumaRequired = false; // toggle for rendering AvgLuma
	orthoRequired = false; // toggle for rendering Ortho map

	if (Effects.Debug->Enabled) avglumaRequired = true;

	// context variables
	GameState.PipBoyIsOn = InterfaceManager->getIsMenuOpen();
	GameState.VATSIsOn = InterfaceManager->IsActive(Menu::kMenuType_VATS);
	GameState.OverlayIsOn = InterfaceManager->IsActive(Menu::kMenuType_Computers) ||
		InterfaceManager->IsActive(Menu::kMenuType_LockPick) ||
		InterfaceManager->IsActive(Menu::kMenuType_Surgery) ||
		InterfaceManager->IsActive(Menu::kMenuType_SlotMachine) ||
		InterfaceManager->IsActive(Menu::kMenuType_Blackjack) ||
		InterfaceManager->IsActive(Menu::kMenuType_Roulette) ||
		InterfaceManager->IsActive(Menu::kMenuType_Caravan);
	GameState.isDialog = InterfaceManager->IsActive(Menu::MenuType::kMenuType_Dialog);
	GameState.isPersuasion = InterfaceManager->IsActive(Menu::MenuType::kMenuType_Persuasion);
	
	if (GameState.OverlayIsOn) return;
	if (!currentCell) return; // if no cell is present, we skip update to avoid trying to access values that don't exist
	
	GameState.isExterior = !currentCell->IsInterior();// || Player->parentCell->flags0 & TESObjectCELL::kFlags0_BehaveLikeExterior; // < use exterior flag, broken for now
	GameState.isCellChanged = currentCell != PreviousCell;
	PreviousCell = currentCell;
	if (GameState.isCellChanged) TheSettingManager->SettingsChanged = true; // force update constants during cell transition

	GameState.isUnderwater = Tes->sky->GetIsUnderWater();
	GameState.isRainy = currentWeather?currentWeather->GetWeatherType() == TESWeather::WeatherType::kType_Rainy : false;
	GameState.isSnow = currentWeather?currentWeather->GetWeatherType() == TESWeather::WeatherType::kType_Snow : false;

	TimeGlobals* GameTimeGlobals = TimeGlobals::Get();
	float GameHour = fmod(GameTimeGlobals->GameHour->data, 24); // make sure the hours values are less than 24

	float SunriseStart = WorldSky->GetSunriseBegin();
	float SunriseEnd = WorldSky->GetSunriseEnd();
	float SunsetStart = WorldSky->GetSunsetBegin();
	float SunsetEnd = WorldSky->GetSunsetEnd();

	// calculating sun amount for shaders (currently not used by any shaders)
	float sunRise = step(SunriseStart, SunriseEnd, GameHour); // 0 at night to 1 after sunrise
	float sunSet = step(SunsetEnd, SunsetStart, GameHour);  // 1 before sunset to 0 at night
	GameState.isDayTime = sunRise * sunSet;

	ShaderConst.SunTiming.x = WorldSky->GetSunriseColorBegin();
	ShaderConst.SunTiming.y = SunriseEnd;
	ShaderConst.SunTiming.z = SunsetStart;
	ShaderConst.SunTiming.w = WorldSky->GetSunsetColorEnd();

	// fake sunset time tracking with more time given before sunrise/sunset
	float sunRiseLight = step(SunriseStart - 1.0f, SunriseEnd, GameHour); // 0 at night to 1 after sunrise
	float sunSetLight = step(SunsetEnd + 1.0f, SunsetStart, GameHour);  // 1 before sunset to 0 at night
	float newDayLight = sunRiseLight * sunSetLight;
	GameState.transitionCurve = smoothStep(0.0f, 0.6f, newDayLight); // a curve for day/night transitions that occurs mostly during second half of sunset

	GameState.isDayTimeChanged = (newDayLight != GameState.dayLight);  // allow effects to fire settings update during sunset/sunrise transitions
	GameState.dayLight = newDayLight;

	ShaderConst.GameTime.x = TimeGlobals::GetGameTime(); //time in milliseconds
	ShaderConst.GameTime.y = GameHour; //time in hours
	ShaderConst.GameTime.z = (float)TheFrameRateManager->Time;
	ShaderConst.GameTime.w = TheFrameRateManager->ElapsedTime; // frameTime in seconds

	ShaderConst.SunPosition = SunRoot->m_localTransform.pos.toD3DXVEC4();
	D3DXVec4Normalize(&ShaderConst.SunPosition, &ShaderConst.SunPosition);
	ShaderConst.SunDir = Tes->directionalLight->direction.toD3DXVEC4() * -1.0f;

	// during the day, track the sun mesh position instead of the lighting direction in exteriors
	if (GameState.isExterior && GameState.isDayTime > 0.5) ShaderConst.SunDir = ShaderConst.SunPosition;

	// expose the light vector in view space for screen space lighting
	D3DXVec4Transform(&ShaderConst.ScreenSpaceLightDir, &ShaderConst.SunDir, &TheRenderManager->ViewProjMatrix);
	D3DXVec4Normalize(&ShaderConst.ScreenSpaceLightDir, &ShaderConst.ScreenSpaceLightDir);

	D3DXVec4Transform(&ShaderConst.ViewSpaceLightDir, &ShaderConst.SunDir, &TheRenderManager->ViewMatrix);
	D3DXVec4Normalize(&ShaderConst.ViewSpaceLightDir, &ShaderConst.ViewSpaceLightDir);

	ShaderConst.sunGlare = currentWeather ? (currentWeather->GetSunGlare() / 255.0f) : 0.5f;

	GameState.isDayTime = smoothStep(0, 1, GameState.isDayTime); // smooth daytime progression
	ShaderConst.SunAmount.x = GameState.isDayTime;
	ShaderConst.SunAmount.y = ShaderConst.sunGlare;

	ShaderConst.sunColor.x = WorldSky->sunDirectional.r;
	ShaderConst.sunColor.y = WorldSky->sunDirectional.g;
	ShaderConst.sunColor.z = WorldSky->sunDirectional.b;
	ShaderConst.sunColor.w = ShaderConst.sunGlare;

	ShaderConst.windSpeed = WorldSky->windSpeed;

	ShaderConst.fogColor.x = WorldSky->fogColor.r;
	ShaderConst.fogColor.y = WorldSky->fogColor.g;
	ShaderConst.fogColor.z = WorldSky->fogColor.b;
	ShaderConst.fogColor.w = 1.0f;

	ShaderConst.horizonColor.x = WorldSky->Horizon.r;
	ShaderConst.horizonColor.y = WorldSky->Horizon.g;
	ShaderConst.horizonColor.z = WorldSky->Horizon.b;
	ShaderConst.horizonColor.w = 1.0f;

	ShaderConst.sunAmbient.x = WorldSky->sunAmbient.r;
	ShaderConst.sunAmbient.y = WorldSky->sunAmbient.g;
	ShaderConst.sunAmbient.z = WorldSky->sunAmbient.b;
	ShaderConst.sunAmbient.w = 1.0f;

	ShaderConst.skyLowColor.x = WorldSky->SkyLower.r;
	ShaderConst.skyLowColor.y = WorldSky->SkyLower.g;
	ShaderConst.skyLowColor.z = WorldSky->SkyLower.b;
	ShaderConst.skyLowColor.w = 1.0f;

	ShaderConst.skyColor.x = WorldSky->skyUpper.r;
	ShaderConst.skyColor.y = WorldSky->skyUpper.g;
	ShaderConst.skyColor.z = WorldSky->skyUpper.b;
	ShaderConst.skyColor.w = 1.0f;

	// for near plane, ensure that far > near
	ShaderConst.fogData.x = WorldSky->fogFarPlane > WorldSky->fogNearPlane ? WorldSky->fogNearPlane : WorldSky->fogFarPlane * 0.7;
	ShaderConst.fogData.y = WorldSky->fogFarPlane;
	ShaderConst.fogData.z = ShaderConst.sunGlare;
	ShaderConst.fogData.w = WorldSky->fogPower;

	ShaderConst.fogDistance.x = ShaderConst.fogData.x;
	ShaderConst.fogDistance.y = ShaderConst.fogData.y;
	ShaderConst.fogDistance.z = 1.0f;
	ShaderConst.fogDistance.w = ShaderConst.sunGlare;
	
	if (TheSettingManager->SettingsChanged) {
		// update settings
		for (const auto [Name, effect] : EffectsNames) {
			(*effect)->UpdateSettings();
		}
		for (const auto [Name, shader] : ShaderNames) {
			(*shader)->UpdateSettings();
		}

		// sky settings are used in several shaders whether the shader is active or not
		ShaderConst.SunAmount.z = TheSettingManager->GetSettingI("Shaders.Sky.Main", "ReplaceSun");
		ShaderConst.SunAmount.w = TheSettingManager->GetSettingF("Shaders.Sky.Main", "GlareStrength");
	}

	// update Constants
	for (const auto [Name, shader] : ShaderNames) {
		if ((*shader)->Enabled) (*shader)->UpdateConstants();
	}
	for (const auto [Name, effect] : EffectsNames) {
		if ((*effect)->Enabled) (*effect)->UpdateConstants();
	}

	// Underwater effect uses constants from the water shader
	if (Effects.Underwater->Enabled && !Shaders.Water->Enabled) Shaders.Water->UpdateConstants();

	TheSettingManager->SettingsChanged = false;
	timer.LogTime("ShaderManager::UpdateConstants");
}


ShaderCollection* ShaderManager::GetShaderCollection(const char* Name) {

	if (!memcmp(Name, "WATER", 5)) return Shaders.Water;
	if (!memcmp(Name, "GRASS", 5)) return Shaders.Grass;
	if (!memcmp(Name, "ISHDR", 5) || !memcmp(Name, "HDR", 3)) return Shaders.Tonemapping; // tonemapping shaders have different names between New vegas and Oblivion
	if (!memcmp(Name, "PAR", 3)) return Shaders.POM;
	if (!memcmp(Name, "SKIN", 4)) return Shaders.Skin;
	if (!memcmp(Name, "SKY", 3)) return Shaders.Sky;
	if (strstr(TerrainShadersNames, Name)) return Shaders.Terrain;
	if (strstr(BloodShaders, Name)) return Shaders.Blood;

	return Shaders.ExtraShaders;
}


/*
* Load generic Vertex Shaders as well as the ones for interiors and exteriors if the exist. 
* Returns false if generic one isn't found (as other ones are optional)
*/
bool ShaderManager::LoadShader(NiD3DVertexShader* Shader) {
	
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)Shader;
	ShaderCollection* Collection = GetShaderCollection(VertexShader->ShaderName);
	bool enabled = Collection->Enabled;

	// if AMD and no DXVK, disable glitchy Tonemapping shader replacements
	if (Collection == Shaders.Tonemapping && (TheRenderManager->RESZ && !TheRenderManager->DXVK)) return false;

	// Load generic, interior and exterior shaders
	VertexShader->ShaderProg  = (ShaderRecordVertex*)ShaderRecord::LoadShader(VertexShader->ShaderName, NULL);
	VertexShader->ShaderProgE = (ShaderRecordVertex*)ShaderRecord::LoadShader(VertexShader->ShaderName, "Exteriors\\");
	VertexShader->ShaderProgI = (ShaderRecordVertex*)ShaderRecord::LoadShader(VertexShader->ShaderName, "Interiors\\");
	VertexShader->Enabled = enabled;

	if (VertexShader->ShaderProg != nullptr || VertexShader->ShaderProgE != nullptr || VertexShader->ShaderProgI != nullptr) {
		Collection->VertexShaderList.push_back(VertexShader);
		Logger::Log("Loaded %s Vertex Shader %s", Collection->Name, VertexShader->ShaderName);
	}

	return enabled;
}


/*
* Load generic Pixel Shaders as well as the ones for interiors and exteriors if the exist. 
* Returns false if generic one isn't found (as other ones are optional)
*/
bool ShaderManager::LoadShader(NiD3DPixelShader* Shader) {

	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)Shader;
	ShaderCollection* Collection = GetShaderCollection(PixelShader->ShaderName);

	bool enabled = Collection->Enabled;
	// if AMD and no DXVK, disable glitchy Tonemapping shader replacements
	if (Collection == Shaders.Tonemapping && (TheRenderManager->RESZ && !TheRenderManager->DXVK)) return false;

	PixelShader->ShaderProg  = (ShaderRecordPixel*)ShaderRecord::LoadShader(PixelShader->ShaderName, NULL);
	PixelShader->ShaderProgE = (ShaderRecordPixel*)ShaderRecord::LoadShader(PixelShader->ShaderName, "Exteriors\\");
	PixelShader->ShaderProgI = (ShaderRecordPixel*)ShaderRecord::LoadShader(PixelShader->ShaderName, "Interiors\\");
	PixelShader->Enabled = enabled;

	if (PixelShader->ShaderProg != nullptr || PixelShader->ShaderProgE != nullptr || PixelShader->ShaderProgI != nullptr) {
		Collection->PixelShaderList.push_back(PixelShader);
		Logger::Log("Loaded %s Pixel Shader %s", Collection->Name, PixelShader->ShaderName);
	}

	return enabled;
}


// used to reenable shaders after disposing. Probably useless now that they are not disposed, just toggled.
/*
* Gets the pointer to the vertex shader list and size by name
*/
int ShaderManager::GetVertexShaders(const char* Name, NiD3DVertexShader*** Shader) {

	//BSShader** Shaders = (BSShader**)0x011F9548;
	//NiD3DVertexShader** ShadowLightVertexShaders = (NiD3DVertexShader**)0x011FDE5C;
	int Size = 0;

	if (!strcmp(Name, "Water")) {
		*Shader = ((WaterShader*)BSVertexShaders[kShaderDefinition_WaterShader])->pVertexShaders;
		Size = sizeof(*Shader) / 4;
	}
	else if (!strcmp(Name, "WaterHeightMap")) {
		*Shader = WaterVertexShaders;
		Size = sizeof(WaterVertexShaders) / 4;
	}
	else if (!strcmp(Name, "Terrain") || !strcmp(Name, "ExtraShaders")) {
		*Shader = ShadowLightVertexShaders;
		Size = 103;
	}
	else if (!strcmp(Name, "POM")) {
		*Shader = ((ParallaxShader*)BSVertexShaders[kShaderDefinition_ParallaxShader])->pVertexShaders;
		Size = sizeof(*Shader) / 4;
	}

	return Size;
}


/*
* Gets the pointer to the pixel shader list and size by name
*/
int ShaderManager::GetPixelShaders(const char* Name, NiD3DPixelShader*** Shader) {

	//BSShader** Shaders = (BSShader**)0x011F9548;
	//NiD3DPixelShader** ShadowLightPixelShaders = (NiD3DPixelShader**)0x011FDB08;
	int Size = 0;

	if (!strcmp(Name, "Water")) {
		*Shader = ((WaterShader*)BSPixelShaders[kShaderDefinition_WaterShader])->pPixelShaders;
		Size = sizeof(*Shader) / 4;
	}
	else if (!strcmp(Name, "WaterHeightMap")) {
		*Shader = WaterPixelShaders;
		Size = sizeof(WaterPixelShaders) / 4;
	}
	else if (!strcmp(Name, "Terrain") || !strcmp(Name, "ExtraShaders")) {
		*Shader = ShadowLightPixelShaders;
		Size = 160;
	}
	else if (!strcmp(Name, "POM")) {
		*Shader = ((ParallaxShader*)BSPixelShaders[kShaderDefinition_ParallaxShader])->pPixelShaders;
		Size = sizeof(*Shader) / 4;
	}
	return Size;

}


/*
* Creates shaders after game has started? Seems broken
*/
bool ShaderManager::CreateShader(const char* Name) {

	NiD3DVertexShader** VertexShaderList = NULL;
	NiD3DPixelShader** PixelShaderList = NULL;
	int WaterVertexShadersSize = sizeof(WaterVertexShaders) / 4;
	int WaterPixelShadersSize = sizeof(WaterPixelShaders) / 4;
	int Size = 0;
	bool success = true;

	if (!strcmp(Name, "Terrain") || !strcmp(Name, "ExtraShaders")) {
		VertexShaderList = ShadowLightVertexShaders;
		Size = 103;
		for (int i = 0; i < Size; i++)
			if (VertexShaderList[i] && strstr(TerrainShadersNames, ((NiD3DVertexShaderEx*)VertexShaderList[i])->ShaderName))
				success = LoadShader(VertexShaderList[i]) && success;

		PixelShaderList = ShadowLightPixelShaders;
		Size = 160;
		for (int i = 0; i < Size; i++)
			if (PixelShaderList[i] && strstr(TerrainShadersNames, ((NiD3DPixelShaderEx*)PixelShaderList[i])->ShaderName))
				success = LoadShader(PixelShaderList[i]) && success;
	}
	else if (!strcmp(Name, "Water")) {
		VertexShaderList = ((WaterShader*)BSVertexShaders[kShaderDefinition_WaterShader])->pVertexShaders;
		Size = sizeof(PixelShaderList) / 4;
		for (int i = 0; i < Size; i++)
			if (VertexShaderList[i]) success = LoadShader(VertexShaderList[i]) && success;

		PixelShaderList = ((WaterShader*)BSPixelShaders[kShaderDefinition_WaterShader])->pPixelShaders;
		Size = sizeof(PixelShaderList) / 4;
		for (int i = 0; i < Size; i++)
			if (PixelShaderList[i]) success = LoadShader(PixelShaderList[i]) && success;

		// Water Height map
		VertexShaderList = WaterVertexShaders;
		Size = sizeof(WaterVertexShaders) / 4;
		for (int i = 0; i < Size; i++)
			if (VertexShaderList[i]) success = LoadShader(VertexShaderList[i]) && success;

		PixelShaderList = WaterPixelShaders;
		Size = sizeof(WaterPixelShaders) / 4;
		for (int i = 0; i < Size; i++) if (PixelShaderList[i])
			success = LoadShader(PixelShaderList[i]) && success;
	}

	if (!success) {
		char Message[256] = "Error: Could not load shader ";
		strcat(Message, Name);
		InterfaceManager->ShowMessage(Message);
		Logger::Log(Message);
	}

	return success;
}


/*
* Removes loaded shaders. TODO: move this to ShaderCollection
*/
void ShaderManager::DisposeShader(const char* Name) {

	NiD3DVertexShader** Vertex = NULL;
	NiD3DPixelShader** Pixel = NULL;
	int WaterVertexShadersSize = sizeof(WaterVertexShaders) / 4;
	int WaterPixelShadersSize = sizeof(WaterPixelShaders) / 4;
	int Upperbound = 0;

	if (!strcmp(Name, "Terrain")) {
		Upperbound = GetVertexShaders(Name, &Vertex);
		for (int i = 0; i < Upperbound; i++)
			if (Vertex[i] && strstr(TerrainShadersNames, ((NiD3DVertexShaderEx*)Vertex[i])->ShaderName))
				((NiD3DVertexShaderEx*)Vertex[i])->DisposeShader();

		Upperbound = GetPixelShaders(Name, &Pixel);
		for (int i = 0; i < Upperbound; i++)
			if (Pixel[i] && strstr(TerrainShadersNames, ((NiD3DPixelShaderEx*)Pixel[i])->ShaderName))
				((NiD3DPixelShaderEx*)Pixel[i])->DisposeShader();
	}
	else if (!strcmp(Name, "ExtraShaders")) {
		Upperbound = GetVertexShaders(Name, &Vertex);
		for (int i = 0; i < Upperbound; i++)
			if (Vertex[i] && !strstr(TerrainShadersNames, ((NiD3DVertexShaderEx*)Vertex[i])->ShaderName))
				((NiD3DVertexShaderEx*)Vertex[i])->DisposeShader();

		Upperbound = GetPixelShaders(Name, &Pixel);
		for (int i = 0; i < Upperbound; i++)
			if (Pixel[i] && !strstr(TerrainShadersNames, ((NiD3DPixelShaderEx*)Pixel[i])->ShaderName))
				((NiD3DPixelShaderEx*)Pixel[i])->DisposeShader();
	}
	else {
		Upperbound = GetVertexShaders(Name, &Vertex);
		for (int i = 0; i < Upperbound; i++)
			if (Vertex[i]) ((NiD3DVertexShaderEx*)Vertex[i])->DisposeShader();

		Upperbound = GetPixelShaders(Name, &Pixel);
		for (int i = 0; i < Upperbound; i++)
			if (Pixel[i]) ((NiD3DPixelShaderEx*)Pixel[i])->DisposeShader();

		if (!strcmp(Name, "Water")) {
			Upperbound = GetVertexShaders("WaterHeightMap", &Vertex);
			for (int i = 0; i < Upperbound; i++)
				if (Vertex[i]) ((NiD3DVertexShaderEx*)Vertex[i])->DisposeShader();
			
			Upperbound = GetPixelShaders("WaterHeightMap", &Pixel);
			for (int i = 0; i < Upperbound; i++)
				if (Pixel[i]) ((NiD3DPixelShaderEx*)Pixel[i])->DisposeShader();
			
			Upperbound = GetVertexShaders("WaterDisplacement", &Vertex);
			for (int i = 0; i < Upperbound; i++)
				if (Vertex[i]) ((NiD3DVertexShaderEx*)Vertex[i])->DisposeShader();
			
			Upperbound = GetPixelShaders("WaterDisplacement", &Pixel);
			for (int i = 0; i < Upperbound; i++)
				if (Pixel[i]) ((NiD3DPixelShaderEx*)Pixel[i])->DisposeShader();
		}
	}

}


void ShaderManager::GetNearbyLights(ShadowSceneLight* ShadowLightsList[], NiPointLight* LightsList[]) {
	D3DXVECTOR4 PlayerPosition = Player->pos.toD3DXVEC4();
	//Logger::Log(" ==== Getting lights ====");
	auto timer = TimeLogger();

	// create a map of all nearby valid lights and sort them per distance to player
	std::map<int, ShadowSceneLight*> SceneLights;
	NiTList<ShadowSceneLight>::Entry* Entry = SceneNode->lights.start;

	while (Entry) {
		NiPointLight* Light = Entry->data->sourceLight;
		D3DXVECTOR4 LightPosition = Light->m_worldTransform.pos.toD3DXVEC4();

		bool lightCulled = Light->m_flags & NiAVObject::NiFlags::APP_CULLED;
		bool lightOn = (Light->Diff.r + Light->Diff.g + Light->Diff.b) * Light->Dimmer > 5.0 / 255.0; // Check for low values in case of human error
		if (lightCulled || !lightOn) {
			Entry = Entry->next;
			continue;
		}

		D3DXVECTOR4 LightVector = LightPosition - PlayerPosition;
		D3DXVec4Normalize(&LightVector, &LightVector);
		float inFront = D3DXVec4Dot(&LightVector, &TheRenderManager->CameraForward);
		float Distance = Light->GetDistance(&Player->pos);
		float radius = Light->Spec.r * TheSettingManager->SettingsShadows.Interiors.LightRadiusMult;

		// select lights that will be tracked by removing culled lights and lights behind the player further away than their radius
		// TODO: handle using frustum check
		float drawDistance = 8000;//TheShaderManager->GameState.isExterior ? TheSettingManager->SettingsShadows.Exteriors.ShadowMapRadius[TheShadowManager->ShadowMapTypeEnum::MapLod] : TheSettingManager->SettingsShadows.Interiors.DrawDistance;
		if ((inFront > 0 || Distance < radius) && (Distance + radius) < drawDistance) {
			SceneLights[(int)(Distance * 10000)] = Entry->data; // mutliplying distance (used as key) before convertion to avoid duplicates in case of similar values
		}

		Entry = Entry->next;
	}

	// save only the n first lights (based on #define TrackedLightsMax)
	memset(&TheShaderManager->LightPosition, 0, TrackedLightsMax * sizeof(D3DXVECTOR4)); // clear previous lights from array

	// get the data for all tracked lights
	int ShadowIndex = 0;
	int LightIndex = 0;
	TheShadowManager->PointLightsNum = 0;

#if defined(OBLIVION)
	bool TorchOnBeltEnabled = TheSettingManager->SettingsMain.EquipmentMode.Enabled && TheSettingManager->SettingsMain.EquipmentMode.TorchKey != 255;
#endif

	D3DXVECTOR4 Empty = D3DXVECTOR4(0, 0, 0, 0);

	std::map<int, ShadowSceneLight*>::iterator v = SceneLights.begin();
	for (int i = 0; i < TrackedLightsMax + ShadowCubeMapsMax; i++) {
		if (v != SceneLights.end()) {
			NiPointLight* Light = v->second->sourceLight;
			if (!Light || Light->EffectType != NiDynamicEffect::EffectTypes::POINT_LIGHT) {
				v++;
				continue;
			}

			// determin if light is a shadow caster
			bool CastShadow = TheSettingManager->SettingsShadows.Interiors.UseCastShadowFlag ? Light->CastShadows : true;

#if defined(OBLIVION)
			// Oblivion exception for carried torch lights 
			if (TorchOnBeltEnabled && Light->CanCarry == 2) {
				HighProcessEx* Process = (HighProcessEx*)Player->process;
				if (Process->OnBeltState == HighProcessEx::State::In) CastShadow = false;
			}
#endif

			D3DXVECTOR4 LightPos = Light->m_worldTransform.pos.toD3DXVEC4();
			LightPos.w = Light->Spec.r * TheSettingManager->SettingsShadows.Interiors.LightRadiusMult;

			if (CastShadow && ShadowIndex < ShadowCubeMapsMax) {
				// add found light to list of lights that cast shadows
				ShadowLightsList[ShadowIndex] = v->second;
				TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[ShadowIndex] = LightPos;
				ShadowIndex++;
				TheShadowManager->PointLightsNum++; // Constant to track number of shadow casting lights are present
			}
			else if (LightIndex < TrackedLightsMax) {
				LightsList[LightIndex] = Light;
				LightPosition[LightIndex] = LightPos;
				LightIndex++;
			};

			v++;
		}
		else {
			// set null values if number of lights in the scene becomes lower than previous iteration
			if (LightIndex < TrackedLightsMax) {
				//Logger::Log("clearing light at index %i", LightIndex);
				LightsList[LightIndex] = NULL;
				LightPosition[LightIndex] = Empty;
				LightIndex++;
			}
			if (ShadowIndex < ShadowCubeMapsMax) {
				//Logger::Log("clearing shadow casting light at index %i", ShadowIndex);
				ShadowLightsList[ShadowIndex] = NULL;
				ShaderConst.ShadowMap.ShadowLightPosition[ShadowIndex] = Empty;
				ShadowIndex++;
			}
		}
	}

	timer.LogTime("ShadowManager::GetNearbyLights");

}

//
///*
//* Deletes an Effect based on the Effect Record effect type. 
//*/
//void ShaderManager::DisposeEffect(EffectRecord** Effect) {
//	*Effect = NULL;
//	delete* Effect;
//
//	//	case EffectRecord::EffectRecordType::Extra:
//	//		ExtraEffectsList::iterator v = ExtraEffects.begin();
//	//		while (v != ExtraEffects.end()) {
//	//			delete v->second;
//	//			v++;
//	//		}
//	//		ExtraEffects.clear();
//	//		break;
//	//}
//}

/*
* Renders a given effect to an arbitrary render target
*/
void ShaderManager::RenderEffectToRT(IDirect3DSurface9* RenderTarget, EffectRecord* Effect, bool clearRenderTarget) {
	IDirect3DDevice9* Device = TheRenderManager->device;
	Device->SetRenderTarget(0, RenderTarget);
	Device->StretchRect(RenderTarget, NULL, RenderTarget, NULL, D3DTEXF_NONE);
	Effect->Render(Device, RenderTarget, RenderTarget, 0, clearRenderTarget, NULL);
};


void ShaderManager::RenderEffectsPreTonemapping(IDirect3DSurface9* RenderTarget) {
	TheRenderManager->UpdateSceneCameraData();
	TheRenderManager->SetupSceneCamera();

	if (!TheSettingManager->SettingsMain.Main.RenderEffects) return; // Main toggle
	if (!Player->parentCell) return;
	if (GameState.OverlayIsOn) return; // disable all effects during terminal/lockpicking sequences because they bleed through the overlay

	auto timer = TimeLogger();

	IDirect3DDevice9* Device = TheRenderManager->device;
	IDirect3DSurface9* SourceSurface = TheTextureManager->SourceSurface;
	IDirect3DSurface9* RenderedSurface = TheTextureManager->RenderedSurface;

	// prepare device for effects
	Device->SetStreamSource(0, FrameVertex, 0, sizeof(FrameVS));
	Device->SetFVF(FrameFVF);

	// render post process normals for use by shaders
	RenderEffectToRT(Effects.Normals->Textures.NormalsSurface, Effects.Normals, false);

	// render a shadow pass for point lights
	if ((GameState.isExterior && Effects.ShadowsExteriors->Enabled) || (!GameState.isExterior && Effects.ShadowsInteriors->Enabled)) {
		// separate lights in 2 batches
		RenderEffectToRT(Effects.ShadowsExteriors->Textures.ShadowPassSurface, Effects.PointShadows, true);
		if (TheShadowManager->PointLightsNum > 6) RenderEffectToRT(Effects.ShadowsExteriors->Textures.ShadowPassSurface, Effects.PointShadows2, false);
		if (GameState.isExterior) RenderEffectToRT(Effects.ShadowsExteriors->Textures.ShadowPassSurface, Effects.SunShadows, false);
	}

	Device->SetRenderTarget(0, RenderTarget);

	// copy the source render target to both the rendered and source textures (rendered gets updated after every pass, source once per effect)
	Device->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);

	if (GameState.isExterior) 
		Effects.ShadowsExteriors->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	else 
		Effects.ShadowsInteriors->Render(Device, RenderTarget, RenderedSurface, 0, true, SourceSurface);

	Effects.AmbientOcclusion->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.Underwater->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.Specular->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.WetWorld->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.SnowAccumulation->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.VolumetricFog->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.GodRays->Render(Device, RenderTarget, RenderedSurface, 0, true, SourceSurface);
	
	// For AMD devices without DXVK, replace vanilla tonemapping replacement with an Effect
	if ((TheRenderManager->RESZ && !TheRenderManager->DXVK) && Shaders.Tonemapping->Enabled)
		Effects.Pretonemapper->Render(Device, RenderTarget, RenderedSurface, 0, true, SourceSurface);

	timer.LogTime("ShaderManager::RenderEffectsPreTonemapping");
}


/*
* Renders the effect that have been set to enabled.
*/
void ShaderManager::RenderEffects(IDirect3DSurface9* RenderTarget) {
	if (!TheSettingManager->SettingsMain.Main.RenderEffects) return; // Main toggle
	if (!Player->parentCell) return;
	if (GameState.OverlayIsOn) return; // disable all effects during terminal/lockpicking sequences because they bleed through the overlay

	auto timer = TimeLogger();

	TheRenderManager->UpdateSceneCameraData();
	TheRenderManager->SetupSceneCamera();

	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	IDirect3DSurface9* SourceSurface = TheTextureManager->SourceSurface;
	IDirect3DSurface9* RenderedSurface = TheTextureManager->RenderedSurface;

	Device->SetStreamSource(0, FrameVertex, 0, sizeof(FrameVS));
	Device->SetFVF(FrameFVF);

	// prepare device for effects
	Device->SetRenderTarget(0, RenderTarget);

	// copy the source render target to both the rendered and source textures (rendered gets updated after every pass, source once per effect)
	Device->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);

	Effects.Rain->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.Snow->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.BloomLegacy->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);

	// calculate average luma for use by shaders
	if (avglumaRequired) {
		RenderEffectToRT(Effects.AvgLuma->Textures.AvgLumaSurface, Effects.AvgLuma, NULL);
		Device->SetRenderTarget(0, RenderTarget); 	// restore device used for effects
	}

	Effects.Exposure->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);

	// screenspace coloring/blurring effects get rendered last
	Effects.Coloring->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.DepthOfField->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.MotionBlur->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);

	// lens effects
	Effects.BloodLens->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.WaterLens->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.LowHF->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);
	Effects.Lens->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);
	Effects.Sharpening->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);

	// cinema effect gets rendered very last because of vignetting/letterboxing
	Effects.Cinema->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);

	// final adjustments
	Effects.ImageAdjust->Render(Device, RenderTarget, RenderedSurface, 0, false, SourceSurface);

	// debug shader allows to display some of the buffers
	Effects.Debug->Render(Device, RenderTarget, RenderedSurface, 0, false, NULL);

	//if (EffectsSettings->Extra) {
	//	for (EffectsList::iterator iter = Effects.ExtraEffects.begin(); iter != Effects.ExtraEffects.end(); ++iter) {
	//		if (iter->second->Enabled) {
	//			iter->second->Render(Device, RenderTarget, RenderedSurface, false, true);
	//		}
	//	}
	//}

	timer.LogTime("ShaderManager::RenderEffects");
}

/*
* Writes the settings corresponding to the shader/effect name, to switch it between enabled/disabled.*
* Also creates or deletes the corresponding Effect Record.
*/
void ShaderManager::SwitchShaderStatus(const char* Name) {
	IsMenuSwitch = true;

	// effects
	try {
		EffectRecord* effect = *EffectsNames.at(Name);
		bool setting = effect->SwitchEffect();
		TheSettingManager->SetMenuShaderEnabled(Name, setting);

		IsMenuSwitch = false;
		return;
	}
	catch (const std::exception&) {}

	// shaders
	try {
		ShaderCollection* shader = *ShaderNames.at(Name);
		bool setting = shader->SwitchShader();
		TheSettingManager->SetMenuShaderEnabled(Name, setting);
	
		IsMenuSwitch = false;
	}
	catch (const std::exception&) {}

	//bool enable = !TheSettingManager->GetMenuShaderEnabled(Name);
	//DisposeShader(Name);
	//if (enable) CreateShader(Name);
	//TheSettingManager->SetMenuShaderEnabled(Name, enable);


	//else if (!strcmp(Name, "ExtraEffectsSettings")) { //TODO change to new effect switch
	//	EffectsSettings->Extra = !EffectsSettings->Extra;
	//	DisposeEffect(EffectRecord::EffectRecordType::Extra);
	//	if (EffectsSettings->Extra) CreateEffect(EffectRecord::EffectRecordType::Extra);
	//}
}

void ShaderManager::SetCustomConstant(const char* Name, D3DXVECTOR4 Value) {
	CustomConstants::iterator v = CustomConst.find(std::string(Name));
	if (v != CustomConst.end()) v->second = Value;
}

void ShaderManager::SetExtraEffectEnabled(const char* Name, bool Value) {
	//EffectsList::iterator v = Effects.ExtraEffects.find(std::string(Name));
	//if (v != Effects.ExtraEffects.end()) v->second->Enabled = Value;
}