#include "SceneIntro.h"
#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Animation.h"
#include "Map.h"
#include "Scene.h"

#include "FadeToBlack.h"
#include "SDL/include/SDL_scancode.h"


SceneIntro::SceneIntro() : Module()
{
	name.Create("intro");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			introAnim.PushBack({ 1500 * j,1020 * i,1500,1020 });
		}
	}
	introAnim.loop = false;
	introAnim.speed = 0.005f;
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	bool ret = true;

	if(this->active == true)
	{
		bgTexture = app->tex->Load("Assets/textures/Backgrounds/BackgroundIntro.png");
		logoTexture = app->tex->Load("Assets/textures/Backgrounds/LogoRealAmbient.png");
		app->scene->Disable();
		app->map->Disable();
		app->player->Disable();
	}

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	time++;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
		app->fade->Fade(this, (Module*)app->scene, 60);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN) ret = false;

	if (time > 10000) introAnim.Update();

	return ret;
}

// PostUpdate: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	// Draw everything 
	if (time > 10000) 
		app->render->DrawTexture(bgTexture, 0, 0, &introAnim.GetCurrentFrame());

	else 
		app->render->DrawTexture(logoTexture, 250, 10, NULL);

	return ret;
}

bool SceneIntro::CleanUp() 
{
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	app->tex->UnLoad(logoTexture);
	introAnim.Reset();
	time = 10001;

	return true;
}