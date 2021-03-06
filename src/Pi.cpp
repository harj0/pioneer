// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Copyright © 2013-14 Meteoric Games Ltd
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Pi.h"
#include "libs.h"
#include "AmbientSounds.h"
#include "CargoBody.h"
#include "CityOnPlanet.h"
#include "DeathView.h"
#include "FaceGenManager.h"
#include "Factions.h"
#include "FileSystem.h"
#include "Frame.h"
#include "GalacticView.h"
#include "Game.h"
#include "GeoSphere.h"
#include "Intro.h"
#include "Lang.h"
#include "LuaComms.h"
#include "LuaConsole.h"
#include "LuaConstants.h"
#include "LuaDev.h"
#include "LuaEngine.h"
#include "LuaEquipDef.h"
#include "LuaEvent.h"
#include "LuaFileSystem.h"
#include "LuaFormat.h"
#include "LuaGame.h"
#include "LuaLang.h"
#include "LuaManager.h"
#include "LuaMissile.h"
#include "LuaMusic.h"
#include "LuaNameGen.h"
#include "LuaRef.h"
#include "LuaShipDef.h"
#include "LuaSpace.h"
#include "LuaTimer.h"
#include "Missile.h"
#include "ModelCache.h"
#include "ModManager.h"
#include "NavLights.h"
#include "Shields.h"
#include "ObjectViewerView.h"
#include "OS.h"
#include "Planet.h"
#include "Player.h"
#include "Projectile.h"
#include "SDLWrappers.h"
#include "SectorView.h"
#include "Serializer.h"
#include "Sfx.h"
#include "ShipCpanel.h"
#include "ShipType.h"
#include "Sound.h"
#include "SoundMusic.h"
#include "Space.h"
#include "SpaceStation.h"
#include "Star.h"
#include "StringF.h"
#include "SystemInfoView.h"
#include "SystemView.h"
#include "Tombstone.h"
#include "UIView.h"
#include "WorldView.h"
#include "KeyBindings.h"
#include "EnumStrings.h"
#include "galaxy/CustomSystem.h"
#include "galaxy/Galaxy.h"
#include "galaxy/StarSystem.h"
#include "gameui/Lua.h"
#include "graphics/Graphics.h"
#include "graphics/Light.h"
#include "graphics/Renderer.h"
#include "graphics/TextureBuilder.h"
#include "graphics/PostProcessing.h"
#include "graphics/PostProcess.h"
#include "graphics/gl2/HorizontalBlurMaterial.h"
#include "graphics/gl3/Effect.h"
#include "gui/Gui.h"
#include "scenegraph/Model.h"
#include "scenegraph/Lua.h"
#include "ui/Context.h"
#include "ui/Lua.h"
#include "MouseCursor.h"
#include "SMAA.h"
#include <algorithm>
#include <sstream>
#include "Tweaker.h"

float Pi::gameTickAlpha;
sigc::signal<void, SDL_Keysym*> Pi::onKeyPress;
sigc::signal<void, SDL_Keysym*> Pi::onKeyRelease;
sigc::signal<void, int, int, int> Pi::onMouseButtonUp;
sigc::signal<void, int, int, int> Pi::onMouseButtonDown;
sigc::signal<void, bool> Pi::onMouseWheel;
sigc::signal<void> Pi::onPlayerChangeTarget;
sigc::signal<void> Pi::onPlayerChangeFlightControlState;
sigc::signal<void> Pi::onPlayerChangeEquipment;
sigc::signal<void, const SpaceStation*> Pi::onDockingClearanceExpired;
LuaSerializer *Pi::luaSerializer;
LuaTimer *Pi::luaTimer;
LuaNameGen *Pi::luaNameGen;
int Pi::keyModState;
std::map<SDL_Keycode,bool> Pi::keyState; // XXX SDL2 SDLK_LAST
char Pi::mouseButton[6];
int Pi::mouseMotion[2];
bool Pi::doingMouseGrab = false;
bool Pi::warpAfterMouseGrab = false;
int Pi::mouseGrabWarpPos[2];
Player *Pi::player;
View *Pi::currentView;
WorldView *Pi::worldView;
DeathView *Pi::deathView;
UIView *Pi::spaceStationView;
UIView *Pi::infoView;
SectorView *Pi::sectorView;
GalacticView *Pi::galacticView;
UIView *Pi::settingsView;
SystemView *Pi::systemView;
SystemInfoView *Pi::systemInfoView;
ShipCpanel *Pi::cpan;
LuaConsole *Pi::luaConsole;
Game *Pi::game;
Random Pi::rng;
float Pi::frameTime;
double Pi::lazyTimer = 0.0;
#if WITH_DEVKEYS
bool Pi::showDebugInfo = false;
#endif
#if PIONEER_PROFILER
std::string Pi::profilerPath;
bool Pi::doProfileSlow = false;
bool Pi::doProfileOne = false;
#endif
int Pi::statSceneTris;
GameConfig *Pi::config;
struct DetailLevel Pi::detail = { 0, 0 };
bool Pi::joystickEnabled;
bool Pi::mouseYInvert;
std::map<SDL_JoystickID, Pi::JoystickState> Pi::joysticks;
std::vector<SDL_JoystickID> Pi::joystickIDs;
int Pi::activeJoystick = -1;

bool Pi::navTunnelDisplayed;
bool Pi::speedLinesDisplayed = false;
bool Pi::targetIndicatorsDisplayed = true;
bool Pi::postProcessingEnabled = true;
Gui::Fixed *Pi::menu;
bool Pi::DrawGUI = true;
Graphics::Renderer *Pi::renderer;
RefCountedPtr<UI::Context> Pi::ui;
ModelCache *Pi::modelCache;
Intro *Pi::intro;
SDLGraphics *Pi::sdl;
Graphics::PostProcess* Pi::m_gamePP;
SMAA* Pi::m_smaa = nullptr;
Graphics::PostProcess* Pi::m_guiPP;
Graphics::RenderTarget *Pi::renderTarget;
RefCountedPtr<Graphics::Texture> Pi::renderTexture;
std::unique_ptr<Graphics::Drawables::TexturedQuad> Pi::renderQuad;
MouseCursor* Pi::mouseCursor = nullptr;
Graphics::RenderState *Pi::quadRenderState = nullptr;

unsigned Pi::m_chromaticAberrationId = -1;
unsigned Pi::m_scanlinesId = -1;
unsigned Pi::m_filmgrainId = -1;
TS_ToneMapping Pi::ts_tonemapping, Pi::ts_default_tonemapping;
TS_FilmGrain Pi::ts_filmgrain, Pi::ts_default_filmgrain;
TS_Scanlines Pi::ts_scanlines, Pi::ts_default_scanlines;
TS_ChromaticAberration Pi::ts_chromatic, Pi::ts_default_chromatic;
TS_FXAA Pi::ts_fxaa;

#if WITH_OBJECTVIEWER
ObjectViewerView *Pi::objectViewerView;
#endif

Sound::MusicPlayer Pi::musicPlayer;
std::unique_ptr<JobQueue> Pi::jobQueue;

float Pi::GetLazyTimer()
{
	return lazyTimer += (SDL_GetTicks() / 1000.0f) - lazyTimer;
}

// XXX enabling this breaks UI gauge rendering. see #2627
#define USE_RTT 0

//static
void Pi::CreateRenderTarget(const Uint16 width, const Uint16 height) {
	/*	@fluffyfreak here's a rendertarget implementation you can use for oculusing and other things. It's pretty simple:
		 - fill out a RenderTargetDesc struct and call Renderer::CreateRenderTarget
		 - pass target to Renderer::SetRenderTarget to start rendering to texture
		 - set up viewport, clear etc, then draw as usual
		 - SetRenderTarget(0) to resume render to screen
		 - you can access the attached texture with GetColorTexture to use it with a material
		You can reuse the same target with multiple textures.
		In that case, leave the color format to NONE so the initial texture is not created, then use SetColorTexture to attach your own.
	*/
#if USE_RTT
	Graphics::RenderStateDesc rsd;
	rsd.depthTest  = false;
	rsd.depthWrite = false;
	rsd.blendMode = Graphics::BLEND_ALPHA;
	quadRenderState = Pi::renderer->CreateRenderState(rsd);

	Graphics::TextureDescriptor texDesc(
		Graphics::TEXTURE_RGBA_8888,
		vector2f(width, height),
		Graphics::LINEAR_CLAMP, false, false, 0);
	Pi::renderTexture.Reset(Pi::renderer->CreateTexture(texDesc));
	Pi::renderQuad.reset(new Graphics::Drawables::TexturedQuad(
		Pi::renderer, Pi::renderTexture.Get(), 
		vector2f(0.0f,0.0f), vector2f(float(Graphics::GetScreenWidth()), float(Graphics::GetScreenHeight())), 
		quadRenderState));

	// Complete the RT description so we can request a buffer.
	// NB: we don't want it to create use a texture because we share it with the textured quad created above.
	Graphics::RenderTargetDesc rtDesc(
		width,
		height,
		Graphics::TEXTURE_NONE,		// don't create a texture
		Graphics::TEXTURE_DEPTH,
		false);
	Pi::renderTarget = Pi::renderer->CreateRenderTarget(rtDesc);

	Pi::renderTarget->SetColorTexture(Pi::renderTexture.Get());
#endif
}

//static
void Pi::DrawRenderTarget() {
#if USE_RTT
	Pi::renderer->BeginFrame();
	Pi::renderer->SetViewport(0, 0, Graphics::GetScreenWidth(), Graphics::GetScreenHeight());	
	Pi::renderer->SetTransform(matrix4x4f::Identity());

	//Gui::Screen::EnterOrtho();
	{
		Pi::renderer->SetMatrixMode(Graphics::MatrixMode::PROJECTION);
		Pi::renderer->PushMatrix();
		Pi::renderer->SetOrthographicProjection(0, Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), 0, -1, 1);
		Pi::renderer->SetMatrixMode(Graphics::MatrixMode::MODELVIEW);
		Pi::renderer->PushMatrix();
		Pi::renderer->LoadIdentity();
	}
	
	Pi::renderQuad->Draw( Pi::renderer );

	//Gui::Screen::LeaveOrtho();
	{
		Pi::renderer->SetMatrixMode(Graphics::MatrixMode::PROJECTION);
		Pi::renderer->PopMatrix();
		Pi::renderer->SetMatrixMode(Graphics::MatrixMode::MODELVIEW);
		Pi::renderer->PopMatrix();
	}

	Pi::renderer->EndFrame();
