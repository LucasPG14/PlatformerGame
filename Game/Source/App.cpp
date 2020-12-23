#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "ColliderManagement.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(pTimer);
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	entityManager = new EntityManager();
	sceneManager = new SceneManager();
	map = new Map();
	colliderManager = new ColliderManagement();
	fonts = new Fonts();
	pathfinding = new Pathfinding();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win, true);
	AddModule(input, true);
	AddModule(tex, true);
	AddModule(audio, true);
	AddModule(map, true);
	AddModule(pathfinding, true);
	AddModule(entityManager, true);
	AddModule(sceneManager, true);
	AddModule(fonts, true);

	// Render last to swap buffer
	AddModule(render, true);

	PERF_PEEK(pTimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool active)
{
	module->Init(active);
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(pTimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}

		int cap = configApp.attribute("framerate_cap").as_int(-1);

		if (cap > 0) cappedMs = 1000 / cap;
	}

	PERF_PEEK(pTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(pTimer);

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(pTimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Cap the game to 30 FPS
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (cappedMs == 1000 / 60)	cappedMs = 1000 / 30;
		else cappedMs = 1000 / 60;
	}

	// Calculate the differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	SString vsyncMode;
	if (render->vsync) vsyncMode.Create("on");
	else vsyncMode.Create("off");

	static char title[256];
	sprintf_s(title, 256, "FPS: %i / Avg. FPS: %.2f / Last-frame MS: %02u / Vsync: %s ",
		framesOnLastUpdate, averageFps, lastFrameMs, vsyncMode.GetString());

	app->win->SetTitle(title);

	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		PerfTimer pt;
		SDL_Delay(cappedMs - lastFrameMs);
		LOG("We waited for %i ms and got back in %f", cappedMs - lastFrameMs, pt.ReadMs());
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	loadGameRequested = false;
	bool ret = true;

	pugi::xml_parse_result resul = saveLoadFile.load_file("save_game.xml");
	
	if (resul == NULL)
	{
		LOG("Could not load save and load xml file. pugi error: %s", resul.description());
		return false;
	}
	else
	{
		saveState = saveLoadFile.child("save_status");

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret)
		{
			ret = item->data->LoadState(saveState.child(item->data->name.GetString()));
			item = item->next;
		}

		LOG("File loaded successfully!");
	}

	return ret;
}

bool App::SaveGame() const
{
	LOG("Saving Results!!");
	saveGameRequested = false;
	bool ret = true;

	ListItem<Module*>* item = modules.start;
	pugi::xml_document file;

	auto root = file.append_child("save_status");

	while (item != NULL)
	{
		root.append_child(item->data->name.GetString());
		ret = item->data->SaveState(root.child(item->data->name.GetString()));
		item = item->next;
	}

	bool saveSucceed = file.save_file("save_game.xml", PUGIXML_TEXT("  "));
	if (saveSucceed == false)
	{
		LOG("Couldn't save the file. pugi error: %s", pugi::status_internal_error);
	}
	return ret;
}