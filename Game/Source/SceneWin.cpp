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
	winAnim.speed = 0.0006f;
}

SceneWin::~SceneWin()
{

}

// Load assets
bool SceneWin::Start()
{


	bool ret = true;

	bgTexture = app->tex->Load("Assets/textures/Backgrounds/BackgroundWin.png");
	//App->audio->PlayMusic("Assets/music/Build/music/opening.ogg", 1.0f);

	return ret;
}

bool SceneWin::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->player->resetPlayer();
		app->player->Disable();
		app->map->Disable();
		app->fade->Fade(this, (Module*)app->sceneIntro, 1200);

		active = false;
	}

	winAnim.Update();

	return true;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, &winAnim.GetCurrentFrame());

	return true;
}

bool SceneWin::CleanUp() {
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	//app->audio->CleanUp();
	return true;
}