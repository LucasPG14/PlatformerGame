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
	//IsActive(false);
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	stringBackground.Create(config.child("image").attribute("source").as_string(""));

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//app->map->Load("hello2.tmx");
	app->map->Load("SnowMap.tmx");

	SString tmp("%s%s", folder.GetString(), stringBackground.GetString());
	bg = app->tex->Load(tmp.GetString());

	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	//app->player->active = true;

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
    // L02: DONE 3: Request Load / Save when pressing L/S
	//To Delete
	//if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//	app->LoadGameRequest();

	//if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	//	app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	// VIEW COLLIDERS(F9)
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->map->viewCollisions = !app->map->viewCollisions;

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count());

	app->win->SetTitle(title.GetString());

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 60);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(bg, 0, 0, NULL, 1.0f);
	// Draw map
	app->map->Draw();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	//Unload the background
	app->tex->UnLoad(bg);

	return true;
}