#endif
}

//static
void Pi::BeginRenderTarget() {
#if USE_RTT
	Pi::renderer->SetRenderTarget(Pi::renderTarget);
#endif
}

//static
void Pi::EndRenderTarget() {
#if USE_RTT
	Pi::renderer->SetRenderTarget(nullptr);
#endif
}

static void draw_progress(UI::Gauge *gauge, UI::Label *label, float progress)
{
	gauge->SetValue(progress);
	label->SetText(stringf(Lang::SIMULATING_UNIVERSE_EVOLUTION_N_BYEARS, formatarg("age", progress * 13.7f)));

	Pi::renderer->ClearScreen();
	Pi::ui->Update();
	Pi::ui->Draw();

	Pi::renderer->SwapBuffers();
}

static void LuaInit()
{
	LuaObject<PropertiedObject>::RegisterClass();

	LuaObject<Body>::RegisterClass();
	LuaObject<Ship>::RegisterClass();
	LuaObject<SpaceStation>::RegisterClass();
	LuaObject<Planet>::RegisterClass();
	LuaObject<Star>::RegisterClass();
	LuaObject<Player>::RegisterClass();
	LuaObject<Missile>::RegisterClass();
	LuaObject<CargoBody>::RegisterClass();
	LuaObject<ModelBody>::RegisterClass();

	LuaObject<StarSystem>::RegisterClass();
	LuaObject<SystemPath>::RegisterClass();
	LuaObject<SystemBody>::RegisterClass();
	LuaObject<Random>::RegisterClass();
	LuaObject<Faction>::RegisterClass();

	Pi::luaSerializer = new LuaSerializer();
	Pi::luaTimer = new LuaTimer();

	LuaObject<LuaSerializer>::RegisterClass();
	LuaObject<LuaTimer>::RegisterClass();

	LuaConstants::Register(Lua::manager->GetLuaState());
	LuaLang::Register();
	LuaEngine::Register();
	LuaEquipDef::Register();
	LuaFileSystem::Register();
	LuaGame::Register();
	LuaComms::Register();
	LuaFormat::Register();
	LuaSpace::Register();
	LuaShipDef::Register();
	LuaMusic::Register();
	LuaDev::Register();
	LuaConsole::Register();

	// XXX sigh
	UI::Lua::Init();
	GameUI::Lua::Init();
	SceneGraph::Lua::Init();

	// XXX load everything. for now, just modules
	lua_State *l = Lua::manager->GetLuaState();
	pi_lua_dofile(l, "libs/autoload.lua");
	pi_lua_dofile_recursive(l, "ui");
	pi_lua_dofile_recursive(l, "modules");

	Pi::luaNameGen = new LuaNameGen(Lua::manager);
}

static void LuaUninit() {
	delete Pi::luaNameGen;

	delete Pi::luaSerializer;
	delete Pi::luaTimer;

	Lua::Uninit();
}

static void LuaInitGame() {
	LuaEvent::Clear();
}

SceneGraph::Model *Pi::FindModel(const std::string &name, bool allowPlaceholder)
{
	SceneGraph::Model *m = 0;
	try {
		m = Pi::modelCache->FindModel(name);
	} catch (ModelCache::ModelNotFoundException) {
		Output("Could not find model: %s\n", name.c_str());
		if (allowPlaceholder) {
			try {
				m = Pi::modelCache->FindModel("error");
			} catch (ModelCache::ModelNotFoundException) {
				Error("Could not find placeholder model");
			}
		}
	}

	return m;
}

const char Pi::SAVE_DIR_NAME[] = "savefiles";

std::string Pi::GetSaveDir()
{
	return FileSystem::JoinPath(FileSystem::GetUserDir(), Pi::SAVE_DIR_NAME);
}

void Pi::Init(const std::map<std::string,std::string> &options)
{
#ifdef PIONEER_PROFILER
	Profiler::reset();
#endif

	OS::EnableBreakpad();
	OS::NotifyLoadBegin();

	FileSystem::Init();
	FileSystem::userFiles.MakeDirectory(""); // ensure the config directory exists

#ifdef PIONEER_PROFILER
	FileSystem::userFiles.MakeDirectory("profiler");
	profilerPath = FileSystem::JoinPathBelow(FileSystem::userFiles.GetRoot(), "profiler");
#endif

	Pi::config = new GameConfig(options);
	KeyBindings::InitBindings();

	if (config->Int("RedirectStdio"))
		OS::RedirectStdio();

	ModManager::Init();

	Lang::Resource res(Lang::GetResource("core", config->String("Lang")));
	Lang::MakeCore(res);

	Pi::detail.planets = config->Int("DetailPlanets");
	Pi::detail.textures = config->Int("Textures");
	Pi::detail.fracmult = config->Int("FractalMultiple");
	Pi::detail.cities = config->Int("DetailCities");

#ifdef STEAM
	bool steam_init_result = SteamAPI_Init();
	assert(steam_init_result);
#endif

	// Initialize SDL
	Uint32 sdlInitFlags = SDL_INIT_VIDEO | SDL_INIT_JOYSTICK;
#if defined(DEBUG) || defined(_DEBUG)
	sdlInitFlags |= SDL_INIT_NOPARACHUTE;
#endif
	if (SDL_Init(sdlInitFlags) < 0) {
		Error("SDL initialization failed: %s\n", SDL_GetError());
	}

	// Do rest of SDL video initialization and create Renderer
	Graphics::Settings videoSettings = {};
	videoSettings.width = config->Int("ScrWidth");
	videoSettings.height = config->Int("ScrHeight");
	videoSettings.fullscreen = (config->Int("StartFullscreen") != 0);
	videoSettings.requestedSamples = config->Int("AntiAliasingMode");
	videoSettings.vsync = (config->Int("VSync") != 0);
	videoSettings.useTextureCompression = (config->Int("UseTextureCompression") != 0);
	videoSettings.enableDebugMessages = (config->Int("EnableGLDebug") != 0);
	videoSettings.iconFile = OS::GetIconFilename();
	videoSettings.title = "Paragon";

	Pi::renderer = Graphics::Init(videoSettings);
	{
		std::ostringstream buf;
		renderer->PrintDebugInfo(buf);

		FILE *f = FileSystem::userFiles.OpenWriteStream("opengl.txt", FileSystem::FileSourceFS::WRITE_TEXT);
		if (!f)
			Output("Could not open 'opengl.txt'\n");
		const std::string &s = buf.str();
		fwrite(s.c_str(), 1, s.size(), f);
		fclose(f);
	}

	Pi::CreateRenderTarget(videoSettings.width, videoSettings.height);
	Pi::rng.IncRefCount(); // so nothing tries to free it
	Pi::rng.seed(time(0));

	InitJoysticks();
	joystickEnabled = (config->Int("EnableJoystick")) ? true : false;
	mouseYInvert = (config->Int("InvertMouseY")) ? true : false;

	navTunnelDisplayed = (config->Int("DisplayNavTunnel")) ? true : false;
	speedLinesDisplayed = (config->Int("SpeedLines")) ? true : false;
	targetIndicatorsDisplayed = (config->Int("TargetIndicators")) ? true : false;
	postProcessingEnabled = (config->Int("PostProcessing")) ? true : false;

	EnumStrings::Init();

	// get threads up
	Uint32 numThreads = config->Int("WorkerThreads");
	const int numCores = OS::GetNumCores();
	assert(numCores > 0);
	if (numThreads == 0) numThreads = std::max(Uint32(numCores) - 1, 1U);
	jobQueue.reset(new JobQueue(numThreads));
	Output("started %d worker threads\n", numThreads);

	// XXX early, Lua init needs it
	ShipType::Init();

	// XXX UI requires Lua  but Pi::ui must exist before we start loading
	// templates. so now we have crap everywhere :/
	Lua::Init();

	Pi::ui.Reset(new UI::Context(Lua::manager, Pi::renderer, Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), Lang::GetCore().GetLangCode()));

	LuaInit();

	Gui::Init(renderer, Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), 800, 600);

	UI::Box *box = Pi::ui->VBox(5);
	UI::Label *label = Pi::ui->Label("");
	label->SetFont(UI::Widget::FONT_HEADING_NORMAL);
	label->SetColor(Color::PARAGON_BLUE);
	UI::Gauge *gauge = Pi::ui->Gauge();

	// Background layer
	UI::Layer *background_layer = Pi::ui->NewLayer();
	// Parse all images
	FileSystem::FileSourceFS fs(FileSystem::GetDataDir());
	FileSystem::FileEnumerator fe(fs, "images");
	std::vector<std::string> bg_files;
	while(!fe.Finished()) {
		const FileSystem::FileInfo& fi = fe.Current();
		if(static_cast<int>(fi.GetType()) == 1) {
			const std::string& fname = fi.GetName();
			unsigned int r = fname.rfind(".png");
			if(r != std::string::npos && r == fname.size() - 4) {
				bg_files.push_back(fname);
			}
		}
		fe.Next();
	}
	if(bg_files.size() > 0) {
		srand(time(nullptr));
		background_layer->SetInnerWidget(
			Pi::ui->Align(UI::Align::MIDDLE)->SetInnerWidget(
				Pi::ui->Expand()->SetInnerWidget(
					Pi::ui->Image("images/" + bg_files[rand() % bg_files.size()], UI::Image::PRESERVE_ASPECT)
				)
			)
		);
	}

	// Gauge layer
	UI::Layer *gauge_layer = Pi::ui->NewLayer();
	gauge_layer->SetInnerWidget(
		Pi::ui->Margin(10)->SetInnerWidget(
			Pi::ui->Expand()->SetInnerWidget(
			Pi::ui->Align(UI::Align::BOTTOM)->SetInnerWidget(
					box->PackEnd(UI::WidgetSet(
						label,
						gauge
					))
				)
			)
		)
    );

	draw_progress(gauge, label, 0.1f);
	
	// Beginning game loading
	double start_game_loading = SDL_GetTicks();

	Galaxy::Init();
	draw_progress(gauge, label, 0.2f);

	FaceGenManager::Init();
	draw_progress(gauge, label, 0.25f);

	Faction::Init();
	draw_progress(gauge, label, 0.3f);

	CustomSystem::Init();
	draw_progress(gauge, label, 0.4f);

	// Reload home sector, they might have changed, due to custom systems
	// Sectors might be changed in game, so have to re-create them again once we have a Game.
	Faction::SetHomeSectors();
	draw_progress(gauge, label, 0.45f);

	modelCache = new ModelCache(Pi::renderer);
	Shields::Init(Pi::renderer);
	draw_progress(gauge, label, 0.5f);

