// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Copyright © 2013-14 Meteoric Games Ltd
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _PI_H
#define _PI_H

#include "utils.h"
#include "gui/Gui.h"
#include "Random.h"
#include "gameconsts.h"
#include "GameConfig.h"
#include "LuaSerializer.h"
#include "LuaTimer.h"
#include "CargoBody.h"
#include "Space.h"
#include "JobQueue.h"
#include <map>
#include <string>
#include <vector>
#include "TweakerSettings.h"

class DeathView;
class GalacticView;
class Intro;
class LuaConsole;
class LuaNameGen;
class ModelCache;
class Player;
class SectorView;
class Ship;
class ShipCpanel;
class SpaceStation;
class StarSystem;
class SystemInfoView;
class SystemView;
class UIView;
class View;
class WorldView;
class SDLGraphics;
class SMAA;
namespace Graphics { class Renderer; class PostProcess; }
namespace SceneGraph { class Model; }
namespace Sound { class MusicPlayer; }
namespace UI { class Context; }
class MouseCursor;

#if WITH_OBJECTVIEWER
class ObjectViewerView;
#endif

struct DetailLevel {
	int planets;
	int textures;
	int fracmult;
	int cities;
};

/*enum MsgLevel {
	MSG_NORMAL,
	MSG_IMPORTANT
};*/

class Frame;
class Game;

class Pi {
public:
	static void Init(const std::map<std::string,std::string> &options);
	static void InitGame();
	static void InitPostProcessing();
	static void StarportStart(Uint32 starport);
	static void PreStartGame();
	static void PreStartUpdateProgress(float total, float current);
	static void StartGame();
	static void PostStartGame();
	static void EndGame();
	static void Start();
	static void MainLoop();
	static void TombStoneLoop();
	static void OnChangeDetailLevel();
	static void Quit() __attribute((noreturn));
	static float GetFrameTime() { return frameTime; }
	static float GetLazyTimer();
	static float GetGameTickAlpha() { return gameTickAlpha; }
	static bool KeyState(SDL_Keycode k) { return keyState[k]; }
	static int KeyModState() { return keyModState; }
	static bool IsConsoleActive();

	static int JoystickButtonState(int joystick, int button);
	static int JoystickHatState(int joystick, int hat);
	static float JoystickAxisState(int joystick, int axis);
	static bool IsJoystickEnabled() { return joystickEnabled; }
	static void SetJoystickEnabled(bool state) { joystickEnabled = state; }
	static int GetActiveJoystick() { return activeJoystick; }
	static void SetActiveJoystick(int joystick_idx);
	static int GetJoystickCount() { return joystickIDs.size(); }
	static std::string GetJoystickName(int id) { return joysticks[joystickIDs[id]].name; }
	static int GetJoystickAxisCount(int jindex) { return joysticks[joystickIDs[jindex]].axes.size(); }
	static int GetJoystickAxisCount() { return GetJoystickAxisCount(GetActiveJoystick()); }
	static bool GetJoystickAxisInvertState(int aindex) { return joysticks[GetActiveJoystick()].invertAxes[aindex]; }
	static void SetJoystickAxisInvertState(int aindex, bool state);
	static int ReadJoystickAxisValue(int aindex);
	static int GetJoystickAxisDeadZone(int aindex);
	static void SetJoystickAxisDeadZone(int aindex, int deadzone);

    static void SetMouseYInvert(bool state) { mouseYInvert = state; }
    static bool IsMouseYInvert() { return mouseYInvert; }
	static bool IsNavTunnelDisplayed() { return navTunnelDisplayed; }
	static void SetNavTunnelDisplayed(bool state) { navTunnelDisplayed = state; }
	static bool AreSpeedLinesDisplayed() { return speedLinesDisplayed; }
	static void SetSpeedLinesDisplayed(bool state) { speedLinesDisplayed = state; }
	static bool AreTargetIndicatorsDisplayed() { return targetIndicatorsDisplayed; }
	static void SetTargetIndicatorsDisplayed(bool state) { targetIndicatorsDisplayed = state; }
	static bool IsPostProcessingEnabled() { return postProcessingEnabled; }
	static void SetPostProcessingEnabled(bool state);
	static void UpdatePostProcessingEffects(bool active_tweak = true);
	static int MouseButtonState(int button) { return mouseButton[button]; }
	/// Get the default speed modifier to apply to movement (scrolling, zooming...), depending on the "shift" keys.
	/// This is a default value only, centralized here to promote uniform user expericience.
	static float GetMoveSpeedShiftModifier();
	static void GetMouseMotion(int motion[2]) {
		memcpy(motion, mouseMotion, sizeof(int)*2);
	}
	static void SetMouseGrab(bool on);
	static void FlushCaches();
	static void BoinkNoise();
	static float CalcHyperspaceRangeMax(int hyperclass, int total_mass_in_tonnes);
	static float CalcHyperspaceRange(int hyperclass, float total_mass_in_tonnes, int fuel);
	static float CalcHyperspaceDuration(int hyperclass, int total_mass_in_tonnes, float dist);
	static float CalcHyperspaceFuelOut(int hyperclass, float dist, float hyperspace_range_max);
	//static void Message(const std::string &message, const std::string &from = "", enum MsgLevel level = MSG_NORMAL);
	static std::string GetSaveDir();
	static SceneGraph::Model *FindModel(const std::string&, bool allowPlaceholder = true);

