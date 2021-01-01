#include "Input.h"
#include "SceneManager.h"
#include "App.h"
#include "Scene.h"
#include "Render.h"
#include "SceneLogo.h"
#include "SceneIntro.h"
#include "SceneDie.h"
#include "SceneWin.h"

#include "Log.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager() : Module()
{
	name.Create("scenes");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;

	pause = false;
	continueClicked = false;
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

	current = new SceneLogo();
	current->Load();

	next = nullptr;

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

	if (!onTransition)
	{
		ret = current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->Unload();	// Unload current screen
				next->Load();	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw();

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, 720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO: next = new SceneLogo(); break;
		case SceneType::TITLE: next = new SceneIntro(); break;
		case SceneType::GAMEPLAY: next = new Scene(); break;
		case SceneType::WIN: next = new SceneWin(); break;
		case SceneType::LOSE: next = new SceneDie(); break;
		default: break;
		}

		current->transitionRequired = false;
	}

	// Quit the game
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && current->name != "scene1") ret = false;
	
	//High Score Mechanism
	lastScore = finalScore;
	highScore = highScore < lastScore ? lastScore : highScore;

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	LOG("Unloading scene");
	if (current != nullptr) current->Unload();

	return ret;
}

bool SceneManager::LoadState(pugi::xml_node& load)
{
	saved = load.child("save").attribute("value").as_bool();

	if (saved)
	{
		score = load.child("score").attribute("value").as_int();
		stars = load.child("stars").attribute("value").as_int();
	}

	return true;
}

bool SceneManager::SaveState(pugi::xml_node& save) const
{
	save.append_child("save").append_attribute("value").set_value(saved);

	save.append_child("score").append_attribute("value").set_value(score);
	save.append_child("stars").append_attribute("value").set_value(stars);

	return true;
}
