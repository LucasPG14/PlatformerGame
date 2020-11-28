#include "App.h"
#include "Player.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Module.h"
#include "Input.h"
#include "Scene.h"
#include "Animation.h"
#include "SceneDie.h"
#include "Audio.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Log.h"
#include "SceneWin.h"

Player::Player() : Module()
{
	name.Create("player");

	// Animation when player is static
	rightIdleAnim.PushBack({ 24, 26, 17, 22 });
	rightIdleAnim.PushBack({ 88,27,17,21 });
	rightIdleAnim.PushBack({ 152,28,17,20 });
	rightIdleAnim.PushBack({ 216,27,17,21 });
	rightIdleAnim.PushBack({ 280,26,17,22 });

	rightIdleAnim.speed = 0.1f;
	rightIdleAnim.loop = true;

	leftIdleAnim.PushBack({ 989, 26, 17, 22 });
	leftIdleAnim.PushBack({ 925, 27, 17, 21 });
	leftIdleAnim.PushBack({ 861, 28, 17, 20 });
	leftIdleAnim.PushBack({ 797, 27, 17, 21 });
	leftIdleAnim.PushBack({ 733, 26, 17, 22 });

	leftIdleAnim.speed = 0.1f;
	leftIdleAnim.loop = true;

	// Animation when player is running
	rightRunAnim.PushBack({ 25, 89, 15, 22 });
	rightRunAnim.PushBack({ 89, 88, 14, 23 });
	rightRunAnim.PushBack({ 153, 89, 15, 23 });
	rightRunAnim.PushBack({ 216, 90, 17, 22 });
	rightRunAnim.PushBack({ 279, 89, 19, 21 });
	rightRunAnim.PushBack({ 342, 88, 21, 22 });
	rightRunAnim.PushBack({ 407, 89, 19, 23 });
	rightRunAnim.PushBack({ 472, 90, 17, 22 });

	rightRunAnim.speed = 0.1f;
	rightRunAnim.loop = true;

	leftRunAnim.PushBack({ 990, 89, 15, 22 });
	leftRunAnim.PushBack({ 927, 88, 14, 23 });
	leftRunAnim.PushBack({ 862, 89, 15, 23 });
	leftRunAnim.PushBack({ 797, 90, 17, 22 });
	leftRunAnim.PushBack({ 732, 89, 19, 21 });
	leftRunAnim.PushBack({ 667, 88, 21, 22 });
	leftRunAnim.PushBack({ 604, 89, 19, 23 });
	leftRunAnim.PushBack({ 541, 90, 17, 22 });

	leftRunAnim.speed = 0.1f;
	leftRunAnim.loop = true;

	// Animation when player is jumping
	rightJumpAnim.PushBack({ 25, 154, 17, 22 });
	rightJumpAnim.PushBack({ 88, 154, 17, 22 });
	rightJumpAnim.PushBack({ 151, 154, 17, 22 });

	rightJumpAnim.speed = 0.1f;
	rightJumpAnim.loop = false;

	leftJumpAnim.PushBack({ 988, 154, 17, 22 });
	leftJumpAnim.PushBack({ 925, 154, 17, 22 });
	leftJumpAnim.PushBack({ 862, 154, 17, 22 });

	leftJumpAnim.speed = 0.1f;
	leftJumpAnim.loop = false;

	// Animation when player dies
	deadAnim.PushBack({ 22, 405, 17, 27 });
	deadAnim.PushBack({ 75, 405, 31, 27 });
	deadAnim.PushBack({ 143, 405, 39, 27 });
	deadAnim.PushBack({ 206, 405, 40, 27 });
	deadAnim.PushBack({ 272, 405, 38, 27 });
	deadAnim.PushBack({ 335, 405, 39, 27 });
	deadAnim.PushBack({ 399, 405, 39, 27 });

	deadAnim.speed = 0.1f;
}
bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	playerString.Create(config.child("image").attribute("source").as_string(""));

	godMode = config.child("godmode").attribute("value").as_bool();
	gravity = config.child("gravity").attribute("value").as_float();
	jump = config.child("jump").attribute("value").as_bool();
	speedX = config.child("speed").attribute("x").as_int();
	speedY = config.child("speed").attribute("y").as_float();
	time = 0;
	deadPlayer = false;

	return true;
}
bool Player::Start()
{
	if (this->active == true)
	{
		SString tmp("%s%s", folder.GetString(), playerString.GetString());
		player = app->tex->Load(tmp.GetString());
		// Set position
		ResetPlayer();
		currentAnimation = &rightIdleAnim;
		stepSnow = app->audio->LoadFx("Assets/Audio/Fx/step_snow.wav");

		jumping = false;
	}

	return true;
}
bool Player::Update(float dt)
{
	if (deadPlayer == false)
	{
		if (lastAnimation == &deadAnim) currentAnimation = &rightIdleAnim;

		// Input to move the player
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation == &leftJumpAnim)
			{
				rightJumpAnim.Reset();
				currentAnimation = &rightJumpAnim;
				lastAnimation = currentAnimation;
			}

			if (position.x >= (app->map->data.width * app->map->data.tileWidth) - 90) 
				position.x = (app->map->data.width * app->map->data.tileWidth) - 90;


			if (currentAnimation != &rightRunAnim && (lastAnimation != &rightJumpAnim || Collision("bottom") == true))
			{
				rightRunAnim.Reset();
				currentAnimation = &rightRunAnim;
				lastAnimation = currentAnimation;
			}

			if (Collision("right") == false)
			{
				if (position.x >= app->render->camera.w / 2) app->render->camera.x -= speedX;

 				position.x += speedX;
			}

			if (Collision("bottom") == true) app->audio->PlayFx(stepSnow);

		}

		else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation == &rightJumpAnim)
			{
				leftJumpAnim.Reset();
				currentAnimation = &leftJumpAnim;
				lastAnimation = currentAnimation;
			}

			else if (currentAnimation != &leftRunAnim && (lastAnimation != &leftJumpAnim || Collision("bottom") == true))
			{
				leftRunAnim.Reset();
				currentAnimation = &leftRunAnim;
				lastAnimation = currentAnimation;
			}

			if (Collision("left") == false)
			{
				if (position.x > app->render->camera.w / 2) app->render->camera.x += speedX;

				position.x -= speedX;
			}

			if (Collision("bottom") == true) app->audio->PlayFx(stepSnow);

		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT && godMode == true)
			position.y -= speedX;


		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && godMode == true)
			position.y += speedX;

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && Collision("bottom") == true)
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

			jumping = true;
			jump = true;
			speedY = 12.0f;
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

		// Restart game
		if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN ||
			app->input->GetKey(SDL_SCANCODE_F3) == KeyState::KEY_DOWN)
		{
			app->player->CleanUp();
			app->player->Start();
			app->render->cameraStartPosition();
		}

		// Save game
		if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
			app->SaveGameRequest();

		// Load game
		if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN) 
			app->LoadGameRequest();

		// God mode
		if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN) 
			godMode = !godMode;

		if (jump == true) Jump();


		if (Collision("top") == true) speedY = 0.0f;


		if (speedY <= 0.0f) jumping = false;

		Gravity();
	}
	else
	{
		time++;
		Dead();
	}

	currentAnimation->Update();

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
	ResetPlayer();
	this->active = false;
	return true;
}

