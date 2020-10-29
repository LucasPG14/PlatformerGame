#include"App.h"
#include"Player.h"
#include"Render.h"
#include"Textures.h"
#include"Module.h"
#include"Input.h"
#include"Scene.h"
#include"Animation.h"
#include"Audio.h"
#include "Map.h"
#include"SDL/include/SDL_scancode.h"

Player::Player() : Module()
{
	//ANIMATION WHEN PLAYER IS STATIC
	rightIdleAnim.PushBack({ 11, 18, 59, 92 });

	rightIdleAnim.speed = 0.01f;

	leftIdleAnim.PushBack({ 213, 18, 59, 92 });

	leftIdleAnim.speed = 0.01f;

	//ANIMATION WHEN PLAYER IS RUNNING
	rightRunAnim.PushBack({ 216, 122, 71, 93 });
	rightRunAnim.PushBack({ 303, 121, 54, 94 });

	rightRunAnim.speed = 0.01f;
	rightRunAnim.loop = true;

	leftRunAnim.PushBack({ 219, 234, 71, 93 });
	leftRunAnim.PushBack({ 301, 233, 54, 94 });

	leftRunAnim.speed = 0.01f;
	leftRunAnim.loop = true;

	//ANIMATION WHEN PLAYER IS JUMPING 
	rightJumpAnim.PushBack({ 9, 118, 71, 96 });

	rightJumpAnim.speed = 1.0f;
	rightJumpAnim.loop = false;

	leftJumpAnim.PushBack({ 95, 118, 71, 96 });

	leftJumpAnim.speed = 1.0f;
	leftJumpAnim.loop = false;

	//ANIMATION WHEN PLAYER DIES

}
bool Player::Start()
{

	player = app->tex->Load("Assets/textures/Characters/Adventurer/adventurer_tilesheet2.png");
	//SET POSITION
	position.x = 15;
	position.y = 608;
	currentAnimation = &rightIdleAnim;

	return true;
}
bool Player::Update(float dt)
{
	//INPUT TO MOVE THE PLAYER
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if (currentAnimation == &leftJumpAnim)
		{
			rightJumpAnim.Reset();
			currentAnimation = &rightJumpAnim;
			lastAnimation = currentAnimation;
		}

		else if (currentAnimation != &rightRunAnim && (lastAnimation != &rightJumpAnim || Collision() == 1))
		{
			rightRunAnim.Reset();
			currentAnimation = &rightRunAnim;
			lastAnimation = currentAnimation;
		}

		if (Collision() != 3)
		{
			position.x += 1;
		}
		else
		{
			app->render->camera.x += 1;
		}
		app->render->camera.x -= 1;
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (currentAnimation == &rightJumpAnim)
		{
			leftJumpAnim.Reset();
			currentAnimation = &leftJumpAnim;
			lastAnimation = currentAnimation;
		}

		else if (currentAnimation != &leftRunAnim && (lastAnimation != &leftJumpAnim || Collision() == 1))
		{
			leftRunAnim.Reset();
			currentAnimation = &leftRunAnim;
			lastAnimation = currentAnimation;
		}

		if (Collision() != 4)
		{
			position.x -= 1;
		}
		else
		{
			app->render->camera.x -= 1;
		}
		app->render->camera.x += 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
	{
		position.y -= 2;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
	{
			position.y += 2;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && Collision() == 1)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation != &rightJumpAnim)
			{
				rightJumpAnim.Reset();
				currentAnimation = &rightJumpAnim;
				lastAnimation = currentAnimation;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation != &leftJumpAnim)
			{
				leftJumpAnim.Reset();
				currentAnimation = &leftJumpAnim;
				lastAnimation = currentAnimation;
			}
		}
		if (currentAnimation == &rightIdleAnim)
		{
			rightJumpAnim.Reset();
			currentAnimation = &rightJumpAnim;
			lastAnimation = currentAnimation;
		}
		if (currentAnimation == &leftIdleAnim)
		{
			leftJumpAnim.Reset();
			currentAnimation = &leftJumpAnim;
			lastAnimation = currentAnimation;
		}

		jump = true;
		speedY = 2.0f;
	}


	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE)
	{
		if (currentAnimation == &rightJumpAnim || currentAnimation == &rightRunAnim)
		{
			rightIdleAnim.Reset();
			currentAnimation = &rightIdleAnim;
			lastAnimation = currentAnimation;
		}
		if (currentAnimation == &leftJumpAnim || currentAnimation == &leftRunAnim)
		{
			leftIdleAnim.Reset();
			currentAnimation = &leftIdleAnim;
			lastAnimation = currentAnimation;
		}
	}

	//DEBUG KEYS

	//RESTART GAME (f1 && f3)

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->player->CleanUp();
		app->scene->CleanUp();
		app->player->Start();
		app->scene->Start();
	}

	//SAVE GAME (f5)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}

	//LOAD GAME (f6)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}

	//VIEW COLLIDERS (f9)


	//GODMODE (f10)

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

int Player::Collision()
{
	int ret = 0;

	iPoint playerTileDown;
	iPoint playerTileUp;
	iPoint playerTileRight;
	iPoint playerTileLeft;

	playerTileDown = app->map->WorldToMap(position.x + 29, position.y + 93);
	playerTileUp = app->map->WorldToMap(position.x + 29, position.y - 1);
	playerTileRight = app->map->WorldToMap(position.x + 59, position.y + 46);
	playerTileLeft = app->map->WorldToMap(position.x - 1, position.y + 46);

	ListItem<MapLayer*>* lay = app->map->data.layers.start;

	int idDown;
	int idUp;
	int idRight;
	int idLeft;

	while (lay != NULL)
	{
		if (lay->data->properties.GetProperty("Collision") == 1)
		{
			idDown = lay->data->Get(playerTileDown.x, playerTileDown.y);
			idUp = lay->data->Get(playerTileUp.x, playerTileUp.y);
			idRight = lay->data->Get(playerTileRight.x, playerTileRight.y);
			idLeft = lay->data->Get(playerTileLeft.x, playerTileLeft.y);

			//Tile Down
			if (idDown == 50 || idDown == 51)
			{
				return 1;
			}
			//Tile Up
			if (idUp == 50 || idUp == 51)
			{
				return 2;
			}
			//Tile Right
			if (idRight == 50 || idRight == 51)
			{
				return 3;
			}
			//Tile Left
			if (idLeft == 50 || idLeft == 51)
			{
				return 4;
			}
		}
		lay = lay->next;
	}

	return ret;
}

void Player::Gravity()
{
	if(Collision() != 1)
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