#include"App.h"
#include"Player.h"
#include"Render.h"
#include"Textures.h"
#include"Module.h"
#include"Input.h"
#include"Animation.h"
#include"Audio.h"
#include"SDL/include/SDL_scancode.h"

Player::Player() : Module()
{
	//ANIMATION WHEN PLAYER IS STATIC
	//ANIMATION WHEN PLAYER IS RUNNING
	//ANIMATION WHEN PLAYER IS JUMPING
	//ANIMATION WHEN PLAYER DIES

}
bool Player::Start()
{

	player = app->tex->Load("Assets/textures/Characters/adventurer_tilesheet.png");
	//SET POSITION
	//Position.x = ; Position.y = ;
	currentAnimation = &Static;

	return true;
}
bool Player::Update(float dt)
{
	//INPUT TO MOVE THE PLAYER
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		//currentAnimation = &Run_Right;
		Position.x += 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		//currentAnimation = &Run_Left;
		Position.x -= 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		Position.y -= 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		Position.y += 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		//currentAnimation = &Jump;
		//JumpMove();
	}

	return true;
}
bool Player::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(player, Position.x, Position.y, &rect);
	currentAnimation->Update();

	return true;
}
bool Player::CleanUp() {

	return true;
}