//unsigned int control_word;
//_clearfp();
//_controlfp_s(&control_word, _EM_INEXACT | _EM_UNDERFLOW | _EM_ZERODIVIDE, _MCW_EM);
//double fpexcept = Pi::timeAccelRates[1] / Pi::timeAccelRates[0];

	draw_progress(gauge, label, 0.6f);

	GeoSphere::Init();
	draw_progress(gauge, label, 0.7f);

	CityOnPlanet::Init();
	draw_progress(gauge, label, 0.8f);

	SpaceStation::Init();
	draw_progress(gauge, label, 0.9f);

	NavLights::Init(Pi::renderer);
	Sfx::Init(Pi::renderer);
	draw_progress(gauge, label, 0.95f);

	if (!config->Int("DisableSound")) {
		Sound::Init();
		Sound::SetMasterVolume(config->Float("MasterVolume"));
		Sound::SetSfxVolume(config->Float("SfxVolume"));
		GetMusicPlayer().SetVolume(config->Float("MusicVolume"));

		Sound::Pause(0);
		if (config->Int("MasterMuted")) Sound::Pause(1);
		if (config->Int("SfxMuted")) Sound::SetSfxVolume(0.f);
		if (config->Int("MusicMuted")) GetMusicPlayer().SetEnabled(false);
	}
	draw_progress(gauge, label, 1.0f);

	// End of game loading
	double end_game_loading = SDL_GetTicks();
	Output("Game loading time: %.3f seconds\n", (end_game_loading - start_game_loading) * 0.001);

	Pi::ui->DropAllLayers();

	OS::NotifyLoadEnd();

#if 0
	// frame test code

	Frame *root = new Frame(0, "root", 0);
	Frame *p1 = new Frame(root, "p1", Frame::FLAG_HAS_ROT);
	Frame *p1r = new Frame(p1, "p1r", Frame::FLAG_ROTATING);
	Frame *m1 = new Frame(p1, "m1", Frame::FLAG_HAS_ROT);
	Frame *m1r = new Frame(m1, "m1r", Frame::FLAG_ROTATING);
	Frame *p2 = new Frame(root, "p2", Frame::FLAG_HAS_ROT);
	Frame *p2r = new Frame(p2, "pr2", Frame::FLAG_ROTATING);

	p1->SetPosition(vector3d(1000,0,0));
	p1->SetVelocity(vector3d(0,1,0));
	p2->SetPosition(vector3d(0,2000,0));
	p2->SetVelocity(vector3d(-2,0,0));
	p1r->SetAngVelocity(vector3d(0,0,0.0001));
	p1r->SetOrient(matrix3x3d::BuildRotate(M_PI/4, vector3d(0,0,1)));
	p2r->SetAngVelocity(vector3d(0,0,-0.0004));
	p2r->SetOrient(matrix3x3d::BuildRotate(-M_PI/2, vector3d(0,0,1)));
	root->UpdateOrbitRails(0, 0);

	CargoBody *c1 = new CargoBody(Equip::Type::SLAVES);
	c1->SetFrame(p1r);
	c1->SetPosition(vector3d(0,180,0));
//	c1->SetVelocity(vector3d(1,0,0));
	CargoBody *c2 = new CargoBody(Equip::Type::SLAVES);
	c2->SetFrame(p1r);
	c2->SetPosition(vector3d(0,200,0));
//	c2->SetVelocity(vector3d(1,0,0));

	vector3d pos = c1->GetPositionRelTo(p1);
	vector3d vel = c1->GetVelocityRelTo(p1);
	double speed = vel.Length();
	vector3d pos2 = c2->GetPositionRelTo(p1);
	vector3d vel2 = c2->GetVelocityRelTo(p1);
	double speed2 = vel2.Length();

	double speed3 = c2->GetVelocityRelTo(c1).Length();
	c2->SwitchToFrame(p1);
	vector3d vel4 = c2->GetVelocityRelTo(c1);
	double speed4 = c2->GetVelocityRelTo(c1).Length();

	root->UpdateOrbitRails(0, 1.0);

	//buildrotate test

	matrix3x3d m = matrix3x3d::BuildRotate(M_PI/2, vector3d(0,0,1));
	vector3d v = m * vector3d(1,0,0);

/*	vector3d pos = p1r->GetPositionRelTo(p2r);
	vector3d vel = p1r->GetVelocityRelTo(p2r);
	matrix3x3d o1 = p1r->GetOrientRelTo(p2r);
	double speed = vel.Length();
	vector3d pos2 = p2r->GetPositionRelTo(p1r);
	vector3d vel2 = p2r->GetVelocityRelTo(p1r);
	matrix3x3d o2 = p2r->GetOrientRelTo(p1r);
	double speed2 = vel2.Length();
*/	root->UpdateOrbitRails(0, 1.0/60);

	delete p2r; delete p2; delete m1r; delete m1; delete p1r; delete p1; delete root;
	delete c1; delete c2;

#endif

#if 0
	// test code to produce list of ship stats

	FILE *pStatFile = fopen("shipstat.csv","wt");
	if (pStatFile)
	{
		fprintf(pStatFile, "name,modelname,hullmass,capacity,fakevol,rescale,xsize,ysize,zsize,facc,racc,uacc,sacc,aacc,exvel\n");
		for (std::map<std::string, ShipType>::iterator i = ShipType::types.begin();
				i != ShipType::types.end(); ++i)
		{
			const ShipType *shipdef = &(i->second);
			SceneGraph::Model *model = Pi::FindModel(shipdef->modelName, false);

			double hullmass = shipdef->hullMass;
			double capacity = shipdef->capacity;

			double xsize = 0.0, ysize = 0.0, zsize = 0.0, fakevol = 0.0, rescale = 0.0, brad = 0.0;
			if (model) {
				std::unique_ptr<SceneGraph::Model> inst(model->MakeInstance());
				model->CreateCollisionMesh();
				Aabb aabb = model->GetCollisionMesh()->GetAabb();
				xsize = aabb.max.x-aabb.min.x;
				ysize = aabb.max.y-aabb.min.y;
				zsize = aabb.max.z-aabb.min.z;
				fakevol = xsize*ysize*zsize;
				brad = aabb.GetRadius();
				rescale = pow(fakevol/(100 * (hullmass+capacity)), 0.3333333333);
			}

			double simass = (hullmass + capacity) * 1000.0;
			double angInertia = (2/5.0)*simass*brad*brad;
			double acc1 = shipdef->linThrust[ShipType::THRUSTER_FORWARD] / (9.81*simass);
			double acc2 = shipdef->linThrust[ShipType::THRUSTER_REVERSE] / (9.81*simass);
			double acc3 = shipdef->linThrust[ShipType::THRUSTER_UP] / (9.81*simass);
			double acc4 = shipdef->linThrust[ShipType::THRUSTER_RIGHT] / (9.81*simass);
			double acca = shipdef->angThrust/angInertia;
			double exvel = shipdef->effectiveExhaustVelocity;

			fprintf(pStatFile, "%s,%s,%.1f,%.1f,%.1f,%.3f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%f,%.1f\n",
				shipdef->name.c_str(), shipdef->modelName.c_str(), hullmass, capacity,
				fakevol, rescale, xsize, ysize, zsize, acc1, acc2, acc3, acc4, acca, exvel);
		}
		fclose(pStatFile);
	}
#endif

	luaConsole = new LuaConsole();
	KeyBindings::toggleLuaConsole.onPress.connect(sigc::mem_fun(Pi::luaConsole, &LuaConsole::Toggle));

	// Tweaker
	Tweaker::Init(renderer->GetWindow()->GetWidth(), renderer->GetWindow()->GetHeight());
#if defined(RELEASE) || !defined(_WIN32)
	Tweaker::Disable();
#endif

	// Create Test tweak
	/*float* test_float = new float; *test_float = 1.0f;
	int* test_int = new int; *test_int = 5;
	bool* test_toggle = new bool; *test_toggle = false;
	Color4f* test_color = new Color4f(0.75f, 0.5f, 0.25f, 1.0f);
	{
		std::vector<STweakValue> vtv = {
			STweakValue(ETWEAK_FLOAT, "TestFloat", test_float, "label='Test Float' min=0.01 max=2.5 help='This is a float'"),
			STweakValue(ETWEAK_INT, "TestInt", test_int, "label='Test Int' min=1 max=42 help='This is an integer'"),
			STweakValue(ETWEAK_SEPERATOR, "TestSeperator", nullptr, ""),
			STweakValue(ETWEAK_TOGGLE, "TestToggle", test_toggle, "label='Test Toggle' help='This is a toggle'"),
			STweakValue(ETWEAK_COLOR, "TestColor", &test_color->r, "label='Test Color' opened=true help='This is a color'"),
		};
		Tweaker::DefineTweak("Test", vtv);
	}*/

	InitPostProcessing();

	// Mouse cursor
	mouseCursor = new MouseCursor(renderer);
}

