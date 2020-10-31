#include "SceneIntro.h"
#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"

#include "FadeToBlack.h"
#include "SDL/include/SDL_scancode.h"


SceneIntro::SceneIntro() : Module()
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	bool ret = true;
	bgTexture = app->tex->Load("Assets/textures/Backgrounds/BackgroundIntro.png");
	//App->audio->PlayMusic("Assets/music/Build/music/opening.ogg", 1.0f);

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F7) == KeyState::KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 60);
	}

	return ret;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

bool SceneIntro::CleanUp() {
	bool ret = true;

	//app->tex->UnLoad(bgTexture);
	//app->audio->CleanUp();
	return true;
}