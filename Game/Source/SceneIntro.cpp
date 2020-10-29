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

	//bgTexture = App->textures->Load("Assets/sprites/menu/menuinicial_1.png");
	//App->audio->PlayMusic("Assets/music/Build/music/opening.ogg", 1.0f);

	

	return ret;
}

bool SceneIntro::Update()
{

	if (app->input->GetKey(SDLK_KP_ENTER) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 90);
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		exit(0);
	}
	

	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	/*App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(bgTexture, 0, 0, &(menuAnim->GetCurrentFrame()));*/

	return true;
}

bool SceneIntro::CleanUp() {
	bool ret = true;

	//app->tex->UnLoad(bgTexture);
	//app->audio->CleanUp();
	return true;
}