	static void CreateRenderTarget(const Uint16 width, const Uint16 height);
	static void DrawRenderTarget();
	static void BeginRenderTarget();
	static void EndRenderTarget();

	static const char SAVE_DIR_NAME[];

	static sigc::signal<void, SDL_Keysym*> onKeyPress;
	static sigc::signal<void, SDL_Keysym*> onKeyRelease;
	static sigc::signal<void, int, int, int> onMouseButtonUp;
	static sigc::signal<void, int, int, int> onMouseButtonDown;
	static sigc::signal<void, bool> onMouseWheel;
	static sigc::signal<void> onPlayerChangeTarget; // navigation or combat
	static sigc::signal<void> onPlayerChangeFlightControlState;
	static sigc::signal<void> onPlayerChangeEquipment;
	static sigc::signal<void, const SpaceStation*> onDockingClearanceExpired;

	static LuaSerializer *luaSerializer;
	static LuaTimer *luaTimer;

	static LuaNameGen *luaNameGen;

	static RefCountedPtr<UI::Context> ui;

	static Random rng;
	static int statSceneTris;

	static void SetView(View *v);
	static View *GetView() { return currentView; }

#if WITH_DEVKEYS
	static bool showDebugInfo;
#endif
#if PIONEER_PROFILER
	static std::string profilerPath;
	static bool doProfileSlow;
	static bool doProfileOne;
#endif

	static Player *player;
	static SectorView *sectorView;
	static GalacticView *galacticView;
	static UIView *settingsView;
	static SystemInfoView *systemInfoView;
	static SystemView *systemView;
	static WorldView *worldView;
	static DeathView *deathView;
	static UIView *spaceStationView;
	static UIView *infoView;
	static LuaConsole *luaConsole;
	static ShipCpanel *cpan;
	static Sound::MusicPlayer &GetMusicPlayer() { return musicPlayer; }
	static Graphics::Renderer *renderer;
	static ModelCache *modelCache;
	static Intro *intro;
	static SDLGraphics *sdl;
	static MouseCursor* mouseCursor;

#if WITH_OBJECTVIEWER
	static ObjectViewerView *objectViewerView;
#endif

	static Game *game;

	static struct DetailLevel detail;
	static GameConfig *config;

	static JobQueue *Jobs() { return jobQueue.get();}

	static bool DrawGUI;


	static void SetExpChromaticAberrationEnabled(bool state);
	static void SetExpScanlinesEnabled(bool state);
	static void SetExpFilmGrainEnabled(bool state);

private:
	static void HandleEvents();
	static void InitJoysticks();

	static std::unique_ptr<JobQueue> jobQueue;

	static bool menuDone;

	static View *currentView;

	/** So, the game physics rate (50Hz) can run slower
	  * than the frame rate. gameTickAlpha is the interpolation
	  * factor between one physics tick and another [0.0-1.0]
	  */
	static float gameTickAlpha;
	static int timeAccelIdx;
	static int requestedTimeAccelIdx;
	static bool forceTimeAccel;
	static float frameTime;
	static double lazyTimer; // Timer that only counts when called, used for non-time priority systems like UI
	static std::map<SDL_Keycode,bool> keyState;
	static int keyModState;
	static char mouseButton[6];
	static int mouseMotion[2];
	static bool doingMouseGrab;
	static bool warpAfterMouseGrab;
	static int mouseGrabWarpPos[2];
	static const float timeAccelRates[];

	static bool joystickEnabled;
	static bool mouseYInvert;
	struct JoystickState {
		SDL_Joystick *joystick;
		std::vector<bool> buttons;
		std::vector<int> hats;
		std::vector<float> axes;
		std::string name;
		std::vector<bool> invertAxes;
		std::vector<int> deadZoneAxes;
	};
	static std::vector<SDL_JoystickID> joystickIDs;
	static std::map<SDL_JoystickID,JoystickState> joysticks;
	static int activeJoystick;

	static Sound::MusicPlayer musicPlayer;

	static bool navTunnelDisplayed;
	static bool speedLinesDisplayed;
	static bool targetIndicatorsDisplayed;
	static bool postProcessingEnabled;

	static Gui::Fixed *menu;

	static Graphics::PostProcess* m_gamePP;
	static Graphics::PostProcess* m_guiPP;
	static SMAA* m_smaa;
	static Graphics::RenderTarget *renderTarget;
	static RefCountedPtr<Graphics::Texture> renderTexture;
	static std::unique_ptr<Graphics::Drawables::TexturedQuad> renderQuad;
	static Graphics::RenderState *quadRenderState;

	static unsigned m_chromaticAberrationId;
	static unsigned m_scanlinesId;
	static unsigned m_filmgrainId;

	// Tweaker settings
	static TS_ToneMapping ts_tonemapping, ts_default_tonemapping;
	static TS_FilmGrain ts_filmgrain, ts_default_filmgrain;
	static TS_Scanlines ts_scanlines, ts_default_scanlines;
	static TS_ChromaticAberration ts_chromatic, ts_default_chromatic;
	static TS_FXAA ts_fxaa, ts_default_fxaa;
};

#endif /* _PI_H */