void Pi::InitPostProcessing()
{
	// Define post processes
	m_gamePP = new Graphics::PostProcess("Bloom", renderer->GetWindow());

	if (Graphics::Hardware::GL3()) {
		// FXAA
		{
			Graphics::GL3::EffectDescriptor fxaa_desc;
			fxaa_desc.strict = true;
			fxaa_desc.uniforms = {
				"texture0",
				"u_texCoordOffset",
				"u_fxaaSpanMax",
				"u_fxaaReduceMul",
				"u_fxaaReduceMin",
			};
			fxaa_desc.vertex_shader = "gl3/postprocessing/textured_fullscreen_quad.vert";
			fxaa_desc.fragment_shader = "gl3/postprocessing/fxaa.frag";
			Graphics::GL3::Effect* fxaa = new Graphics::GL3::Effect(renderer, fxaa_desc);
			fxaa->SetProgram();
			fxaa->GetUniform(fxaa->GetUniformID("u_texCoordOffset")).Set(vector2f(
				1.0f / static_cast<float>(renderer->GetWindow()->GetWidth()),
				1.0f / static_cast<float>(renderer->GetWindow()->GetHeight())));
			fxaa->GetUniform(fxaa->GetUniformID("u_fxaaSpanMax")).Set(8.0f);
			fxaa->GetUniform(fxaa->GetUniformID("u_fxaaReduceMul")).Set(1.0f / 8.0f);
			fxaa->GetUniform(fxaa->GetUniformID("u_fxaaReduceMin")).Set(1.0f / 128.0f);

			m_gamePP->AddPass(renderer, "FXAA", std::shared_ptr<Graphics::GL3::Effect>(fxaa));
		}

		// Chromatic Aberration
		{
			Graphics::GL3::EffectDescriptor chromaber_desc;
			chromaber_desc.strict = true;
			chromaber_desc.uniforms = {
				"texture0",
				"su_ViewportSize",
				"u_centerBuffer",
				"u_aberrationStrength",
			};
			chromaber_desc.vertex_shader = "gl3/postprocessing/textured_fullscreen_quad.vert";
			chromaber_desc.fragment_shader = "gl3/postprocessing/chromatic_aberration.frag";
			Graphics::GL3::Effect* chromaber = new Graphics::GL3::Effect(renderer, chromaber_desc);
			chromaber->SetProgram();
			
			ts_chromatic.effect = chromaber;
			ts_chromatic.centerBufferId = chromaber->GetUniformID("u_centerBuffer");
			ts_chromatic.aberrationStrengthId = chromaber->GetUniformID("u_aberrationStrength");

			{
				std::vector<STweakValue> vtv = {
					STweakValue(ETWEAK_FLOAT, "CenterBuffer", &ts_chromatic.centerBuffer, &ts_default_chromatic.centerBuffer,
						"label='Center Buffer' min=0.0 max=1.0 step=0.01 help='Center Buffer'"),
						STweakValue(ETWEAK_FLOAT, "AberrationStrength", &ts_chromatic.aberrationStrength, 
						&ts_default_chromatic.centerBuffer, 
						"label='AberrationStrength' min=0.0 max=1.0 step=0.01 help='Aberration Strength'"),
				};
				Tweaker::DefineTweak("ChromaticAberration", vtv);
			}

			m_chromaticAberrationId = m_gamePP->AddPass(renderer, "Chromatic Aberration",
				std::shared_ptr<Graphics::GL3::Effect>(chromaber));
			m_gamePP->SetBypassState(m_chromaticAberrationId,
				Pi::config->Int("EnableChromaticAberration") == 0);
		}

		// Scanlines
		{
			Graphics::GL3::EffectDescriptor scanlines_desc;
			scanlines_desc.strict = true;
			scanlines_desc.uniforms = {
				"texture0",
				"su_ViewportSize",
				"u_resolution",
				"u_hardScan",
				//"u_hardPix",
				"u_warp",
				"u_maskDark",
				"u_maskLight",
			};
			scanlines_desc.vertex_shader = "gl3/postprocessing/textured_fullscreen_quad.vert";
			scanlines_desc.fragment_shader = "gl3/postprocessing/scanlines.frag";
			Graphics::GL3::Effect* scanlines = new Graphics::GL3::Effect(renderer, scanlines_desc);
			scanlines->SetProgram();

			ts_scanlines.effect = scanlines;
			ts_scanlines.resolutionId = scanlines->GetUniformID("u_resolution");
			ts_scanlines.hardScanId = scanlines->GetUniformID("u_hardScan");
			//ts_scanlines.hardPixId = scanlines->GetUniformID("u_hardPix");
			ts_scanlines.warpId = scanlines->GetUniformID("u_warp");
			ts_scanlines.maskDarkId = scanlines->GetUniformID("u_maskDark");
			ts_scanlines.maskLightId = scanlines->GetUniformID("u_maskLight");

			{
				std::vector<STweakValue> vtv = {
					STweakValue(ETWEAK_FLOAT, "ResolutionX", &ts_scanlines.resolution.x, &ts_default_scanlines.resolution.x,
						"label='Resolution X' min=1.0 max=10.0 step=0.1 help='Emulated input resolution'"),
					STweakValue(ETWEAK_FLOAT, "ResolutionY", &ts_scanlines.resolution.y, &ts_default_scanlines.resolution.y,
						"label='Resolution Y' min=1.0 max=10.0 step=0.1 help='Emulated input resolution'"),
					STweakValue(ETWEAK_FLOAT, "HardScan", &ts_scanlines.hardScan, &ts_default_scanlines.hardScan,
						"label='Scan Hardness' min=-24.0 max=0.0 step=0.25 help='Hardness of scanline'"),
					//STweakValue(ETWEAK_FLOAT, "HardPix", &ts_scanlines.hardPix, "label='Pixel Hardness' min=-8.0 max=0.0 step=0.1 help='Hardness of pixels in scanline'"),
					STweakValue(ETWEAK_FLOAT, "Warp X", &ts_scanlines.warp.x, &ts_default_scanlines.warp.x,
						"label='Warp X' min=0.0 max=1.0 step=0.01 help='Display warp'"),
					STweakValue(ETWEAK_FLOAT, "Warp Y", &ts_scanlines.warp.y, &ts_default_scanlines.warp.y,
						"label='Warp Y' min=0.0 max=1.0 step=0.01 help='Display warp'"),
					STweakValue(ETWEAK_FLOAT, "MaskDark", &ts_scanlines.maskDark, &ts_default_scanlines.maskDark,
						"label='Mask Dark' min=0.0 max=3.0 step=0.01 help='Amount of shadow mask'"),
					STweakValue(ETWEAK_FLOAT, "MaskLight", &ts_scanlines.maskLight, &ts_default_scanlines.maskLight,
						"label='Mask Light' min=0.0 max=3.0 step=0.01 help='Amount of shadow mask'"),
				};
				Tweaker::DefineTweak("Scanlines", vtv);
			}

			m_scanlinesId = m_gamePP->AddPass(renderer, "Scanlines",
				std::shared_ptr<Graphics::GL3::Effect>(scanlines));
			m_gamePP->SetBypassState(m_scanlinesId,
				Pi::config->Int("EnableScanlines") == 0);
		}

		// FilmGrain
		{
			Graphics::GL3::EffectDescriptor fg_desc;
			fg_desc.strict = true;
			fg_desc.uniforms = {
				"texture0",
				"su_ViewportSize",
				"su_GameTime",
				"u_mode",
			};
			fg_desc.vertex_shader = "gl3/postprocessing/textured_fullscreen_quad.vert";
			fg_desc.fragment_shader = "gl3/postprocessing/filmgrain.frag";
			Graphics::GL3::Effect* filmgrain = new Graphics::GL3::Effect(renderer, fg_desc);
			filmgrain->SetProgram();

			ts_filmgrain.effect = filmgrain;
			ts_filmgrain.modeId = filmgrain->GetUniformID("u_mode");

			{
				std::vector<STweakValue> vtv = {
					STweakValue(ETWEAK_FLOAT, "Mode", &ts_filmgrain.mode, &ts_default_filmgrain.mode,
						"label='Mode' min=0.0 max=1.0 step=1.0 help='Film grain has 2 modes'"),
				};
				Tweaker::DefineTweak("FilmGrain", vtv);
			}

			m_filmgrainId = m_gamePP->AddPass(renderer, "Film Grain",
				std::shared_ptr<Graphics::GL3::Effect>(filmgrain));
			m_gamePP->SetBypassState(m_filmgrainId,
				Pi::config->Int("EnableFilmGrain") == 0);
		}

		// Bloomless Tone Mapping
		Graphics::GL3::EffectDescriptor tonemap_desc;
		tonemap_desc.uniforms = {
			"texture0",
			"u_defog",
			"u_fogColor",
			"u_exposure",
			"u_gamma",
			"u_vignetteCenter",
			"u_vignetteRadius",
			"u_vignetteAmount",
			"u_blueShift",
		};
		tonemap_desc.vertex_shader = "gl3/postprocessing/textured_fullscreen_quad.vert";
		tonemap_desc.fragment_shader = "gl3/postprocessing/tonemapping.frag";
		Graphics::GL3::Effect* tonemap = new Graphics::GL3::Effect(renderer, tonemap_desc);
		tonemap->SetProgram();

		ts_tonemapping.effect = tonemap;
		ts_tonemapping.defogId = tonemap->GetUniformID("u_defog");
		ts_tonemapping.fogColorId = tonemap->GetUniformID("u_fogColor");
		ts_tonemapping.exposureId = tonemap->GetUniformID("u_exposure");
		ts_tonemapping.gammaId = tonemap->GetUniformID("u_gamma");
		ts_tonemapping.vignetteCenterId = tonemap->GetUniformID("u_vignetteCenter");
		ts_tonemapping.vignetteRadiusId = tonemap->GetUniformID("u_vignetteRadius");
		ts_tonemapping.vignetteAmountId = tonemap->GetUniformID("u_vignetteAmount");
		ts_tonemapping.blueShiftId = tonemap->GetUniformID("u_blueShift");

		{
			std::vector<STweakValue> vtv = {
				STweakValue(ETWEAK_FLOAT, "Defog", &ts_tonemapping.defog, &ts_default_tonemapping.defog,
					"label='DeFog' min=0.0 max=1.0 step=0.01 help='The amount of fog to remove'"),
				STweakValue(ETWEAK_COLOR, "FogColor", &ts_tonemapping.fogColor.r, &ts_default_tonemapping.fogColor.r, 
					"label='Fog Color' opened=true help='The fog color'"),
				STweakValue(ETWEAK_FLOAT, "Exposure", &ts_tonemapping.exposure, &ts_default_tonemapping.exposure,
					"label='Exposure' min=-1.0 max=1.0 step=0.01 help='The exposure adjustment'"),
				STweakValue(ETWEAK_FLOAT, "Gamma", &ts_tonemapping.gamma, &ts_default_tonemapping.gamma,
					"label='Gamma' min=0.5 max=2.0 step=0.01 help='The gamma correction exponent'"),
				STweakValue(ETWEAK_FLOAT, "VignettingCenterX", &ts_tonemapping.vignetteCenter.x, 
					&ts_default_tonemapping.vignetteCenter.x,
					"label='Vignetting Center X' min=0.0 max=1.0 step=0.01 help='The center of vignetting'"),
				STweakValue(ETWEAK_FLOAT, "VignettingCenterY", &ts_tonemapping.vignetteCenter.y, 
					&ts_default_tonemapping.vignetteCenter.y,
					"label='Vignetting Center Y' min=0.0 max=1.0 step=0.01 help='The center of vignetting'"),
				STweakValue(ETWEAK_FLOAT, "VignettingRadius", &ts_tonemapping.vignetteRadius,
					&ts_default_tonemapping.vignetteRadius,
					"label='Vignetting Radius' min=0.0 max=1.0 step=0.01 help='The radius of vignetting'"),
				STweakValue(ETWEAK_FLOAT, "VignettingAmount", &ts_tonemapping.vignetteAmount, 
					&ts_default_tonemapping.vignetteAmount,
					"label='Vignetting Amount' min=-1.0 max=1.0 step=0.01 help='The amount of vignetting'"),
				STweakValue(ETWEAK_FLOAT, "BlueShift", &ts_tonemapping.blueShift, 
					&ts_default_tonemapping.blueShift,
					"label='Blue Shift' min=0.0 max=1.0 step=0.01 help='The amount of blue shift'"),
			};
			Tweaker::DefineTweak("ToneMapping", vtv);
		}

		m_gamePP->AddPass(renderer, "ToneMapping", std::shared_ptr<Graphics::GL3::Effect>(tonemap));
	} else {
		m_gamePP->AddPass(renderer, "HBlur", Graphics::EFFECT_HORIZONTAL_BLUR);
		m_gamePP->AddPass(renderer, "VBlur", Graphics::EFFECT_VERTICAL_BLUR);
		m_gamePP->AddPass(renderer, "Compose", Graphics::EFFECT_BLOOM_COMPOSITOR,
			Graphics::PP_PASS_COMPOSE);
	}

	m_guiPP = new Graphics::PostProcess("GUI", renderer->GetWindow());

	// Set post processing option
	Pi::renderer->GetPostProcessing()->SetPerformPostProcessing(postProcessingEnabled);

	//m_smaa = new SMAA(renderer);

	// Preload noise texture for tunnel effect
	Graphics::TextureBuilder noise_tb("textures/noise.png", Graphics::LINEAR_CLAMP,
		false, false, false, true);
	noise_tb.GetOrCreateTexture(renderer, "effect");

	// TEMP: Pending conversion to 3.X
	//m_smaa = new SMAA(renderer);
	// Preload SMAA static textures
	//Graphics::TextureBuilder("textures/smaa/AreaTex.png", Graphics::LINEAR_CLAMP,
	//	false, false, false, true).GetOrCreateTexture(renderer, "effect");
	//Graphics::TextureBuilder("textures/smaa/SearchTex.png", Graphics::NEAREST_CLAMP,
	//	false, false, false, true).GetOrCreateTexture(renderer, "effect");

	UpdatePostProcessingEffects();
}

