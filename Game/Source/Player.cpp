#include"App.h"
#include"Player.h"
#include"Render.h"
#include"Textures.h"
#include"Module.h"
#include"Input.h"
#include"Animation.h"
#include"Audio.h"
#include "Map.h"
#include"SDL/include/SDL_scancode.h"

Player::Player() : Module()
{
	//ANIMATION WHEN PLAYER IS STATIC
	idle.PushBack({ 411, 238, 58, 92 });


	idle.speed = 0.0007f;

	//ANIMATION WHEN PLAYER IS RUNNING
	run.PushBack({ 9, 18, 63, 92 });
	run.PushBack({ 3, 127, 72, 93 });

	run.speed = 0.01f;
	run.loop = true;

	//ANIMATION WHEN PLAYER IS JUMPING 
	jumpAnim.PushBack({ 10, 18, 63, 92 });
	jumpAnim.PushBack({ 84, 13, 71, 97 });
	jumpAnim.PushBack({ 321, 12, 79, 95 });
	jumpAnim.PushBack({ 162, 15, 78, 93 });
	jumpAnim.PushBack({ 248, 30, 66, 80 });

	jumpAnim.speed = 1.0f;
	jumpAnim.loop = false;

	//ANIMATION WHEN PLAYER DIES

}
bool Player::Start()
{

	player = app->tex->Load("Assets/textures/Characters/Adventurer/adventurer_tilesheet.png");
	//SET POSITION
	position.x = 15;
	position.y = 608;
	currentAnimation = &idle;

	return true;
}
bool Player::Update(float dt)
{
	//INPUT TO MOVE THE PLAYER
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if (currentAnimation != &run)
		{
			run.Reset();
			currentAnimation = &run;

		}
		if (CollisionHorizontal() == 0 || CollisionHorizontal() == 2)
		{
			position.x += 1;
		}
		app->render->camera.x -= 1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (currentAnimation != &run)
		{
			run.Reset();
			currentAnimation = &run;
		}

		if (CollisionHorizontal() == 0 || CollisionHorizontal() == 1)
		{
			position.x -= 1;
		}
		app->render->camera.x += 1;
	}
	//else if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
	//{
	//	if (CollisionVertical() == 0 || CollisionVertical() == 1)
	//	{
	//		position.y -= 2;
	//	}
	//}
	//else if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
	//{
	//	if (CollisionVertical() == 0 || CollisionVertical() == 2)
	//	{
	//		position.y += 2;
	//	}
	//}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && CollisionVertical() == 1)
	{
		if (currentAnimation != &jumpAnim) 
		{
			jumpAnim.Reset();
			currentAnimation = &jumpAnim;
		}
		jump = true;
		speedY = 2.0f;
	}


	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE)
	{
		if (currentAnimation != &idle)
		{
			idle.Reset();
			currentAnimation = &idle;
		}
	}

	if ((app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT ||
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (currentAnimation != &jumpAnim)
		{
			jumpAnim.Reset();
			currentAnimation = &jumpAnim;
		}
		jump = true;
		speedY = 2.0f;
	}

	currentAnimation->Update();
	

	if (jump == true)
	{
		Jump();
	}

	Gravity();

	return true;
}
bool Player::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(player, position.x, position.y, &rect);

	return true;
}
bool Player::CleanUp() {

	//Unload textures
	app->tex->UnLoad(player);

	return true;
}

int Player::CollisionVertical()
{
	int ret = 0;

	iPoint mapPos1;
	iPoint mapPos2;

	mapPos1 = app->map->WorldToMap(position.x + 29, position.y + 93);
	mapPos2 = app->map->WorldToMap(position.x + 29, position.y - 1);

	ListItem<MapLayer*>* lay = app->map->data.layers.start;

	int id1;
	int id2;

	while (lay != NULL)
	{
		if (lay->data->properties.GetProperty("Collision") == 1)
		{
			id1 = lay->data->Get(mapPos1.x, mapPos1.y);
			id2 = lay->data->Get(mapPos2.x, mapPos2.y);

			if (id1 == 50 || id1 == 51)
			{
				return 1;
			}
			if (id2 == 50 || id2 == 51)
			{
				return 2;
			}
		}
		lay = lay->next;
	}

	return ret;
}

int Player::CollisionHorizontal()
{
	int ret = 0;

	iPoint mapPos1;
	iPoint mapPos2;

	mapPos1 = app->map->WorldToMap(position.x + 59, position.y + 46);
	mapPos2 = app->map->WorldToMap(position.x - 1, position.y + 46);

	ListItem<MapLayer*>* lay = app->map->data.layers.start;

	int id1;
	int id2;

	while (lay != NULL)
	{
		if (lay->data->properties.GetProperty("Collision") == 1)
		{
			id1 = lay->data->Get(mapPos1.x, mapPos1.y);
			id2 = lay->data->Get(mapPos2.x, mapPos2.y);

			if (id1 == 50 || id1 == 51)
			{
				return 1;
			}
			if (id2 == 50 || id2 == 51)
			{
				return 2;
			}
		}
		lay = lay->next;
	}

	return ret;
}

void Player::Gravity()
{
	if(CollisionVertical() != 1)
	{
		speedY -= gravity;
		position.y -= speedY;
		if (speedY < -1.5f)
		{
			speedY = -1.5f;
		}
	}
}

void Player::Jump()
{
	speedY -= gravity;
	position.y -= speedY;
	jump = false;
}

