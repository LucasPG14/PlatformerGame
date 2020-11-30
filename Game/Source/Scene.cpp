#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "ColliderManagement.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
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
	if (this->active == true)
	{
		app->player->Enable();
		// Load music
		app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

		bg = app->tex->Load("Assets/Textures/Backgrounds/level1_dark_trees_background.png");
		bg2 = app->tex->Load("Assets/Textures/Backgrounds/level1_trees_background.png");
		bg3 = app->tex->Load("Assets/Textures/Backgrounds/level1_ground_background.png");
		// Load map
		//app->map->Load("snow_map.tmx");
		app->map->Load("level1.tmx");
		app->map->Enable();

		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;

	// Quit the game
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	// Move the camera up
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += floor(200.0f * dt);

	// Move the camera down
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= floor(200.0f * dt);

	// Move the camera to the left
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= floor(200.0f * dt);

	// Move the camera to the right
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += floor(200.0f * dt);

	// View colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->map->viewCollisions = !app->map->viewCollisions;
		app->colliderManager->showColliders = !app->colliderManager->showColliders;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || 
		app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->Fade(this, (Module*)app->scene, 1/dt);

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (app->cappedMs == 1000 / 60)
		{
			app->cappedMs = 30;
		}
	}

	// Raise up the volume
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->MoreVolume();

	// Lower the volume 
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->LessVolume();

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

	//Unload the background
	app->tex->UnLoad(bg);
	app->tex->UnLoad(bg2);
	app->colliderManager->CleanUp();
	app->player->Disable();
	app->map->Disable();

	return true;
}