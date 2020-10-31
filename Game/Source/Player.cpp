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

	// ANIMATION WHEN PLAYER IS STATIC
	rightIdleAnim.PushBack({ 11, 18, 59, 92 });

	rightIdleAnim.speed = 0.01f;

	leftIdleAnim.PushBack({ 213, 18, 59, 92 });

	leftIdleAnim.speed = 0.01f;

	// ANIMATION WHEN PLAYER IS RUNNING
	rightRunAnim.PushBack({ 216, 122, 71, 93 });
	rightRunAnim.PushBack({ 303, 121, 54, 94 });

	rightRunAnim.speed = 0.05f;
	rightRunAnim.loop = true;

	leftRunAnim.PushBack({ 219, 234, 71, 93 });
	leftRunAnim.PushBack({ 301, 233, 54, 94 });

	leftRunAnim.speed = 0.05f;
	leftRunAnim.loop = true;

	// ANIMATION WHEN PLAYER IS JUMPING 
	rightJumpAnim.PushBack({ 9, 118, 71, 96 });

	rightJumpAnim.speed = 1.0f;
	rightJumpAnim.loop = false;

	leftJumpAnim.PushBack({ 95, 118, 71, 96 });

	leftJumpAnim.speed = 1.0f;
	leftJumpAnim.loop = false;

	// ANIMATION WHEN PLAYER DIES
	deadAnim.PushBack({ 449, 27, 75,92 });
	deadAnim.PushBack({ 447,127,75,92 });
	deadAnim.PushBack({ 447,224,79,95 });

	deadAnim.speed = 0.05f;
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
	speedX = config.child("speed").attribute("x").as_float();
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
		// SET POSITION
		resetPlayer();
		currentAnimation = &rightIdleAnim;
	}

	return true;
}
bool Player::Update(float dt)
{
	if (deadPlayer == false)
	{
		if (lastAnimation == &deadAnim) currentAnimation = &rightIdleAnim;

		// INPUT TO MOVE THE PLAYER
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation == &leftJumpAnim)
			{
				rightJumpAnim.Reset();
				currentAnimation = &rightJumpAnim;
				lastAnimation = currentAnimation;
			}

			if (position.x >= (app->map->data.width * app->map->data.tileWidth) - 90)
			{
				position.x = (app->map->data.width * app->map->data.tileWidth) - 90;
			}

			if (currentAnimation != &rightRunAnim && (lastAnimation != &rightJumpAnim || Collision("bottom") == true))
			{
				rightRunAnim.Reset();
				currentAnimation = &rightRunAnim;
				lastAnimation = currentAnimation;
			}

			if (Collision("right") == false)
			{
				if (position.x >= app->render->camera.w / 2)
				{
					app->render->camera.x -= 2;
				}
				position.x += 2;
			}
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
				if (position.x > app->render->camera.w / 2)
				{
					app->render->camera.x += 2;
				}
				position.x -= 2;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT && godMode == true)
		{
			position.y -= 2;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && godMode == true)
		{
			position.y += 2;
		}

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
			speedY = 7.0f;
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

		// DEBUG KEYS

		// RESTART GAME (F1 && F3)

		if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F3) == KeyState::KEY_DOWN)
		{
			app->player->CleanUp();
			app->player->Start();
			app->render->cameraStartPosition();
		}

		// SAVE GAME (F5)
		if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		{
			app->SaveGameRequest();
		}

		// LOAD GAME (F6)
		if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		{
			app->LoadGameRequest();
		}

		// VIEW COLLIDERS (F9)


		// GODMODE (F10)
		if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
		{
			godMode = !godMode;
		}


		if (jump == true)
		{
			Jump();
		}

		if (Collision("top") == true)
		{
			speedY = 0.0f;
		}

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
						tilePos = app->map->WorldToMap(position.x + (10 + (14 * i)), position.y + 94);
						idTile = lay->data->Get(tilePos.x, tilePos.y);
						if (checkCollisionType(idTile,"bottom"))
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
						if (checkCollisionType(idTile,"top"))
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
						if (checkCollisionType(idTile,"right"))
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
						if (checkCollisionType(idTile,"left"))
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
		if (speedY < -3.0f)
		{
			speedY = -1.0f;
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
		app->sceneDie->active = true;
		app->scene->active = false;
		active = false;
		app->render->cameraStartPosition();
	}
}

void Player::changeLevel(int level)
{
	switch (level)
	{
	case 1:
		app->scene->active = false;
		app->sceneWin->active = true;
		active = false;
		app->render->cameraStartPosition();

		break;
	}


}


int x = 0;
bool touched = false;
bool top = false;
bool bottom = false;
bool canpass = false;
bool z = false;
bool Player::checkCollisionType(int idTile, std::string direction)
{
	switch (idTile)
	{
	case 50:
		x = 0;
		touched = false;
		z = false;
		return true;
		break;


	case 51:
		Dead();
		return true;
		break;

	case 52:
		if (direction == "bottom" && z == false)
			return true;

		if (direction == "bottom" && jumping) {
 			x++;
			if (x > 3 && x < 62)
				jumping = true;
			if (x >= 62)
				jumping = false;
			if (x == 0)
				jumping = false;


		}
		if (direction == "top") {

			z = true;
			return false;


		}
		else if (direction == "bottom" && jumping && !touched) {

			return false;

		}

		else if (direction == "bottom" && !jumping) {

			touched = true;
			x = 0;
			return true;
		}
		else {
			return true;
		}
		break;
	case 53:
		changeLevel(1);
		break;
	}


	return false;
}

void Player::resetPlayer()
{
	position.x = 200;
	position.y = 607;
	active = true;
}
