#include "SceneIntro.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Animation.h"
#include "SceneManager.h"
#include "Scenes.h"
#include "Scene.h"

#include "FadeToBlack.h"
#include "SDL/include/SDL_scancode.h"


SceneIntro::SceneIntro() : Scenes()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			introAnim.PushBack({ 1280 * j, 720 * i, 1280, 720 });
		}
	}
	introAnim.loop = false;
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_intro.png");
	logoTexture = app->tex->Load("Assets/Textures/Backgrounds/logo_real_ambient.png");
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_iron_is_laughter.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	if (time < 1000) time++;

	if (time == 1) introAnim.speed = 0.05f * dt;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		if (time < 120) time = 121;
		else if (time > 120) app->fade->Fade(this, app->sceneManager->level1, 1 / dt);
		
	}

	if (time > 120) introAnim.Update();

	return ret;
}

// PostUpdate: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	// Draw everything 
	if (time > 120) 
		app->render->DrawTexture(bgTexture, 0, 0, &introAnim.GetCurrentFrame());

	else 
		app->render->DrawTexture(logoTexture, 250, 10, NULL);

	return ret;
}

bool SceneIntro::CleanUp() 
{
	bool ret = true;

	app->audio->PlayMusic("Assets/Audio/Music/silence.ogg");

	app->tex->UnLoad(bgTexture);
	app->tex->UnLoad(logoTexture);
	introAnim.Reset();
	time = 301;

	return true;
}