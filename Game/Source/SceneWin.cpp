#include "SceneWin.h"
#include "SceneManager.h"
#include "Textures.h"
#include "Fonts.h"
#include "Player.h"

#include "Input.h"
#include "Audio.h"


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
bool SceneWin::Load()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_win.png");
	winFx = app->audio->LoadFx("Assets/Audio/Fx/you_win.ogg");
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = app->fonts->Load("Assets/Hud/yellow_font.png", lookupTable, 5);
	starTex = app->tex->Load("Assets/Hud/star.png");
	time = 0;
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->offset.x = 0;
	app->render->offset.y = 0;

	if (app->sceneManager->stars > 0) app->sceneManager->finalScore = app->sceneManager->score * app->sceneManager->stars;
	else app->sceneManager->finalScore = app->sceneManager->score;

	return ret;
}

bool SceneWin::Update(float dt)
{
	time++;

	if (time == 1) app->audio->PlayFx(winFx);
		
	winAnim.speed = 5.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) TransitionToScene(SceneType::INTRO);

	winAnim.Update();

	return true;
}

// PostUpdate: draw background
bool SceneWin::Draw()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, &winAnim.GetCurrentFrame());

	app->fonts->BlitText(300, 75, yellowFont, "SCORE:");
	app->fonts->BlitText(550, 75, yellowFont, std::to_string(app->sceneManager->score).c_str());
	app->fonts->BlitText(700, 75, yellowFont, "x");
	app->fonts->BlitText(750, 75, yellowFont, std::to_string(app->sceneManager->stars).c_str());
	app->fonts->BlitText(870, 75, yellowFont, "=");

	app->fonts->BlitText(925, 75, yellowFont, std::to_string(app->sceneManager->finalScore).c_str());

	app->render->DrawTexture(starTex, 810, 70, NULL);


	return true;
}

bool SceneWin::Unload() 
{
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	app->fonts->UnLoad(yellowFont);
	app->tex->UnLoad(starTex);
	winAnim.Reset();

	return true;
}