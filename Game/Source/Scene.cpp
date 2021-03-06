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

	resumeBtn = new GuiButton(1, { 473, 420, 334, 64 }, "RESUME");
	resumeBtn->SetObserver(this);
	resumeBtn->state = GuiControlState::DISABLED;
	resumeBtn->section = { 192,300,324,54 };

	settingsBtn = new GuiButton(2, { 603, 335, 74, 74 }, "SETTINGS");
	settingsBtn->SetObserver(this);
	settingsBtn->state = GuiControlState::DISABLED;
	settingsBtn->section = { 64,300,64,64 };

	backToTitleBtn = new GuiButton(3, { 473, 335, 74, 74 }, "BACKTOTITLE");
	backToTitleBtn->SetObserver(this);
	backToTitleBtn->state = GuiControlState::DISABLED;
	backToTitleBtn->section = { 0,300,64,64 };

	exitBtn = new GuiButton(4, { 733, 335, 74, 74 }, "EXIT");
	exitBtn->SetObserver(this);
	exitBtn->state = GuiControlState::DISABLED;
	exitBtn->section = { 128,300,64,64 };

	musicVolumeSlider = new GuiSlider(5, { 596, 321, 250, 25 }, "MUSIC VOLUME", 0, 128);
	musicVolumeSlider->SetObserver(this);
	musicVolumeSlider->state = GuiControlState::DISABLED;
	musicVolumeSlider->section = { 516,300,240,15 };

	fxVolumeSlider = new GuiSlider(6, { 596, 354, 250, 25 }, "FX VOLUME", 0, 128);
	fxVolumeSlider->SetObserver(this);
	fxVolumeSlider->state = GuiControlState::DISABLED;
	fxVolumeSlider->section = { 516,300,240,15 };

	fullscreenSting = new GuiCheckBox(7, { 821,387,25,25 }, "FULLSCREEN");
	fullscreenSting->SetObserver(this);
	fullscreenSting->state = GuiControlState::DISABLED;
	fullscreenSting->section = { 556,315,17,17 };

	vsyncSting = new GuiCheckBox(8, { 821,420,25,25 }, "VSYNC");
	vsyncSting->SetObserver(this);
	vsyncSting->state = GuiControlState::DISABLED;
	vsyncSting->section = { 556,315,17,17 };

	backBtn = new GuiButton(9, { 796, 451, 50, 50 }, "BACK");
	backBtn->SetObserver(this);
	backBtn->state = GuiControlState::DISABLED;
	backBtn->section = { 516,315,40,40 };

	app->sceneManager->timer = 0;
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
	app->map->Enable();
	app->map->Load("level1.tmx");

	app->pathfinding->Enable();

	app->sceneManager->pause = false;

	// Loading entities
	LoadingEntities();

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_8-8bit_march_10_minutes.ogg");

	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,��?!*$%&()+-/:;<=>@�    " };
	yellowFont = app->fonts->Load("Assets/Hud/yellow_font.png", lookupTable, 5);

	lifesTex = app->tex->Load("Assets/Hud/lifes.png");
	starTex = app->tex->Load("Assets/Hud/star_tex.png");

	guiTexture = app->tex->Load("Assets/Hud/pause.png");

	settingsEnabled = false;

	resumeBtn->texture = guiTexture;
	settingsBtn->texture = guiTexture;
	backToTitleBtn->texture = guiTexture;
	exitBtn->texture = guiTexture;
	musicVolumeSlider->texture = guiTexture;
	fxVolumeSlider->texture = guiTexture;
	fullscreenSting->texture = guiTexture;
	vsyncSting->texture = guiTexture;
	backBtn->texture = guiTexture;

	app->sceneManager->score = 0;
	app->sceneManager->stars = 0;

	if (app->sceneManager->saved && app->sceneManager->continueClicked) app->LoadGameRequest();
		
	app->sceneManager->continueClicked = false;

	exit = false;
	time = 0;

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;
	
	if (!app->sceneManager->pause)
	{
		time += dt;
		if (time >= 1.0f)
		{
			time = 0;
			app->sceneManager->timer++;
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
			TransitionToScene(SceneType::LEVEL_1);

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->sceneManager->saved = true;
			app->SaveGameRequest();
		}

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
		if (!settingsEnabled)
		{
			resumeBtn->Update(app->input, dt);
			settingsBtn->Update(app->input, dt);
			backToTitleBtn->Update(app->input, dt);
			exitBtn->Update(app->input, dt);
		}

		if (settingsEnabled)
		{
			musicVolumeSlider->state = GuiControlState::NORMAL;
			fxVolumeSlider->state = GuiControlState::NORMAL;
			fullscreenSting->state = GuiControlState::NORMAL;
			vsyncSting->state = GuiControlState::NORMAL;
			backBtn->state = GuiControlState::NORMAL;
			musicVolumeSlider->Update(app->input, dt);
			fxVolumeSlider->Update(app->input, dt);
			fullscreenSting->Update(app->input, dt);
			vsyncSting->Update(app->input, dt);
			backBtn->Update(app->input, dt);
		}
		else
		{
			musicVolumeSlider->state = GuiControlState::DISABLED;
			fxVolumeSlider->state = GuiControlState::DISABLED;
			fullscreenSting->state = GuiControlState::DISABLED;
			vsyncSting->state = GuiControlState::DISABLED;
			backBtn->state = GuiControlState::DISABLED;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->sceneManager->pause = !app->sceneManager->pause;
		app->audio->MusicPause();
		if (app->sceneManager->pause == true)
		{
			resumeBtn->state = GuiControlState::NORMAL;
			settingsBtn->state = GuiControlState::NORMAL;
			backToTitleBtn->state = GuiControlState::NORMAL;
			exitBtn->state = GuiControlState::NORMAL;
		}
		else
		{
			resumeBtn->state = GuiControlState::DISABLED;
			settingsBtn->state = GuiControlState::DISABLED;
			backToTitleBtn->state = GuiControlState::DISABLED;
			exitBtn->state = GuiControlState::DISABLED;
		}
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
	app->fonts->BlitText(450, 10, yellowFont, std::to_string(app->sceneManager->timer).c_str());
	SDL_Rect rect = { 0,0,1280,50 };
	app->render->DrawTexture(starTex, ((app->render->camera.x * -1) + app->render->camera.w - 69), (app->render->camera.y * -1) + 5);

	// Score in HUD
	app->fonts->BlitText(700, 10, yellowFont, std::to_string(app->sceneManager->score).c_str());

	for (int i = 0; i < player->lifes; i++)
	{
		app->render->DrawTexture(lifesTex, app->render->camera.x * -1 + (36 * i + 10), (app->render->camera.y * -1) + 10);
	}

	if (app->sceneManager->pause)
	{
		SDL_Rect rect = { 0, 0, 450,300 };
		app->render->DrawTexture(guiTexture, (int)(app->render->offset.x + 415), (int)(app->render->offset.y + 210), &rect);
		resumeBtn->Draw(app->render);
		settingsBtn->Draw(app->render);
		backToTitleBtn->Draw(app->render);
		exitBtn->Draw(app->render);

		if (settingsEnabled)
		{
			SDL_Rect rect = { 450, 0, 450,300 };
			app->render->DrawTexture(guiTexture, (int)(app->render->offset.x + 415), (int)(app->render->offset.y + 210), &rect);
			musicVolumeSlider->Draw(app->render);
			fxVolumeSlider->Draw(app->render);
			fullscreenSting->Draw(app->render);
			vsyncSting->Draw(app->render);
			backBtn->Draw(app->render);
		}
	}

	if (app->sceneManager->guiDebug == true)
	{
		resumeBtn->guiDebug = true;
		settingsBtn->guiDebug = true;
		backToTitleBtn->guiDebug = true;
		exitBtn->guiDebug = true;
		musicVolumeSlider->guiDebug = true;
		fxVolumeSlider->guiDebug = true;
		fullscreenSting->guiDebug = true;
		vsyncSting->guiDebug = true;
		backBtn->guiDebug = true;
	}
	else
	{
		resumeBtn->guiDebug = false;
		settingsBtn->guiDebug = false;
		backToTitleBtn->guiDebug = false;
		exitBtn->guiDebug = false;
		musicVolumeSlider->guiDebug = false;
		fxVolumeSlider->guiDebug = false;
		fullscreenSting->guiDebug = false;
		vsyncSting->guiDebug = false;
		backBtn->guiDebug = false;
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

	delete resumeBtn;
	delete settingsBtn;
	delete backToTitleBtn;
	delete exitBtn;
	delete musicVolumeSlider;
	delete fxVolumeSlider;
	delete fullscreenSting;
	delete vsyncSting;
	delete backBtn;
	app->tex->UnLoad(guiTexture);

	return true;
}

void Scene::LoadingEntities()
{
	app->entityManager->CleanUp();

	// Loading Player
	player = (Player*)app->entityManager->AddEntity(iPoint(10, 600), EntityType::PLAYER);

	// Load Enemies
	app->entityManager->AddEntity(iPoint(2275, 667), EntityType::SLIME);
	app->entityManager->AddEntity(iPoint(1798, 1346), EntityType::BAT);

	// Add lifes
	app->entityManager->AddEntity(iPoint(1222, 1401), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(1442, 229), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(2080, 899), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(3272, 139), EntityType::LIFE);
	app->entityManager->AddEntity(iPoint(3827, 1323), EntityType::LIFE);

	// Add stars
	app->entityManager->AddEntity(iPoint(857, 351), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(1290, 955), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(505, 1025), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(2542, 505), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(3184, 798), EntityType::STAR);
	app->entityManager->AddEntity(iPoint(5378, 925), EntityType::STAR);
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
		if (control->id == 1)
		{
			app->sceneManager->pause = !app->sceneManager->pause;
			app->audio->MusicPause();
		}
		else if (control->id == 2) settingsEnabled = !settingsEnabled;
		else if (control->id == 3) TransitionToScene(SceneType::INTRO);
		else if (control->id == 4) exit = true;
		else if (control->id == 9) settingsEnabled = !settingsEnabled;
	case GuiControlType::SLIDER:
	{
		if (control->id == 5) app->audio->SetMusicVolume(musicVolumeSlider->GetValue());
		else if (control->id == 6) app->audio->SetFxVolume(fxVolumeSlider->GetValue());
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 7) app->win->fullscreen = !app->win->fullscreen;
		else if (control->id == 8) app->render->vsync = !app->render->vsync;
	}
	default: break;
	}
	}
	return true;
}