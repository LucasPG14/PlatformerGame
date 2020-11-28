#include "SceneWin.h"
#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"

#include "Input.h"
#include "Audio.h"

#include "FadeToBlack.h"

#include "SDL/include/SDL_scancode.h"


SceneWin::SceneWin() : Module()
{
	for (int i = 0; i < 5; i++)
	{
		winAnim.PushBack({ 1500 * i, 0, 1500, 1020 });
	}

	winAnim.loop = true;
	winAnim.speed = 0.1f;
}

SceneWin::~SceneWin()
{

}

// Load assets
bool SceneWin::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_win.png");
		winFx = app->audio->LoadFx("Assets/Audio/Fx/you_win.ogg");
		time = 0;
	}

	return ret;
}

bool SceneWin::Update(float dt)
{
	time++;

	if (time == 1) app->audio->PlayFx(winFx);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
		app->fade->Fade(this, (Module*)app->sceneIntro, 60);


	winAnim.Update();

	return true;
}

// PostUpdate: draw background
bool SceneWin::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, &winAnim.GetCurrentFrame());

	return true;
}

bool SceneWin::CleanUp() 
{
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	winAnim.Reset();
	this->active = false;
	return true;
}