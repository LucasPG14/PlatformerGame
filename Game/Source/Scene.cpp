#include "Scene.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Pathfinding.h"
#include "Fonts.h"
#include "ColliderManagement.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Scenes()
{
	name.Create("scene1");

	resumeBtn = new GuiButton(1, { 540, 305, 200, 20 }, "RESUME");
	resumeBtn->SetObserver(this);
	resumeBtn->state = GuiControlState::NORMAL;

	settingsBtn = new GuiButton(2, { 540, 335, 200, 20 }, "SETTINGS");
	settingsBtn->SetObserver(this);
	settingsBtn->state = GuiControlState::NORMAL;

	backToTitleBtn = new GuiButton(3, { 540, 365, 200, 20 }, "BACKTOTITLE");
	backToTitleBtn->SetObserver(this);
	backToTitleBtn->state = GuiControlState::NORMAL;

	exitBtn = new GuiButton(4, { 540, 395, 200, 20 }, "EXIT");
	exitBtn->SetObserver(this);
	exitBtn->state = GuiControlState::NORMAL;

	timer = 0;
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
	// Loading textures
	bg = app->tex->Load("Assets/Textures/Backgrounds/level1_dark_trees_background.png");
	bg2 = app->tex->Load("Assets/Textures/Backgrounds/level1_trees_background.png");
	bg3 = app->tex->Load("Assets/Textures/Backgrounds/level1_ground_background.png");

	// Load map
	app->map->Load("level1.tmx");

	app->pathfinding->Enable();

	app->sceneManager->pause = false;

	// Loading entities
	LoadingEntities();

	// Loading Player
	player = (Player*)app->entityManager->AddEntity(iPoint(10, 600), EntityType::PLAYER);

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_8-8bit_march_10_minutes.ogg");

	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = app->fonts->Load("Assets/Hud/yellow_font.png", lookupTable, 5);

	lifesTex = app->tex->Load("Assets/Hud/lifes.png");
	starTex = app->tex->Load("Assets/Hud/star_tex.png");

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;
	
	if (!app->sceneManager->pause)
	{
		time -= dt;
		if (time >= 1.0f)
		{
			time = 0;
			timer++;
		}

		if (player->godMode == false) app->colliderManager->Update(dt, player);

		if (player->playerChangePos == true)
		{
			player->playerChangePos = false;
			PlayerPosition();
			if (player->whatCheckpoint != 0) app->LoadGameRequest();
		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN ||
			app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			TransitionToScene(SceneType::GAMEPLAY);

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			LoadingEntities();
			app->LoadGameRequest();
		}

		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) app->map->viewCollisions = !app->map->viewCollisions;

		if (player->IsDead() == true && player->time >= 1.0f)
		{
			TransitionToScene(SceneType::LOSE);
			player->time = 0;
		}


		if (player->LevelFinished() == true) TransitionToScene(SceneType::WIN);
	}
	else
	{
		resumeBtn->Update(app->input, dt);
		settingsBtn->Update(app->input, dt);
		backToTitleBtn->Update(app->input, dt);
		exitBtn->Update(app->input, dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->sceneManager->pause = !app->sceneManager->pause;
		app->audio->MusicPause();
	}

	if (exit) ret = false;

	return ret;
}

// Called each loop iteration
bool Scene::Draw()
{
	bool ret = true;

	app->render->DrawTexture(bg, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(bg2, 0, 0, NULL, 0.75f);
	app->render->DrawTexture(bg3, 0, 0, NULL, 1.0f);
	
	// Draw map
	app->map->Draw();

	switch (player->whatCheckpoint)
	{
	case 0:
		break;
	case 1:
		app->render->DrawTexture(player->checkpointTex, 2830, 266, &player->checkpointAnim.GetCurrentFrame());
		break;
	case 2:
		app->render->DrawTexture(player->checkpointTex, 2835, 1070, &player->checkpointAnim.GetCurrentFrame());
		break;
	case 3:
		app->render->DrawTexture(player->checkpointTex, 2831, 1500, &player->checkpointAnim.GetCurrentFrame());
		break;
	}

	app->entityManager->Draw();
	
	// Draw collisions
	if (app->map->viewCollisions) app->colliderManager->DrawColliders();

	//Stars in HUD
	app->fonts->BlitText(1130, 10, yellowFont, "x");
	app->fonts->BlitText(1170, 10, yellowFont, std::to_string(app->sceneManager->stars).c_str());
	app->fonts->BlitText(570, 10, yellowFont, std::to_string(timer).c_str());
	SDL_Rect rect = { 0,0,1280,50 };
	app->render->DrawTexture(starTex, ((app->render->camera.x * -1) + app->render->camera.w - 69), (app->render->camera.y * -1) + 5);

	// Score in HUD
	app->fonts->BlitText(650, 10, yellowFont, std::to_string(app->sceneManager->score).c_str());

	for (int i = 0; i < player->lifes; i++)
	{
		app->render->DrawTexture(lifesTex, app->render->camera.x * -1 + (36 * i + 10), (app->render->camera.y * -1) + 10);
	}

	if (app->sceneManager->pause)
	{
		resumeBtn->Draw(app->render);
		settingsBtn->Draw(app->render);
		backToTitleBtn->Draw(app->render);
		exitBtn->Draw(app->render);
	}

	return ret;
}

// Called before quitting
bool Scene::Unload()
{
	LOG("Freeing scene");

	app->sceneManager->pause = false;
	app->audio->MusicPause();

	//Unload the background
	app->tex->UnLoad(bg);
	app->tex->UnLoad(bg2);
	app->tex->UnLoad(bg3);

	// Unloading UI
	app->tex->UnLoad(lifesTex);
	app->tex->UnLoad(starTex);

	// Unload fonts
	app->fonts->UnLoad(yellowFont);

	app->colliderManager->CleanUp();
	app->entityManager->CleanUp();
	app->map->CleanUp();
	app->pathfinding->CleanUp();

	return true;
}

void Scene::LoadingEntities()
{
	app->entityManager->CleanUp();

	//Load Enemies
	app->entityManager->AddEntity(iPoint(2275, 667), EntityType::SLIME);

	app->entityManager->AddEntity(iPoint(1798, 1286), EntityType::BAT);

	// Add lifes
	app->entityManager->AddEntity(iPoint(1222, 1401), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(1572, 218), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(2080, 899), EntityType::LIFE);

	// Add stars
	app->entityManager->AddEntity(iPoint(1200, 300), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(1290, 955), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(505, 1025), EntityType::STAR);
}

void Scene::PlayerPosition()
{
	player->position.x = 10;
	player->position.y = 600;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->sceneManager->pause = !app->sceneManager->pause;
		else if (control->id == 2);
		else if (control->id == 3) TransitionToScene(SceneType::TITLE);
		else if (control->id == 4) exit = true;
	}
	default: break;
	}

	return true;
}
