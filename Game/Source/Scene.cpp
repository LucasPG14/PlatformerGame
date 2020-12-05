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
#include "Pathfinding.h"
#include "SceneDie.h"
#include "Slime.h"
#include "Bat.h"
#include "SceneWin.h"
#include "ColliderManagement.h"
#include "EnemyManager.h"

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
	//Load Player
	app->player->active = true;
	PlayerPosition();
	app->player->Start();

	//Load Enemies
	slime = (Slime*)app->enemyManager->AddEnemy(iPoint(0, 600), EnemyType::SLIME);

	bat = (Bat*)app->enemyManager->AddEnemy(iPoint(100, 500), EnemyType::BAT);

	app->enemyManager->AddEnemy(iPoint(4178, 633), EnemyType::LIFE);
	app->enemyManager->AddEnemy(iPoint(3829, 1264), EnemyType::LIFE);
	app->enemyManager->AddEnemy(iPoint(4211, 1070), EnemyType::LIFE);

	app->enemyManager->AddEnemy(iPoint(1200, 300), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(1290, 955), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(505, 1025), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(2511, 1375), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(2719, 583), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(3414, 420), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(3446, 798), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(3933, 165), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(5373, 336), EnemyType::STAR);
	app->enemyManager->AddEnemy(iPoint(4426, 418), EnemyType::STAR);

	app->enemyManager->active = true;
	app->enemyManager->Start();

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_8-8bit_march_10_minutes.ogg");

	bg = app->tex->Load("Assets/Textures/Backgrounds/level1_dark_trees_background.png");
	bg2 = app->tex->Load("Assets/Textures/Backgrounds/level1_trees_background.png");
	bg3 = app->tex->Load("Assets/Textures/Backgrounds/level1_ground_background.png");
	// Load map
	app->map->active = true;
	app->map->Load("level1.tmx");


	//app->LoadGameRequest();

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
		ResetEnemiesPosition();
		app->player->playerChangePos = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || 
	app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->Fade(this, this, 1/dt);

		// Pathfinding testing inputs
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		app->pathfinding->ResetPath(iPoint(12, 40));
		app->pathfinding->finishAStar = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		//app->pathfinding->PropagateDijkstra();
		//app->pathfinding->PropagateAStar();

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		//app->pathfinding->PropagateDijkstra();
		//app->pathfinding->PropagateAStar();

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
		app->pathfinding->ComputePathAStar(app->pathfinding->goalAStar.x, app->pathfinding->goalAStar.y);

	if (app->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		iPoint p;
		app->input->GetMousePosition(p.x, p.y);
		app->pathfinding->goalAStar = app->map->WorldToMap(p.x - app->render->camera.x, p.y - app->render->camera.y);
		app->pathfinding->checkPath = true;
		
		//app->pathfinding->ComputePath(p.x - app->render->camera.x - app->map->data.tileWidth, p.y - app->render->camera.y - app->map->data.tileHeight);
		//app->pathfinding->ComputePath(p.x, p.y);
	}

	if (app->player->IsDead() == true && app->player->time == 60)
	{
		app->fade->Fade(this, app->sceneManager->dieScene, 1 / dt);
		app->player->time = 0;
	}

	if (app->player->LevelFinished() == true)
		app->fade->Fade(this, app->sceneManager->winScene, 1 / dt);

	app->enemyManager->Update(dt);

	return ret;
}

// Called each loop iteration
bool Scene::PostUpdate()
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
	app->pathfinding->DrawPath();

	app->enemyManager->Draw();
	app->colliderManager->DrawColliders();

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
	app->enemyManager->CleanUp();
	app->player->CleanUp();
	app->map->CleanUp();
	//app->pathfinding->CleanUp();

	return true;
}

void Scene::PlayerPosition()
{
	app->player->position.x = 200;
	app->player->position.y = 600;
}

void Scene::ResetEnemiesPosition()
{
	slime->pos.x = 0;
	slime->pos.y = 600;

	bat->pos.x = 100;
	bat->pos.y = 500;
}