bool Pi::IsConsoleActive()
{
	return luaConsole && luaConsole->IsActive();
}

void Pi::Quit()
{
	Projectile::FreeModel();
	delete Pi::intro;
	delete Pi::luaConsole;
	NavLights::Uninit();
	Shields::Uninit();
	Sfx::Uninit();
	Sound::Uninit();
	SpaceStation::Uninit();
	CityOnPlanet::Uninit();
	GeoSphere::Uninit();
	Galaxy::Uninit();
	Faction::Uninit();
	FaceGenManager::Destroy();
	CustomSystem::Uninit();
	Graphics::Uninit();
	Pi::ui.Reset(0);
	LuaUninit();
	Gui::Uninit();
	delete Pi::modelCache;
	delete Pi::renderer;
	delete Pi::config;
	StarSystemCache::ShrinkCache(SystemPath(), true);

#ifdef STEAM
	if (SteamUser()) {
		SteamAPI_Shutdown();
	}
#endif

	SDL_Quit();
	FileSystem::Uninit();
	jobQueue.reset();
	exit(0);
}

void Pi::FlushCaches()
{
	StarSystemCache::ShrinkCache(SystemPath(), true);
	Sector::cache.ClearCache();
	// XXX Ideally the cache would now be empty, but we still have Faction::m_homesector :(
	// assert(Sector::cache.IsEmpty());
}

void Pi::BoinkNoise()
{
	Sound::PlaySfx("Click", 0.3f, 0.3f, false);
}

void Pi::SetView(View *v)
{
	if (currentView) currentView->Detach();
	currentView = v;
	if (currentView) currentView->Attach();
	if(Pi::mouseCursor) {
		Pi::mouseCursor->Reset();
	}
}

void Pi::OnChangeDetailLevel()
{
	GeoSphere::OnChangeDetailLevel();
}

void Pi::HandleEvents()
{
	PROFILE_SCOPED()
	SDL_Event event;

	// XXX for most keypresses SDL will generate KEYUP/KEYDOWN and TEXTINPUT
	// events. keybindings run off KEYUP/KEYDOWN. the console is opened/closed
	// via keybinding. the console TextInput widget uses TEXTINPUT events. thus
	// after switching the console, the stray TEXTINPUT event causes the
	// console key (backtick) to appear in the text entry field. we hack around
	// this by setting this flag if the console was switched. if its set, we
	// swallow the TEXTINPUT event this hack must remain until we have a
	// unified input system
	bool skipTextInput = false;

	Pi::mouseMotion[0] = Pi::mouseMotion[1] = 0;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			if (Pi::game)
				Pi::EndGame();
			Pi::Quit();
		}

		if (skipTextInput && event.type == SDL_TEXTINPUT) {
			skipTextInput = false;
			continue;
		}

		if(Tweaker::TranslateEvent(event)) {
			continue;
		}

		if (ui->DispatchSDLEvent(event))
			continue;

		bool consoleActive = Pi::IsConsoleActive();
		if (!consoleActive)
			KeyBindings::DispatchSDLEvent(&event);
		else
			KeyBindings::toggleLuaConsole.CheckSDLEventAndDispatch(&event);
		if (consoleActive != Pi::IsConsoleActive()) {
			skipTextInput = true;
			continue;
		}

		if (Pi::IsConsoleActive())
			continue;

		Gui::HandleSDLEvent(&event);

		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					if (Pi::game) {
						// only accessible once game started
						if (currentView != 0) {
							if (currentView != settingsView) {
								Pi::game->SetTimeAccel(Game::TIMEACCEL_PAUSED);
								SetView(settingsView);
							}
							else {
								Pi::game->RequestTimeAccel(Game::TIMEACCEL_1X);
								SetView(Pi::player->IsDead()
										? static_cast<View*>(deathView)
										: static_cast<View*>(worldView));
							}
						}
					}
					break;
				}
				// special keys. LCTRL+turd
				if ((KeyState(SDLK_LCTRL) || (KeyState(SDLK_RCTRL)))) {
					switch (event.key.keysym.sym) {
						case SDLK_q: // Quit
							if (Pi::game)
								Pi::EndGame();
							Pi::Quit();
							break;
						case SDLK_PRINTSCREEN: // print
						case SDLK_KP_MULTIPLY: // screen
						{
							char buf[256];
							const time_t t = time(0);
							struct tm *_tm = localtime(&t);
							strftime(buf, sizeof(buf), "screenshot-%Y%m%d-%H%M%S.png", _tm);
							Screendump(buf, Graphics::GetScreenWidth(), Graphics::GetScreenHeight());
							break;
						}
#if WITH_DEVKEYS
						case SDLK_i: // Toggle Debug info
							Pi::showDebugInfo = !Pi::showDebugInfo;
							break;

#ifdef PIONEER_PROFILER
						case SDLK_p: // alert it that we want to profile
							if (KeyState(SDLK_LSHIFT) || KeyState(SDLK_RSHIFT))
								Pi::doProfileOne = true;
							else {
								Pi::doProfileSlow = !Pi::doProfileSlow;
								Output("slow frame profiling %s\n", Pi::doProfileSlow ? "enabled" : "disabled");
							}
							break;
#endif

						case SDLK_F12:
						{
							if(Pi::game) {
								vector3d dir = -Pi::player->GetOrient().VectorZ();
								/* add test object */
								if (KeyState(SDLK_RSHIFT)) {
									Missile *missile =
										new Missile(ShipType::MISSILE_GUIDED, Pi::player);
									missile->SetOrient(Pi::player->GetOrient());
									missile->SetFrame(Pi::player->GetFrame());
									missile->SetPosition(Pi::player->GetPosition()+50.0*dir);
									missile->SetVelocity(Pi::player->GetVelocity());
									game->GetSpace()->AddBody(missile);
									missile->AIKamikaze(Pi::player->GetCombatTarget());
								} else if (KeyState(SDLK_LSHIFT)) {
									SpaceStation *s = static_cast<SpaceStation*>(Pi::player->GetNavTarget());
									if (s) {
										Ship *ship = new Ship(ShipType::POLICE);
										int port = s->GetFreeDockingPort(ship);
										if (port != -1) {
											Output("Putting ship into station\n");
											// Make police ship intent on killing the player
											ship->AIKill(Pi::player);
											ship->SetFrame(Pi::player->GetFrame());
											ship->SetDockedWith(s, port);
											game->GetSpace()->AddBody(ship);
										} else {
											delete ship;
											Output("No docking ports free dude\n");
										}
									} else {
											Output("Select a space station...\n");
									}
								} else {
									Ship *ship = new Ship(ShipType::POLICE);
									if( KeyState(SDLK_LCTRL) )
										ship->AIFlyTo(Pi::player);	// a less lethal option
									else
										ship->AIKill(Pi::player);	// a really lethal option!
									ship->m_equipment.Set(Equip::SLOT_LASER, 0, Equip::PULSECANNON_DUAL_1MW);
									ship->m_equipment.Add(Equip::LASER_COOLING_BOOSTER);
									ship->m_equipment.Add(Equip::ATMOSPHERIC_SHIELDING);
									ship->SetFrame(Pi::player->GetFrame());
									ship->SetPosition(Pi::player->GetPosition()+100.0*dir);
									ship->SetVelocity(Pi::player->GetVelocity());
									ship->UpdateStats();
									game->GetSpace()->AddBody(ship);
								}
							}
							break;
						}
#endif /* DEVKEYS */
#if WITH_OBJECTVIEWER
						case SDLK_F10:
							Pi::SetView(Pi::objectViewerView);
							break;
#endif
						case SDLK_F11:
							// XXX only works on X11
							//SDL_WM_ToggleFullScreen(Pi::scrSurface);
#if WITH_DEVKEYS
							renderer->ReloadShaders();
#endif
							break;
						case SDLK_F9: // Quicksave
						{
							if(Pi::game) {
								if (Pi::game->IsHyperspace())
									//Pi::cpan->MsgLog()->Message("", Lang::CANT_SAVE_IN_HYPERSPACE);
									Pi::game->log->Add(Lang::CANT_SAVE_IN_HYPERSPACE);

								else {
									const std::string name = "_quicksave";
									const std::string path = FileSystem::JoinPath(GetSaveDir(), name);
									try {
										Game::SaveGame(name, Pi::game);
										//Pi::cpan->MsgLog()->Message("", Lang::GAME_SAVED_TO + path);
										Pi::game->log->Add(Lang::GAME_SAVED_TO + path);
									} catch (CouldNotOpenFileException) {
										//Pi::cpan->MsgLog()->Message("", stringf(Lang::COULD_NOT_OPEN_FILENAME, formatarg("path", path)));
										Pi::game->log->Add(stringf(Lang::COULD_NOT_OPEN_FILENAME, formatarg("path", path)));
									}
									catch (CouldNotWriteToFileException) {
										//Pi::cpan->MsgLog()->Message("", Lang::GAME_SAVE_CANNOT_WRITE);
										Pi::game->log->Add(Lang::GAME_SAVE_CANNOT_WRITE);
									}
								}
							}
							break;
						}
						default:
							break; // This does nothing but it stops the compiler warnings
					}
				}
				Pi::keyState[event.key.keysym.sym] = true;
				Pi::keyModState = event.key.keysym.mod;
				Pi::onKeyPress.emit(&event.key.keysym);
				break;
			case SDL_KEYUP:
				Pi::keyState[event.key.keysym.sym] = false;
				Pi::keyModState = event.key.keysym.mod;
				Pi::onKeyRelease.emit(&event.key.keysym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button < COUNTOF(Pi::mouseButton)) {
					Pi::mouseButton[event.button.button] = 1;
					Pi::onMouseButtonDown.emit(event.button.button,
							event.button.x, event.button.y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button < COUNTOF(Pi::mouseButton)) {
					Pi::mouseButton[event.button.button] = 0;
					Pi::onMouseButtonUp.emit(event.button.button,
							event.button.x, event.button.y);
				}
				break;
			case SDL_MOUSEWHEEL:
				Pi::onMouseWheel.emit(event.wheel.y > 0); // true = up
				break;
			case SDL_MOUSEMOTION:
				Pi::mouseMotion[0] += event.motion.xrel;
				Pi::mouseMotion[1] += event.motion.yrel;
		//		SDL_GetRelativeMouseState(&Pi::mouseMotion[0], &Pi::mouseMotion[1]);
				break;
			case SDL_JOYAXISMOTION:
				if (!joysticks[event.jaxis.which].joystick)	{
					break;
				} else {
					joysticks[event.jaxis.which].axes[event.jaxis.axis] = 
						static_cast<float>(Pi::ReadJoystickAxisValue(event.jaxis.axis)) / 1000.0f;
				}
				break;				
				/*
				if (!joysticks[event.jaxis.which].joystick)
					break;
				if (event.jaxis.value == -32768)
					joysticks[event.jaxis.which].axes[event.jaxis.axis] = 1.f;
				else
					joysticks[event.jaxis.which].axes[event.jaxis.axis] = -event.jaxis.value / 32767.f;
				break;
				*/
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
				if (!joysticks[event.jaxis.which].joystick)
					break;
				joysticks[event.jbutton.which].buttons[event.jbutton.button] = event.jbutton.state != 0;
				break;
			case SDL_JOYHATMOTION:
				if (!joysticks[event.jaxis.which].joystick)
					break;
				joysticks[event.jhat.which].hats[event.jhat.hat] = event.jhat.value;
				break;
		}
	}
}

