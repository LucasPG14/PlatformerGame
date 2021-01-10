#include "SceneDie.h"
#include "SceneManager.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Fonts.h"

#include "Input.h"
#include "Audio.h"

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
bool SceneDie::Load()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_dead.png");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/you_lose.ogg");
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	redFont = app->fonts->Load("Assets/Hud/red_font.png", lookupTable, 5);
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

bool SceneDie::Update(float dt)
{
	time += dt;

	if (time == dt) app->audio->PlayFx(dieFx);
		
	dieAnim.speed = 5.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) TransitionToScene(SceneType::INTRO);

	dieAnim.Update();

	return true;
}

// Update: draw background
bool SceneDie::Draw()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = dieAnim.GetCurrentFrame();
	app->render->DrawTexture(bgTexture, 0, 0, &rect);

	app->fonts->BlitText(300,75,redFont, "SCORE:");
	app->fonts->BlitText(550, 75, redFont, std::to_string(app->sceneManager->score).c_str());
	app->fonts->BlitText(700, 75, redFont, "x");
	app->fonts->BlitText(750, 75, redFont, std::to_string(app->sceneManager->stars).c_str());
	app->fonts->BlitText(870, 75, redFont, "=");

	app->fonts->BlitText(925, 75, redFont, std::to_string(app->sceneManager->finalScore).c_str());
	
	app->render->DrawTexture(starTex, 810,70, NULL);



	return true;
}

bool SceneDie::Unload() {
	
	bool ret = true;

	app->tex->UnLoad(bgTexture);
	app->tex->UnLoad(starTex);
	app->fonts->UnLoad(redFont);
	dieAnim.Reset();
	
	this->active = false;

	return ret;
}