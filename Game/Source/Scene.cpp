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
#include "EntityManager.h"
#include "Pathfinding.h"
#include "SceneDie.h"
#include "Slime.h"
#include "Fonts.h"
#include "Bat.h"
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
bool Scene::Load()
{
	// Load map
	app->map->Enable();
	app->map->Load("level1.tmx");

	//Load Player
	PlayerPosition();
	app->player->Enable();
	app->entityManager->Enable();
	app->pathfinding->Enable();
	app->fonts->Enable();


	//Load Enemies
	slime2 = (Slime*)app->entityManager->AddEntity(iPoint(2275, 667), EntityType::SLIME);

	bat = (Bat*)app->entityManager->AddEntity(iPoint(1798, 1286), EntityType::BAT);

	// Add lifes
	app->entityManager->AddEntity(iPoint(1222, 1401), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(1572, 218), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(2080, 899), EntityType::LIFE);

	// Add stars
	app->entityManager->AddEntity(iPoint(1200, 300), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(1290, 955), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(505, 1025), EntityType::STAR);

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_8-8bit_march_10_minutes.ogg");

	// Loading textures
	bg = app->tex->Load("Assets/Textures/Backgrounds/level1_dark_trees_background.png");
	bg2 = app->tex->Load("Assets/Textures/Backgrounds/level1_trees_background.png");
	bg3 = app->tex->Load("Assets/Textures/Backgrounds/level1_ground_background.png");


	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;

	app->colliderManager->Update(dt);

	if (app->player->playerChangePos == true)
	{
		PlayerPosition();
		app->player->playerChangePos = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || 
	app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->Fade(this, this, 1/dt);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->map->viewCollisions = !app->map->viewCollisions;

	if (app->player->IsDead() == true && app->player->time == 60)
	{
		app->fade->Fade(this, app->sceneManager->dieScene, 1 / dt);
		app->player->time = 0;
	}

	if (app->player->LevelFinished() == true)
		app->fade->Fade(this, app->sceneManager->winScene, 1 / dt);

	return ret;
}

// Called each loop iteration
bool Scene::Draw()
{
	bool ret = true;

	app->render->DrawTexture(bg, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(bg2, 0, 0, NULL, 0.75f);
	app->render->DrawTexture(bg3, 0, 0, NULL, 1.0f);

	switch (app->player->whatCheckpoint)
	{
	case 0:
		break;
	case 1:
		app->render->DrawTexture(app->player->checkpointTex, 2830, 266, &app->player->checkpointAnim.GetCurrentFrame());
		break;
	case 2:
		app->render->DrawTexture(app->player->checkpointTex, 2835, 1070, &app->player->checkpointAnim.GetCurrentFrame());
		break;
	case 3:
		app->render->DrawTexture(app->player->checkpointTex, 2831, 1500, &app->player->checkpointAnim.GetCurrentFrame());
		break;
	}

	// Draw map
	app->map->Draw();
	app->entityManager->Draw();
	
	// Draw collisions
	if (app->map->viewCollisions) app->colliderManager->DrawColliders();

	return ret;
}

// Called before quitting
bool Scene::Unload()
{
	LOG("Freeing scene");

	app->audio->PlayMusic("Assets/Audio/Music/silence.ogg");

	//Unload the background
	app->tex->UnLoad(bg);
	app->tex->UnLoad(bg2);
	app->tex->UnLoad(bg3);
	app->colliderManager->Disable();
	app->player->Disable();
	app->entityManager->Disable();
	app->map->Disable();
	app->pathfinding->Disable();

	return true;
}

void Scene::PlayerPosition()
{
	app->player->SetPosition(10, 600);
}