#include "SceneWin.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "SceneIntro.h"
#include "SceneManager.h"
#include "Fonts.h"
#include "Player.h"

#include "Input.h"
#include "Audio.h"

#include "FadeToBlack.h"

#include "SDL/include/SDL_scancode.h"


SceneWin::SceneWin() : Scenes()
{
	for (int i = 0; i < 5; i++)
	{
		winAnim.PushBack({ 1280 * i, 0, 1280, 720 });
	}

	winAnim.loop = true;
}

SceneWin::~SceneWin()
{

}

// Load assets
bool SceneWin::Start()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_win.png");
	winFx = app->audio->LoadFx("Assets/Audio/Fx/you_win.ogg");
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = app->fonts->Load("Assets/Textures/Characters/yellow_font.png", lookupTable, 5);
	starTex = app->tex->Load("Assets/Textures/Characters/star.png");
	time = 0;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool SceneWin::Update(float dt)
{
	time++;

	if (time == 1)
	{
		app->audio->PlayFx(winFx);
		winAnim.speed = 0.05f * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
		app->fade->Fade(this, app->sceneManager->intro, 1/dt);

	winAnim.Update();

	return true;
}

// PostUpdate: draw background
bool SceneWin::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, &winAnim.GetCurrentFrame());

	app->fonts->BlitText(300, 100, yellowFont, "SCORE:");
	app->fonts->BlitText(550, 100, yellowFont, std::to_string(app->player->GetScore()).c_str());
	app->fonts->BlitText(700, 100, yellowFont, "x");
	app->fonts->BlitText(750, 100, yellowFont, std::to_string(app->player->GetStars()).c_str());
	app->fonts->BlitText(870, 100, yellowFont, "=");

	if ((app->player->stars) == 0) {
		app->fonts->BlitText(925, 100, yellowFont, std::to_string(app->player->GetScore()).c_str());
	}
	else {
		app->fonts->BlitText(925, 100, yellowFont, std::to_string(app->player->GetFinalScore()).c_str());
	}

	app->render->DrawTexture(starTex, 810, 95, NULL);
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