bool Player::Collision(const char* side)
{
	bool ret = false;

	if (godMode == false)
	{

		ListItem<MapLayer*>* lay = app->map->data.layers.start;

		iPoint tilePos;

		int idTile;


		while (lay != NULL)
		{
			if (lay->data->properties.GetProperty("Collision") == 1)
			{
				if (side == "bottom")
				{
					for (uint i = 0; i < 3; i++)
					{
						tilePos = app->map->WorldToMap(position.x + (10 + (14 * i)), position.y + 96);
						idTile = lay->data->Get(tilePos.x, tilePos.y);
						if (CheckCollisionType(idTile,"bottom"))
						{
							return true;
						}
					}
				}
				else if (side == "top")
				{
					for (uint i = 0; i < 3; i++)
					{
						tilePos = app->map->WorldToMap(position.x + (10 + (14 * i)), position.y + 3);
						idTile = lay->data->Get(tilePos.x, tilePos.y);
						if (CheckCollisionType(idTile,"top"))
						{
							return true;
						}
					}
				}
				else if (side == "right")
				{
					for (uint i = 0; i < 3; i++)
					{
						tilePos = app->map->WorldToMap(position.x + 51, position.y + (45 * i));
						idTile = lay->data->Get(tilePos.x, tilePos.y);
						if (CheckCollisionType(idTile,"right"))
						{
							return true;
						}
					}
				}
				else if (side == "left")
				{
					for (uint i = 0; i < 3; i++)
					{
						tilePos = app->map->WorldToMap(position.x + 9, position.y + (45 * i));
						idTile = lay->data->Get(tilePos.x, tilePos.y);
						if (CheckCollisionType(idTile,"left"))
						{
							return true;
						}
					}
				}
			}
			lay = lay->next;
		}
	}

	return ret;
}

void Player::Gravity()
{
	if(Collision("bottom") == false && godMode == false)
	{
		speedY -= gravity;
		position.y -= speedY;
		if (speedY < -5.0f)
		{
			speedY = -5.0f;
		}
	}
}

void Player::Jump()
{
	speedY -= gravity;
	position.y -= speedY;
	jump = false;
}

bool Player::LoadState(pugi::xml_node& load)
{
	bool ret = true;

	position.x = load.child("position").attribute("x").as_float();
	position.y = load.child("position").attribute("y").as_float();

	return ret;
}

bool Player::SaveState(pugi::xml_node& save) const
{
	bool ret = true;

	pugi::xml_node pos = save.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return ret;
}

void Player::Dead() 
{
	currentAnimation = &deadAnim;
	lastAnimation = currentAnimation;
	deadPlayer = true;
	if (time == 60)
	{
		time = 0;
		deadPlayer = false;
		app->render->cameraStartPosition();
		app->fade->Fade(app->scene, app->sceneDie, 60);
	}
}

void Player::ChangeLevel(int level)
{
	switch (level)
	{
	case 1:
		app->render->cameraStartPosition();
		app->fade->Fade(app->scene, app->sceneWin, 60);
		break;
	}
}


bool Player::CheckCollisionType(int idTile, std::string direction)
{
	switch (idTile)
	{
	case 241:
		return true;
		break;


	case 51:
		Dead();
		return true;
		break;

	case 52:
		if (direction == "bottom" && jumping == true)
		{
			return false;
		}
		else if (direction == "bottom" && jumping == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 53:
		ChangeLevel(1);
		break;
	}

	return false;
}

void Player::ResetPlayer()
{
	position.x = 200;
	position.y = 607;
	active = true;
}