void Pi::TombStoneLoop()
{
	std::unique_ptr<Tombstone> tombstone(new Tombstone(Pi::renderer, Graphics::GetScreenWidth(), Graphics::GetScreenHeight()));
	Uint32 last_time = SDL_GetTicks();
	float _time = 0;
	if(Pi::mouseCursor) {
		Pi::mouseCursor->Reset();
	}
	do {
		Pi::HandleEvents();
		Pi::renderer->GetWindow()->SetGrab(false);

		// render the scene
		//Pi::BeginRenderTarget();
		Pi::renderer->BeginFrame();
		Pi::renderer->BeginPostProcessing();
		tombstone->Draw(_time);	
		if(Pi::IsPostProcessingEnabled()) {
			UpdatePostProcessingEffects();
			Pi::renderer->PostProcessFrame(m_gamePP);
		} else {
			Pi::renderer->PostProcessFrame(nullptr);
		}
		Pi::renderer->EndPostProcessing();
		Pi::renderer->EndFrame();
		Gui::Draw();
		//Pi::EndRenderTarget();

		//Pi::DrawRenderTarget();
		Pi::renderer->SwapBuffers();

		Pi::frameTime = 0.001f*(SDL_GetTicks() - last_time);
		_time += Pi::frameTime;
		last_time = SDL_GetTicks();
	} while (!((_time > 2.0) && ((Pi::MouseButtonState(SDL_BUTTON_LEFT)) || Pi::KeyState(SDLK_SPACE)) ));
}

void Pi::InitGame()
{
	// this is a bit brittle. skank may be forgotten and survive between
	// games

	//reset input states
	keyState.clear();
	keyModState = 0;
	std::fill(mouseButton, mouseButton + COUNTOF(mouseButton), 0);
	std::fill(mouseMotion, mouseMotion + COUNTOF(mouseMotion), 0);
	for (std::map<SDL_JoystickID,JoystickState>::iterator stick = joysticks.begin(); stick != joysticks.end(); ++stick) {
		JoystickState &state = stick->second;
		std::fill(state.buttons.begin(), state.buttons.end(), false);
		std::fill(state.hats.begin(), state.hats.end(), 0);
		std::fill(state.axes.begin(), state.axes.end(), 0.f);
	}

	if (!config->Int("DisableSound")) AmbientSounds::Init();

	LuaInitGame();
}

static void OnPlayerDockOrUndock()
{
	Pi::game->RequestTimeAccel(Game::TIMEACCEL_1X);
	Pi::game->SetTimeAccel(Game::TIMEACCEL_1X);
}

static void OnPlayerChangeEquipment(Equip::Type e)
{
	Pi::onPlayerChangeEquipment.emit();
}

UI::Gauge* PreStartGauge = nullptr;
UI::Context* PreStartUI = nullptr;
double PreStartTime = 0.0;

void Pi::PreStartGame()
{
	PreStartUI = new UI::Context(Lua::manager, Pi::renderer, 
		Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), Lang::GetCore().GetLangCode());
	
	UI::Box *box = PreStartUI->VBox(5);
	UI::Label *label = PreStartUI->Label("");
	label->SetFont(UI::Widget::FONT_HEADING_NORMAL);
	label->SetColor(Color::PARAGON_BLUE);
	label->SetText("WARMING UP");
	UI::Gauge *gauge = PreStartUI->Gauge();
	gauge->SetValue(0.0f);

	UI::Layer *_layer = PreStartUI->NewLayer();
	_layer->SetInnerWidget(
		PreStartUI->Margin(10)->SetInnerWidget(
			PreStartUI->Expand()->SetInnerWidget(
				PreStartUI->Align(UI::Align::BOTTOM)->SetInnerWidget(
					box->PackEnd(UI::WidgetSet(label, gauge))
				)
			)
		)
	);

	Pi::renderer->ClearScreen();
	PreStartUI->Update();
	PreStartUI->Draw();
	Pi::renderer->SwapBuffers();

	PreStartGauge = gauge;

	PreStartTime = SDL_GetTicks();
}

void Pi::PreStartUpdateProgress(float total, float current)
{
	PreStartGauge->SetValue(current / total);

	Pi::renderer->ClearScreen();
	PreStartUI->Update();
	PreStartUI->Draw();
	Pi::renderer->SwapBuffers();
}

void Pi::StartGame()
{
	Pi::player->onDock.connect(sigc::ptr_fun(&OnPlayerDockOrUndock));
	Pi::player->onUndock.connect(sigc::ptr_fun(&OnPlayerDockOrUndock));
	Pi::player->m_equipment.onChange.connect(sigc::ptr_fun(&OnPlayerChangeEquipment));
	cpan->ShowAll();
	DrawGUI = true;
	cpan->SetAlertState(Ship::ALERT_NONE);
	OnPlayerChangeEquipment(Equip::NONE);
	SetView(worldView);

	// fire event before the first frame
	LuaEvent::Queue("onGameStart");
	LuaEvent::Emit();
}

void Pi::PostStartGame()
{
	Output("Start game loading time: %.3f seconds\n", (SDL_GetTicks() - PreStartTime) * 0.001);
	delete PreStartUI;
}

void Pi::Start()
{
	Pi::intro = new Intro(Pi::renderer, Graphics::GetScreenWidth(), Graphics::GetScreenHeight());

	ui->DropAllLayers();
	ui->GetTopLayer()->SetInnerWidget(ui->CallTemplate("MainMenu"));

	//XXX global ambient colour hack to make explicit the old default ambient colour dependency
	// for some models
	Pi::renderer->SetAmbientColor(Color(51, 51, 51, 255));

	ui->Layout();

	Uint32 last_time = SDL_GetTicks();
	float _time = 0;

	if(Pi::mouseCursor) {
		Pi::mouseCursor->Reset();
	}
	
	LuaEvent::Queue("onGameInit");
	LuaEvent::Queue("onMainMenu");
	LuaEvent::Emit();

	while (!Pi::game) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				Pi::Quit();
			else
				ui->DispatchSDLEvent(event);

			// XXX hack
			// if we hit our exit conditions then ignore further queued events
			// protects against eg double-click during game generation
			if (Pi::game)
				while (SDL_PollEvent(&event)) {}
		}

		//Pi::BeginRenderTarget();
		Pi::renderer->BeginFrame();
		renderer->ClearDepthBuffer();
		Pi::renderer->BeginPostProcessing();
		intro->Draw(_time);		
		
		// Test
		//m_smaa->PostProcess();
		if (Pi::IsPostProcessingEnabled()) {
			UpdatePostProcessingEffects();
			Pi::renderer->PostProcessFrame(m_gamePP);
		} else {
			Pi::renderer->PostProcessFrame(nullptr);
		}
		Pi::renderer->EndPostProcessing();
		Pi::renderer->EndFrame();

		ui->Update();
		ui->Draw();
		
		if(mouseCursor) {
			mouseCursor->Update();
			mouseCursor->Draw();
		}
		
		//Pi::EndRenderTarget();

		// render the rendertarget texture
		//Pi::DrawRenderTarget();

		Pi::renderer->SwapBuffers();

		Pi::frameTime = 0.001f*(SDL_GetTicks() - last_time);
		_time += Pi::frameTime;
		last_time = SDL_GetTicks();
	}

	ui->DropAllLayers();
	ui->Layout(); // UI does important things on layout, like updating keyboard shortcuts

	delete Pi::intro; Pi::intro = 0;

	Pi::musicPlayer.Stop();

	InitGame();
	// PreStartGame should be called by LuaGame:: start game function because Game constructor takes some time?
	PreStartGame();
	StartGame();
	PostStartGame();
	MainLoop();

	Tweaker::Terminate();
}

