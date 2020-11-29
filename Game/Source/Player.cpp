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
	rightIdleAnim.PushBack({ 0, 0, 51, 87 });
	rightIdleAnim.PushBack({ 192, 0, 51, 87 });
	rightIdleAnim.PushBack({ 384, 0, 51, 87 });
	rightIdleAnim.PushBack({ 576, 0, 51, 87 });
	rightIdleAnim.PushBack({ 768, 0, 51, 87 });

	rightIdleAnim.speed = 0.1f;
	rightIdleAnim.loop = true;

	leftIdleAnim.PushBack({ 0, 87, 51, 87 });
	leftIdleAnim.PushBack({ 192, 87, 51, 87 });
	leftIdleAnim.PushBack({ 384, 87, 51, 87 });
	leftIdleAnim.PushBack({ 576, 87, 51, 87 });
	leftIdleAnim.PushBack({ 768, 87, 51, 87 });

	leftIdleAnim.speed = 0.1f;
	leftIdleAnim.loop = true;

	// Animation when player is running
	rightRunAnim.PushBack({ 0, 174, 45, 87 });
	rightRunAnim.PushBack({ 192, 174, 42, 87 });
	rightRunAnim.PushBack({ 384, 174, 45, 87 });
	rightRunAnim.PushBack({ 573, 174, 51, 87 });
	rightRunAnim.PushBack({ 762, 174, 57, 87 });
	rightRunAnim.PushBack({ 951, 174, 63, 87 });
	rightRunAnim.PushBack({ 1146, 174, 57, 87 });
	rightRunAnim.PushBack({ 1341, 174, 51, 87 });

	rightRunAnim.speed = 0.2f;
	rightRunAnim.loop = true;

	leftRunAnim.PushBack({ 1347, 261, 45, 87 });
	leftRunAnim.PushBack({ 1158, 261, 42, 87 });
	leftRunAnim.PushBack({ 963, 261, 45, 87 });
	leftRunAnim.PushBack({ 768, 261, 51, 87 });
	leftRunAnim.PushBack({ 573, 261, 57, 87 });
	leftRunAnim.PushBack({ 378, 261, 63, 87 });
	leftRunAnim.PushBack({ 189, 261, 57, 87 });
	leftRunAnim.PushBack({ 0, 261, 51, 87 });

	leftRunAnim.speed = 0.2f;
	leftRunAnim.loop = true;

	// Animation when player is jumping
	rightJumpAnim.PushBack({ 0, 348, 51, 87 });
	rightJumpAnim.PushBack({ 189, 348, 51, 87 });
	rightJumpAnim.PushBack({ 378, 348, 51, 87 });

	rightJumpAnim.speed = 0.3f;
	rightJumpAnim.loop = false;

	leftJumpAnim.PushBack({ 1341, 348, 51, 87 });
	leftJumpAnim.PushBack({ 1152, 348, 51, 87 });
	leftJumpAnim.PushBack({ 963, 348, 51, 87 });

	leftJumpAnim.speed = 0.3f;
	leftJumpAnim.loop = false;

	// Animation when player dies
	rightDeadAnim.PushBack({ 0, 783, 51, 87 });
	rightDeadAnim.PushBack({ 159, 783, 93, 87 });
	rightDeadAnim.PushBack({ 363, 783, 117, 87 });
	rightDeadAnim.PushBack({ 552, 783, 120, 87 });
	rightDeadAnim.PushBack({ 750, 783, 114, 87 });
	rightDeadAnim.PushBack({ 939, 783, 117, 87 });
	rightDeadAnim.PushBack({ 1131, 783, 117, 87 });

	rightDeadAnim.speed = 0.1f;

	leftDeadAnim.PushBack({ 1197, 870, 51, 87 });
	leftDeadAnim.PushBack({ 996, 870, 93, 87 });
	leftDeadAnim.PushBack({ 768, 870, 117, 87 });
	leftDeadAnim.PushBack({ 576, 870, 120, 87 });
	leftDeadAnim.PushBack({ 384, 870, 114, 87 });
	leftDeadAnim.PushBack({ 192, 870, 117, 87 });
	leftDeadAnim.PushBack({ 0, 870, 117, 87 });

	leftDeadAnim.speed = 0.1f;
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
		if (lastAnimation == &rightDeadAnim) currentAnimation = &rightIdleAnim;

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
						tilePos = app->map->WorldToMap(position.x + (9 + (10 * i)), position.y + 87);
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
						tilePos = app->map->WorldToMap(position.x + (9 + (10 * i)), position.y + 15);
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
						tilePos = app->map->WorldToMap(position.x + 39, position.y + (16 + (16 * i)));
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
						tilePos = app->map->WorldToMap(position.x + 8, position.y + (16 + (16 * i)));
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
	currentAnimation = &rightDeadAnim;
	lastAnimation = currentAnimation;
	deadPlayer = true;
	if (time == 60)
	{
		time = 0;
		deadPlayer = false;
		app->fade->Fade(app->scene, app->sceneDie, 60);
	}
}

void Player::ChangeLevel(int level)
{
	switch (level)
	{
	case 1:
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