#include "SceneManager.h"
#include "App.h"
#include "Input.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneDie.h"
#include "SceneWin.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Scenes.h"

#include "SDL/include/SDL_scancode.h"

SceneManager::SceneManager() : Module()
{
	intro = new SceneIntro();
	level1 = new Scene();
	dieScene = new SceneDie();
	winScene = new SceneWin();

	AddScene(intro, true);
	AddScene(level1, false);
	AddScene(dieScene, false);
	AddScene(winScene, false);
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool SceneManager::Start()
{
	bool ret = true;

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		if(sceneItem->data->active == true)
			sceneItem->data->Start();

		sceneItem = sceneItem->next;
	}

	return ret;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;
	// Quit the game
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	// Move the camera up
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		app->render->camera.y += floor(200.0f * dt);
		app->render->offset.y -= floor(200.0f * dt);
	}

	// Move the camera down
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		app->render->camera.y -= floor(200.0f * dt);
		app->render->offset.y += floor(200.0f * dt);
	}

	// Move the camera to the left
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		app->render->camera.x += floor(200.0f * dt);
		app->render->offset.x -= floor(200.0f * dt);
	}

	// Move the camera to the right
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		app->render->camera.x -= floor(200.0f * dt);
		app->render->offset.x += floor(200.0f * dt);
	}

	// Save game
	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->SaveGameRequest();

	// Load game
	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->LoadGameRequest();

	// View colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->map->viewCollisions = !app->map->viewCollisions;
		app->colliderManager->showColliders = !app->colliderManager->showColliders;
	}

	// Cap the game to 30 FPS
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (app->cappedMs == 1000 / 60)
		{
			app->cappedMs = 30;
		}
	}

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		if (sceneItem->data->active == true)
			sceneItem->data->Update(dt);

		sceneItem = sceneItem->next;
	}

	return ret;
}

bool SceneManager::PostUpdate()
{
	bool ret = true;

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		if (sceneItem->data->active == true)
			sceneItem->data->PostUpdate();

		sceneItem = sceneItem->next;
	}

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	return ret;
}

void SceneManager::AddScene(Scenes* scene, bool active)
{
	scene->active = active;
	scenes.Add(scene);
}