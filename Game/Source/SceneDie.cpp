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
	dieAnim.PushBack({ 0, 0, 1280, 720 });
	dieAnim.PushBack({ 1280, 0, 1280, 720 });
	dieAnim.PushBack({ 2560, 0, 1280, 720 });
	dieAnim.PushBack({ 3840, 0, 1280, 720 });
	dieAnim.PushBack({ 5120, 0, 1280, 720 });
	dieAnim.PushBack({ 6400, 0, 1280, 720 });

	dieAnim.speed = 0.1f;
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
		bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_dead.png");
		dieFx = app->audio->LoadFx("Assets/Audio/Fx/you_lose.ogg");
		time = 0;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
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
		app->fade->Fade(this, (Module*)app->sceneIntro, 1/dt);

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