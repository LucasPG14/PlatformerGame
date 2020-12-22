#include "Input.h"
#include "SceneManager.h"
#include "App.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Scenes.h"
#include "SceneIntro.h"
#include "SceneDie.h"
#include "SceneWin.h"

SceneManager::SceneManager() : Module()
{
	name.Create("scenes");

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
		if (sceneItem->data->active == true)
		{
			sceneItem->data->Load();
			break;
		}

		sceneItem = sceneItem->next;
	}

	return ret;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

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

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		if (sceneItem->data->active == true)
		{
			ret = sceneItem->data->Update(dt);
			break;
		}

		sceneItem = sceneItem->next;
	}

	// Quit the game
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	
	return ret;
}

bool SceneManager::PostUpdate()
{
	bool ret = true;

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		if (sceneItem->data->active == true)
		{
			sceneItem->data->Draw();
			break;
		}

		sceneItem = sceneItem->next;
	}

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	ListItem<Scenes*>* sceneItem = scenes.start;

	while (sceneItem != nullptr)
	{
		ret = sceneItem->data->Unload();
		RELEASE(sceneItem->data);
		sceneItem = sceneItem->next;
	}

	scenes.Clear();

	return ret;
}

void SceneManager::AddScene(Scenes* scene, bool active)
{
	scene->active = active;
	scenes.Add(scene);
}