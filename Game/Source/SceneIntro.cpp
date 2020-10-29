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

	//App->render->camera.x = 0;
	//App->render->camera.y = 0;

	

	return ret;
}

bool SceneIntro::Update()
{

	//GamePad& pad = App->input->pads[0];

	//if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN || pad.a == KEY_STATE::KEY_DOWN)
	//{

	//	App->transition->FadeToBlack(this, (Module*)App->shop, 60.0f);
	//}

	//if (App->input->keys[SDL_SCANCODE_F6] == KEY_STATE::KEY_DOWN) {
	//	App->transition->FadeToBlack(this, (Module*)App->sceneWin, 90);

	//}

	//if (App->input->keys[SDL_SCANCODE_F7] == KEY_STATE::KEY_DOWN) {
	//	//App->audio->StopMusic();
	//	App->transition->FadeToBlack(this, (Module*)App->loose);
	//}

	//if (App->input->keys[SDL_SCANCODE_F8] == KEY_STATE::KEY_DOWN) {
	//	//App->audio->StopMusic();
	//	App->transition->FadeToBlack(this, (Module*)App->shop);
	//}


	//if (App->input->keys[SDL_SCANCODE_F5] == KEY_STATE::KEY_DOWN) {
	//	App->transition->FadeToBlack(this, (Module*)App->initialScreen, 90);

	//}

	//if (App->input->keys[SDL_SCANCODE_F10] == KEY_STATE::KEY_DOWN) {
	//	App->transition->FadeToBlack(this, (Module*)App->scene, 90);
	//}

	//if (App->input->keys[SDL_SCANCODE_F11] == KEY_STATE::KEY_DOWN) {
	//	App->transition->FadeToBlack(this, (Module*)App->scene2, 90);
	//}


	//if (menuAnim != nullptr)
	//{
	//	menuAnim->Update();
	//}

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

	/*if (!App->textures->Unload(bgTexture)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	App->textures->Unload(bgTexture);
	App->audio->StopMusic();*/
	return true;
}