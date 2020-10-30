#include "SceneDie.h"
#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"

#include "FadeToBlack.h"



#include "SDL/include/SDL_scancode.h"


SceneDie::SceneDie() : Module()
{

}

SceneDie::~SceneDie()
{

}

// Load assets
bool SceneDie::Start()
{


	bool ret = true;

	bgTexture = app->tex->Load("Assets/Vaqueta.jpg");
	//App->audio->PlayMusic("Assets/music/Build/music/opening.ogg", 1.0f);



	return ret;
}

bool SceneDie::Update()
{

	if (app->input->GetKey(SDLK_KP_ENTER) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 90);
	}

	


	return true;
}

// Update: draw background
bool SceneDie::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	return true;
}

bool SceneDie::CleanUp() {
	bool ret = true;

	//app->tex->UnLoad(bgTexture);
	//app->audio->CleanUp();
	return true;
}