void Pi::EndGame()
{
	Pi::SetMouseGrab(false);

	Pi::musicPlayer.Stop();
	Sound::DestroyAllEvents();

	// final event
	LuaEvent::Queue("onGameEnd");
	LuaEvent::Emit();

	luaTimer->RemoveAll();

	Lua::manager->CollectGarbage();

	if (!config->Int("DisableSound")) AmbientSounds::Uninit();
	Sound::DestroyAllEvents();

	assert(game);
	delete game;
	game = 0;
	player = 0;

	FlushCaches();
	//Faction::SetHomeSectors(); // We might need them to start a new game
}

void Pi::MainLoop()
{
	double time_player_died = 0;
#ifdef MAKING_VIDEO
	Uint32 last_screendump = SDL_GetTicks();
	int dumpnum = 0;
#endif /* MAKING_VIDEO */

#if WITH_DEVKEYS
	Uint32 last_stats = SDL_GetTicks();
	int frame_stat = 0;
	int phys_stat = 0;
	char fps_readout[256];
	memset(fps_readout, 0, sizeof(fps_readout));
#endif

	int MAX_PHYSICS_TICKS = Pi::config->Int("MaxPhysicsCyclesPerRender");
	if (MAX_PHYSICS_TICKS <= 0)
		MAX_PHYSICS_TICKS = 4;

	double currentTime = 0.001 * double(SDL_GetTicks());
	double accumulator = Pi::game->GetTimeStep();
	Pi::gameTickAlpha = 0;

	if(Pi::mouseCursor) {
		Pi::mouseCursor->Reset();
	}

	while (Pi::game) {
		PROFILE_SCOPED()

#ifdef PIONEER_PROFILER
		Profiler::reset();
#endif

		const Uint32 newTicks = SDL_GetTicks();
		double newTime = 0.001 * double(newTicks);
		Pi::frameTime = newTime - currentTime;
		if (Pi::frameTime > 0.25) Pi::frameTime = 0.25;
		currentTime = newTime;
		accumulator += Pi::frameTime * Pi::game->GetTimeAccelRate();

		const float step = Pi::game->GetTimeStep();
		if (step > 0.0f) {
			PROFILE_SCOPED_RAW("unpaused")
			int phys_ticks = 0;
			while (accumulator >= step) {
				if (++phys_ticks >= MAX_PHYSICS_TICKS) {
					accumulator = 0.0;
					break;
				}
				game->TimeStep(step);
				GeoSphere::UpdateAllGeoSpheres();

				accumulator -= step;
			}
			// rendering interpolation between frames: don't use when docked
			int pstate = Pi::game->GetPlayer()->GetFlightState();
			if (pstate == Ship::DOCKED || pstate == Ship::DOCKING) Pi::gameTickAlpha = 1.0;
			else Pi::gameTickAlpha = accumulator / step;

#if WITH_DEVKEYS
			phys_stat += phys_ticks;
#endif
		} else {
			// paused
			PROFILE_SCOPED_RAW("paused")
			GeoSphere::UpdateAllGeoSpheres();
		}
		frame_stat++;

		// fuckadoodledoo, did the player die?
		if (Pi::player->IsDead()) {
			if (time_player_died > 0.0) {
				if (Pi::game->GetTime() - time_player_died > 8.0) {
					Pi::SetView(0);
					Pi::TombStoneLoop();
					Pi::EndGame();
					break;
				}
			} else {
				Pi::game->SetTimeAccel(Game::TIMEACCEL_1X);
				Pi::deathView->Init();
				Pi::SetView(Pi::deathView);
				time_player_died = Pi::game->GetTime();
			}
		}

		//Pi::BeginRenderTarget();

		Pi::renderer->BeginFrame();
		Pi::renderer->BeginPostProcessing();
		Pi::renderer->SetTransform(matrix4x4f::Identity());

		/* Calculate position for this rendered frame (interpolated between two physics ticks */
        // XXX should this be here? what is this anyway?
		for (Body* b : game->GetSpace()->GetBodies()) {
			b->UpdateInterpTransform(Pi::GetGameTickAlpha());
		}
		game->GetSpace()->GetRootFrame()->UpdateInterpTransform(Pi::GetGameTickAlpha());

		currentView->Update();
		currentView->Draw3D();
		// XXX HandleEvents at the moment must be after view->Draw3D and before
		// Gui::Draw so that labels drawn to screen can have mouse events correctly
		// detected. Gui::Draw wipes memory of label positions.
		Pi::HandleEvents();
		// hide cursor for ship control.

		//SetMouseGrab(Pi::MouseButtonState(SDL_BUTTON_RIGHT));

		if(currentView != worldView) {
			if (Pi::IsPostProcessingEnabled()) {
				UpdatePostProcessingEffects();
				Pi::renderer->PostProcessFrame(m_guiPP);
			} else {
				Pi::renderer->PostProcessFrame(nullptr);
			}
		} else {
			// TEMP: Pending conversion to 3.X
			//m_smaa->PostProcess();
			if (Pi::IsPostProcessingEnabled()) {
				UpdatePostProcessingEffects();
				Pi::renderer->PostProcessFrame(m_gamePP);
			} else {
				Pi::renderer->PostProcessFrame(nullptr);
			}
		}

		Pi::renderer->EndPostProcessing();
		Pi::renderer->EndFrame();
		
		if( DrawGUI ) {
			Gui::Draw();
			if (game) {
				game->log->DrawHudMessages(renderer);
			}
		} else if (game && game->IsNormalSpace()) {
			if (config->Int("DisableScreenshotInfo")==0) {
				const RefCountedPtr<StarSystem> sys = game->GetSpace()->GetStarSystem();
				const SystemPath sp = sys->GetPath();
				std::ostringstream pathStr;

				// fill in pathStr from sp values and sys->GetName()
				static const std::string comma(", ");
				pathStr << Pi::player->GetFrame()->GetLabel() << comma << sys->GetName() << " (" << sp.sectorX << comma << sp.sectorY << comma << sp.sectorZ << ")";

				// display pathStr
				Gui::Screen::EnterOrtho();
				Gui::Screen::PushFont("ConsoleFont");
				Gui::Screen::RenderString(pathStr.str(), 0, 0);
				Gui::Screen::PopFont();
				Gui::Screen::LeaveOrtho();
			}
		}

		// XXX don't draw the UI during death obviously a hack, and still
		// wrong, because we shouldn't this when the HUD is disabled, but
		// probably sure draw it if they switch to eg infoview while the HUD is
		// disabled so we need much smarter control for all this rubbish
		if (Pi::GetView() != Pi::deathView) {
			Pi::ui->Update();
			Pi::ui->Draw();
		}

		if(Pi::mouseCursor) {
			Pi::mouseCursor->Update();
			Pi::mouseCursor->Draw();
		}

		// Draw tweaker (when enabled)
		Tweaker::Draw();

#if WITH_DEVKEYS
		if (Pi::showDebugInfo) {
			Gui::Screen::EnterOrtho();
			Gui::Screen::PushFont("ConsoleFont");
			Gui::Screen::RenderString(fps_readout, 0, 0);
			Gui::Screen::PopFont();
			Gui::Screen::LeaveOrtho();
		}
#endif

		//Pi::EndRenderTarget();
		//Pi::DrawRenderTarget();
		Pi::renderer->SwapBuffers();

		// game exit will have cleared Pi::game. we can't continue.
		if (!Pi::game)
			return;

		if (Pi::game->UpdateTimeAccel())
			accumulator = 0; // fix for huge pauses 10000x -> 1x

		if (!Pi::player->IsDead()) {
			// XXX should this really be limited to while the player is alive?
			// this is something we need not do every turn...
			if (!config->Int("DisableSound")) AmbientSounds::Update();
			if( !Pi::game->IsHyperspace() ) {
				StarSystemCache::ShrinkCache( Pi::game->GetSpace()->GetStarSystem()->GetPath() );
			}
		}
		cpan->Update();
		musicPlayer.Update();

		jobQueue->FinishJobs();

#if WITH_DEVKEYS
		if (Pi::showDebugInfo && SDL_GetTicks() - last_stats > 1000) {
			size_t lua_mem = Lua::manager->GetMemoryUsage();
			int lua_memB = int(lua_mem & ((1u << 10) - 1));
			int lua_memKB = int(lua_mem >> 10) % 1024;
			int lua_memMB = int(lua_mem >> 20);

			snprintf(
				fps_readout, sizeof(fps_readout),
				"%d fps (%.1f ms/f), %d phys updates, %d triangles, %.3f M tris/sec, %d terrain vtx/sec, %d glyphs/sec\n"
				"Lua mem usage: %d MB + %d KB + %d bytes",
				frame_stat, (1000.0/frame_stat), phys_stat, Pi::statSceneTris, Pi::statSceneTris*frame_stat*1e-6,
				GeoSphere::GetVtxGenCount(), Text::TextureFont::GetGlyphCount(),
				lua_memMB, lua_memKB, lua_memB
			);
			frame_stat = 0;
			phys_stat = 0;
			Text::TextureFont::ClearGlyphCount();
			GeoSphere::ClearVtxGenCount();
			if (SDL_GetTicks() - last_stats > 1200) last_stats = SDL_GetTicks();
			else last_stats += 1000;
		}
		Pi::statSceneTris = 0;

#ifdef PIONEER_PROFILER
		const Uint32 profTicks = SDL_GetTicks();
		if (Pi::doProfileOne || (Pi::doProfileSlow && (profTicks-newTicks) > 100)) { // slow: < ~10fps
			Output("dumping profile data\n");
			Profiler::dumphtml(profilerPath.c_str());
			Pi::doProfileOne = false;
		}
#endif

#endif

#ifdef MAKING_VIDEO
		if (SDL_GetTicks() - last_screendump > 50) {
			last_screendump = SDL_GetTicks();
			std::string fname = stringf(Lang::SCREENSHOT_FILENAME_TEMPLATE, formatarg("index", dumpnum++));
			Screendump(fname.c_str(), Graphics::GetScreenWidth(), Graphics::GetScreenHeight());
		}
#endif /* MAKING_VIDEO */
	}
	Tweaker::Close();
}

