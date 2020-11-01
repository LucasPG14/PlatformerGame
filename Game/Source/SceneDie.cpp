#include "SceneDie.h"
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


SceneDie::SceneDie() : Module()
{
	dieAnim.PushBack({ 0,0,1502,1020 });
	dieAnim.PushBack({ 1600,0,1502,1020 });
	dieAnim.PushBack({ 3200,0,1502,1020 });
	dieAnim.PushBack({ 4800,0,1502,1020 });
	dieAnim.PushBack({ 6400,0,1502,1020 });
	dieAnim.PushBack({ 8000,0,1502,1020 });

	dieAnim.speed = 0.0008f;
	dieAnim.loop = false;
}

SceneDie::~SceneDie()
{

}

// Load assets
bool SceneDie::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		bgTexture = app->tex->Load("Assets/textures/Backgrounds/BackgroundDead.png");
		dieFx = app->audio->LoadFx("Assets/audio/fx/YouLose.ogg");
		time = 0;
	}

	return ret;
}

bool SceneDie::Update(float dt)
{
	time++;
	if (time == 1) app->audio->PlayFx(dieFx);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
		app->fade->Fade(this, (Module*)app->sceneIntro);

	dieAnim.Update();

	return true;
}

// Update: draw background
bool SceneDie::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = dieAnim.GetCurrentFrame();
	app->render->DrawTexture(bgTexture, 0, 0, &rect);

	return true;
}

bool SceneDie::CleanUp() {
	
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	dieAnim.Reset();
	this->active = false;

	return ret;
}