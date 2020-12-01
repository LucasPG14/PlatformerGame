#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "SceneManager.h"
#include "SceneDie.h"
#include "SceneWin.h"
#include "ColliderManagement.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Scenes()
{
	name.Create("scene1");
}

// Destructor
Scene::~Scene()
{

}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Load music
	app->player->active = true;
	app->player->Start();
	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_8-8bit_march_10_minutes.ogg");

	bg = app->tex->Load("Assets/Textures/Backgrounds/level1_dark_trees_background.png");
	bg2 = app->tex->Load("Assets/Textures/Backgrounds/level1_trees_background.png");
	bg3 = app->tex->Load("Assets/Textures/Backgrounds/level1_ground_background.png");
	// Load map
	app->map->active = true;
	app->map->Load("level1.tmx");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || 
	//	app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	//	app->fade->Fade(this, this, 1/dt);

	if (app->player->IsDead() == true && app->player->time == 60)
	{
		app->fade->Fade(this, app->sceneManager->dieScene, 1 / dt);
		app->player->time = 0;
		app->player->deadPlayer = false;
	}

	if (app->player->LevelFinished() == true)
		app->fade->Fade(this, app->sceneManager->winScene, 1 / dt);

	return ret;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(bg, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(bg2, 0, 0, NULL, 0.75f);
	app->render->DrawTexture(bg3, 0, 0, NULL, 1.0f);
	app->colliderManager->DrawColliders();
	// Draw map
	if (app->map->active == true) app->map->Draw();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->audio->PlayMusic("Assets/Audio/Music/silence.ogg");

	//Unload the background
	app->tex->UnLoad(bg);
	app->tex->UnLoad(bg2);
	app->tex->UnLoad(bg3);
	app->colliderManager->CleanUp();
	app->player->CleanUp();
	app->map->CleanUp();

	return true;
}

void Scene::PlayerPosition()
{
}