float Pi::CalcHyperspaceRangeMax(int hyperclass, int total_mass_in_tonnes)
{
	// 625.0f is balancing parameter
	return 312.0f * hyperclass * hyperclass / (total_mass_in_tonnes);
}

float Pi::CalcHyperspaceRange(int hyperclass, float total_mass_in_tonnes, int fuel)
{
	const float range_max = CalcHyperspaceRangeMax(hyperclass, total_mass_in_tonnes);
	int fuel_required_max = CalcHyperspaceFuelOut(hyperclass, range_max, range_max);

	if(fuel_required_max <= fuel)
		return range_max;
	else {
		// range is proportional to fuel - use this as first guess
		float range = range_max*fuel/fuel_required_max;

		// if the range is too big due to rounding error, lower it until is is OK.
		while(range > 0 && CalcHyperspaceFuelOut(hyperclass, range, range_max) > fuel)
			range -= 0.05;

		// range is never negative
		range = std::max(range, 0.0f);
		return range;
	}
}

float Pi::CalcHyperspaceDuration(int hyperclass, int total_mass_in_tonnes, float dist)
{
	float hyperspace_range_max = CalcHyperspaceRangeMax(hyperclass, total_mass_in_tonnes);

	// 0.36 is balancing parameter
	return ((dist * dist * 0.0000036) / (hyperspace_range_max * hyperclass)) *
			(60.0 * 60.0 * 24.0 * sqrtf(total_mass_in_tonnes));
}

float Pi::CalcHyperspaceFuelOut(int hyperclass, float dist, float hyperspace_range_max)
{
	int outFuelRequired = int(ceil(hyperclass*hyperclass*dist / hyperspace_range_max));
	if (outFuelRequired > hyperclass*hyperclass) outFuelRequired = hyperclass*hyperclass;
	if (outFuelRequired < 1) outFuelRequired = 1;

	return outFuelRequired;
}

/*void Pi::Message(const std::string &message, const std::string &from, enum MsgLevel level)
{
	if (level == MSG_IMPORTANT) {
		Pi::cpan->MsgLog()->ImportantMessage(from, message);
	} else {
		Pi::cpan->MsgLog()->Message(from, message);
	}
}*/

void Pi::InitJoysticks() {
	int joy_count = SDL_NumJoysticks();

	for (int n = 0; n < joy_count; n++) {
		JoystickState state;

		state.joystick = SDL_JoystickOpen(n);
		if (!state.joystick) {
			Output("SDL_JoystickOpen(%i): %s\n", n, SDL_GetError());
			continue;
		}

		state.axes.resize(SDL_JoystickNumAxes(state.joystick));
		state.buttons.resize(SDL_JoystickNumButtons(state.joystick));
		state.hats.resize(SDL_JoystickNumHats(state.joystick));
		state.name = SDL_JoystickName(state.joystick);

		for(int i = 0; i < SDL_JoystickNumAxes(state.joystick); ++i) {
			std::ostringstream ss1, ss2;
			ss1 << "Axis" << i << "Invert";
			bool invert_axis = Pi::config->Int(ss1.str(), 0) ? true : false;
			ss2 << "Axis" << i << "DeadZone";
			int deadzone_axis = Pi::config->Int(ss2.str(), 0);

			state.invertAxes.push_back(invert_axis);
			state.deadZoneAxes.push_back(deadzone_axis);
		}

		SDL_JoystickID joyID = SDL_JoystickInstanceID(state.joystick);
		joystickIDs.push_back(joyID);
		joysticks[joyID] = state;
	}
	// TODO this should come from config
	if(joy_count > 0) {
		activeJoystick = 0;
	}
}

void Pi::SetActiveJoystick(int joystick_idx)
{
	assert(joystick_idx < GetJoystickCount());
	activeJoystick = joystick_idx;
}

int Pi::ReadJoystickAxisValue(int aindex)
{
	assert(aindex < GetJoystickAxisCount(activeJoystick));	
	float raw_max = 32767.0f;
	float raw_value = SDL_JoystickGetAxis(joysticks[joystickIDs[activeJoystick]].joystick, aindex);
	float deadzone = (GetJoystickAxisDeadZone(aindex) / 99.0f) * raw_max;
	float livezone = raw_max - deadzone;

	if(abs(raw_value) <= deadzone) {
		return 0;
	} else {
		int value = static_cast<int>((1000.0f * (abs(raw_value) - deadzone)) / livezone);
		if(raw_value < 0.0f) {
			value *= -1;
		}
		if (GetJoystickAxisInvertState(aindex)) {
			value *= -1;
		}
		return value;
	}
}

void Pi::SetJoystickAxisInvertState(int aindex, bool state) 
{ 
	joysticks[GetActiveJoystick()].invertAxes[aindex] = state; 
	std::ostringstream ss;
	ss << "Axis" << aindex << "Invert";
	Pi::config->SetInt(ss.str(), state);
	Pi::config->Save();
}

int Pi::GetJoystickAxisDeadZone(int aindex)
{
	return joysticks[GetActiveJoystick()].deadZoneAxes[aindex];
}

void Pi::SetJoystickAxisDeadZone(int aindex, int deadzone)
{
	assert(aindex < GetJoystickAxisCount());
	joysticks[GetActiveJoystick()].deadZoneAxes[aindex] = Clamp(deadzone, 0, 99);
	std::ostringstream ss;
	ss << "Axis" << aindex << "DeadZone";
	Pi::config->SetInt(ss.str(), joysticks[GetActiveJoystick()].deadZoneAxes[aindex]);
	Pi::config->Save();
}

int Pi::JoystickButtonState(int joystick, int button) {
	if (!joystickEnabled) return 0;
	if (joystick < 0 || joystick >= int(joysticks.size()))
		return 0;

	if (button < 0 || button >= int(joysticks[joystick].buttons.size()))
		return 0;

	return joysticks[joystick].buttons[button];
}

int Pi::JoystickHatState(int joystick, int hat) {
	if (!joystickEnabled) return 0;
	if (joystick < 0 || joystick >= int(joysticks.size()))
		return 0;

	if (hat < 0 || hat >= int(joysticks[joystick].hats.size()))
		return 0;

	return joysticks[joystick].hats[hat];
}

float Pi::JoystickAxisState(int joystick, int axis) {
	if (!joystickEnabled) return 0;
	if (joystick < 0 || joystick >= int(joysticks.size()))
		return 0;

	if (axis < 0 || axis >= int(joysticks[joystick].axes.size()))
		return 0;

	return joysticks[joystick].axes[axis];
}

void Pi::SetMouseGrab(bool on)
{
	if (!doingMouseGrab && on) {
		if(!Pi::mouseCursor) {
			SDL_ShowCursor(0);
		}
		Pi::renderer->GetWindow()->SetGrab(true);
		doingMouseGrab = true;
	}
	else if(doingMouseGrab && !on) {
		if(!Pi::mouseCursor) {
			SDL_ShowCursor(1);
		}
		Pi::renderer->GetWindow()->SetGrab(false);
		doingMouseGrab = false;
	}
}

float Pi::GetMoveSpeedShiftModifier() {
	// Suggestion: make x1000 speed on pressing both keys?
	// This is only used for UI views: systems, sector, and galaxy
	if (Pi::KeyState(SDLK_LSHIFT)) return 100.f;
	if (Pi::KeyState(SDLK_RSHIFT)) return 10.f;
	return 1;
}

void Pi::SetPostProcessingEnabled(bool state) {
	if (postProcessingEnabled != state && Pi::renderer &&
		Pi::renderer->GetPostProcessing())
	{
		Pi::renderer->GetPostProcessing()->SetPerformPostProcessing(state);
	}
	postProcessingEnabled = state;
}

void Pi::UpdatePostProcessingEffects(bool active_tweak)
{
	if(m_gamePP && active_tweak) {
		ts_tonemapping.effect->SetProgram();
		ts_tonemapping.effect->GetUniform(ts_tonemapping.defogId).Set(ts_tonemapping.defog);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.fogColorId).Set(ts_tonemapping.fogColor);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.exposureId).Set(ts_tonemapping.exposure);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.gammaId).Set(ts_tonemapping.gamma);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.vignetteCenterId).Set(ts_tonemapping.vignetteCenter);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.vignetteRadiusId).Set(ts_tonemapping.vignetteRadius);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.vignetteAmountId).Set(ts_tonemapping.vignetteAmount);
		ts_tonemapping.effect->GetUniform(ts_tonemapping.blueShiftId).Set(ts_tonemapping.blueShift);

		ts_filmgrain.effect->SetProgram();
		ts_filmgrain.effect->GetUniform(ts_filmgrain.modeId).Set(ts_filmgrain.mode);

		ts_scanlines.effect->SetProgram();
		ts_scanlines.effect->GetUniform(ts_scanlines.resolutionId).Set(ts_scanlines.resolution);
		ts_scanlines.effect->GetUniform(ts_scanlines.hardScanId).Set(ts_scanlines.hardScan);
		//ts_scanlines.effect->GetUniform(ts_scanlines.hardPixId).Set(ts_scanlines.hardPix);
		ts_scanlines.effect->GetUniform(ts_scanlines.warpId).Set(ts_scanlines.warp);
		ts_scanlines.effect->GetUniform(ts_scanlines.maskDarkId).Set(ts_scanlines.maskDark);
		ts_scanlines.effect->GetUniform(ts_scanlines.maskLightId).Set(ts_scanlines.maskLight);

		ts_chromatic.effect->SetProgram();
		ts_chromatic.effect->GetUniform(ts_chromatic.centerBufferId).Set(ts_chromatic.centerBuffer);
		ts_chromatic.effect->GetUniform(ts_chromatic.aberrationStrengthId).Set(ts_chromatic.aberrationStrength);
	}
}

void Pi::SetExpChromaticAberrationEnabled(bool state)
{
	if(m_chromaticAberrationId != -1) {
		m_gamePP->SetBypassState(m_chromaticAberrationId, !state);
	}
}

void Pi::SetExpScanlinesEnabled(bool state)
{
	if (m_scanlinesId != -1) {
		m_gamePP->SetBypassState(m_scanlinesId, !state);
	}
}

void Pi::SetExpFilmGrainEnabled(bool state)
{
	if (m_filmgrainId != -1) {
		m_gamePP->SetBypassState(m_filmgrainId, !state);
	}
}
