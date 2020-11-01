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
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

		bg = app->tex->Load("Assets/textures/Backgrounds/SnowBackground.png");
		bg2 = app->tex->Load("Assets/textures/Backgrounds/SnowBackground2.png");
		// Load map
		app->map->Load("SnowMap.tmx");
		app->map->Enable();
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

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	// View colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->map->viewCollisions = !app->map->viewCollisions;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
		app->fade->Fade(this, (Module*)app->scene, 60);


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(bg, 0, 0, NULL, 0.75f);
	app->render->DrawTexture(bg2, 9100, 0, NULL, 0.75f);
	
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

	app->player->Disable();
	app->map->Disable();

	return true;
}