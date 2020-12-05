#include "SceneDie.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "SceneIntro.h"
#include "SceneManager.h"
#include "Fonts.h"
#include "Fonts.h"

#include "Input.h"
#include "Audio.h"

#include "FadeToBlack.h"

#include "SDL/include/SDL_scancode.h"


SceneDie::SceneDie() : Scenes()
{
	dieAnim.PushBack({ 0, 0, 1280, 720 });
	dieAnim.PushBack({ 1280, 0, 1280, 720 });
	dieAnim.PushBack({ 2560, 0, 1280, 720 });
	dieAnim.PushBack({ 3840, 0, 1280, 720 });
	dieAnim.PushBack({ 5120, 0, 1280, 720 });
	dieAnim.PushBack({ 6400, 0, 1280, 720 });

	dieAnim.loop = false;
}

SceneDie::~SceneDie()
{

}

// Load assets
bool SceneDie::Start()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_dead.png");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/you_lose.ogg");
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	redFont = app->fonts->Load("Assets/Textures/Characters/red_font.png", lookupTable, 5);
	starTex = app->tex->Load("Assets/Textures/Characters/star.png");

	time = 0;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool SceneDie::Update(float dt)
{
	time++;

	if (time == 1)
	{
		app->audio->PlayFx(dieFx);
		dieAnim.speed = 0.05f * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
		app->fade->Fade(this, (Scenes*)app->sceneManager->intro, 1/dt);

	dieAnim.Update();

	return true;
}

// Update: draw background
bool SceneDie::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = dieAnim.GetCurrentFrame();
	app->render->DrawTexture(bgTexture, 0, 0, &rect);

	app->fonts->BlitText(300,100,redFont, "SCORE:");
	app->fonts->BlitText(550, 100, redFont, std::to_string(app->player->getScore()).c_str());
	app->fonts->BlitText(700, 100, redFont, "x");
	app->fonts->BlitText(750, 100, redFont, std::to_string(app->player->getStars()).c_str());
	app->fonts->BlitText(870, 100, redFont, "=");
	app->fonts->BlitText(925, 100, redFont, std::to_string(app->player->getfinalScore()).c_str());

	app->render->DrawTexture(starTex, 810,95, NULL);

	return true;
}

bool SceneDie::CleanUp() {
	
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	dieAnim.Reset();
	
	this->active = false;

	return ret;
}