#include "App.h"
#include "Player.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Input.h"
#include "Scene.h"
#include "Animation.h"
#include "Audio.h"
#include "Map.h"
#include "Log.h"
#include "Fonts.h"


Player::Player(iPoint pos) : Entity(pos)
{
	name.Create("player");

	LoadAnimations();

	godMode = false;
	gravity = 1500.0f;
	jump = false;
	speedX = 200.0f;
	speedY = 0.0f;
	time = 0;
	deadPlayer = false;
	levelFinished = false;
	attackCooldown = 0;

	lifes = 3;

	texture = app->tex->Load("Assets/Textures/dark_knight_spritesheet.png");
	currentAnimation = &rightIdleAnim;
	cooldownTex = app->tex->Load("Assets/Hud/cooldown_attack.png");
	checkpointTex = app->tex->Load("Assets/Textures/checkpoint.png");

	jumping = false;
	levelFinished = false;
	checkpoint = false;
	whatCheckpoint = 0;
	deadPlayer = false;
	playerChangePos = false;
	playerCollider = app->colliderManager->AddCollider({ (int)position.x + 12, (int)position.y + 30, 25, 51 }, Collider::PLAYER);
	checkpointAudio = app->audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");
	jumpAudio = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	swordFx = app->audio->LoadFx("Assets/Audio/Fx/sword_swing_soundbible.com_639083727.wav");
	playerHurt = app->audio->LoadFx("Assets/Audio/Fx/impactplate_heavy_004.wav");

	app->render->ResetCam();
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	playerString.Create(config.child("image").attribute("source").as_string(""));

	godMode = config.child("godmode").attribute("value").as_bool();
	jump = config.child("jump").attribute("value").as_bool();
	speedX = config.child("speed").attribute("x").as_int();
	speedY = config.child("speed").attribute("y").as_float();
	levelFinished = config.child("level_finished").attribute("value").as_bool();
	attackCooldown = config.child("attack_cooldown").attribute("value").as_int();

	return true;
}

bool Player::Update(float dt)
{
	if (deadPlayer == false)
	{
		if (lastAnimation == &rightDeadAnim) currentAnimation = &rightIdleAnim;

		// God mode
		if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
		{
			godMode = !godMode;
			if (godMode) speedX *= 3;
			else speedX /= 3;
		}

		SpeedAnimations(dt);

		if (attackCooldown != 0) attackCooldown--;

		// Input to move the player
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation != &rightAttackAnim || rightAttackAnim.HasFinished())
			{
				if (currentAnimation == &leftJumpAnim)
				{
					rightJumpAnim.Reset();
					lastAnimation = currentAnimation;
					currentAnimation = &rightJumpAnim;
				}

				if (position.x >= (app->map->data.width * app->map->data.tileWidth) - 90)
					position.x = (app->map->data.width * app->map->data.tileWidth) - 90;


				if (currentAnimation != &rightRunAnim && (lastAnimation != &rightJumpAnim || Collision("bottom") == true))
				{
					rightRunAnim.Reset();
					lastAnimation = currentAnimation;
 					currentAnimation = &rightRunAnim;
				}
			}
			if (Collision("right") == false)
			{
				position.x += floor(speedX * dt);
				if (position.x >= app->render->offset.x + app->render->camera.w / 2 - 16 && 
					app->render->offset.x < (app->map->data.width * app->map->data.tileWidth) - app->render->camera.w)
				{
						app->render->offset.x += floor(speedX * dt);
						app->render->camera.x -= floor(speedX * dt);
				}
			}
		}

		else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			if (currentAnimation != &leftAttackAnim || leftAttackAnim.HasFinished())
			{
				if (currentAnimation == &rightJumpAnim)
				{
					leftJumpAnim.Reset();
					lastAnimation = currentAnimation;
					currentAnimation = &leftJumpAnim;
				}

				if (position.x <= 0) position.x = 0;

				else if (currentAnimation != &leftRunAnim && (lastAnimation != &leftJumpAnim || Collision("bottom") == true))
				{
					leftRunAnim.Reset();
					lastAnimation = currentAnimation;
					currentAnimation = &leftRunAnim;
				}
			}

			if (Collision("left") == false)
			{
				position.x -= floor(speedX * dt);
				if (position.x < app->render->offset.x + 640 && app->render->offset.x > 0)
				{
					app->render->offset.x -= floor(speedX * dt);
					app->render->camera.x += floor(speedX * dt);
				}
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT && godMode == true)
			position.y -= floor(speedX * dt);


		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && godMode == true)
			position.y += floor(speedX * dt);

		if (currentAnimation != &leftAttackAnim || currentAnimation != &rightAttackAnim)
		{
			if (swordCollider != nullptr)
			{
				app->colliderManager->RemoveCollider(swordCollider);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && Collision("bottom") == true)
		{
			app->audio->PlayFx(jumpAudio);

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

			checkpoint = false;
			jumping = true;
			jump = true;
			speedY = 650.0f;
			position.y -= speedY * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE &&
			app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE &&
			app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE &&
			app->input->GetKey(SDL_SCANCODE_M) == KeyState::KEY_IDLE)
		{
			if (currentAnimation == &rightJumpAnim || currentAnimation == &rightRunAnim ||
				(currentAnimation == &rightAttackAnim && rightAttackAnim.HasFinished() == true))
			{
				rightIdleAnim.Reset();
				currentAnimation = &rightIdleAnim;
				lastAnimation = currentAnimation;
			}
			if (currentAnimation == &leftJumpAnim || currentAnimation == &leftRunAnim ||
				(currentAnimation == &leftAttackAnim && leftAttackAnim.HasFinished() == true))
			{
				leftIdleAnim.Reset();
				currentAnimation = &leftIdleAnim;
				lastAnimation = currentAnimation;
			}
		}

		if ((app->input->GetKey(SDL_SCANCODE_M) == KeyState::KEY_DOWN || app->input->GetMouseButtonDown(1) == KEY_DOWN) && attackCooldown == 0)
		{
			SwordAttack(dt);
			cooldownAtk.Reset();
			app->audio->PlayFx(swordFx);
		}

		Gravity(dt);

		if (Collision("top") == true && speedY > 0) speedY = -speedY + 100;

		if (speedY <= 0.0f) jumping = false;

		if (position.y >= app->render->offset.y + 360 && !(app->render->offset.y + app->render->camera.h >= (app->map->data.height * app->map->data.tileHeight)))
		{
			app->render->offset.y = position.y - 360;
			app->render->camera.y = -(position.y - 360);
		}

		if (position.y < app->render->offset.y + 360 && !(app->render->offset.y <= 0))
		{
			app->render->offset.y = position.y - 360;
			app->render->camera.y = -(position.y - 360);
		}

		playerCollider->SetPos(position.x + 12, position.y + 35, &playerCollider->rect);

		if (checkpoint == true) checkpointAnim.Update();
	}
	else
	{
		time++;
	}

	currentAnimation->Update();

	switch (attackCooldown)
	{
	case 3:
		cooldownAtk.Update();
		break;
	case 2:
		cooldownAtk.Update();
		break;
	case 1:
		cooldownAtk.Update();
		break;
	case 0:
		cooldownAtk.Update();
		break;
	}

	return true;
}

void Player::Draw() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (rect.x == 525)
		app->render->DrawTexture(texture, position.x - 51, position.y, &rect);
	else if (rect.x == 333)
		app->render->DrawTexture(texture, position.x - 48, position.y, &rect);
	else if (rect.x == 156)
		app->render->DrawTexture(texture, position.x - 36, position.y, &rect);
	else
		app->render->DrawTexture(texture, position.x, position.y, &rect);

	// Cooldown Attack HUD
	app->render->DrawTexture(cooldownTex, app->render->offset.x + 1180, app->render->offset.y + 620, &cooldownAtk.GetCurrentFrame());
}

bool Player::CleanUp() {

	// Unload textures
	app->tex->UnLoad(texture);
	app->tex->UnLoad(checkpointTex);
	app->tex->UnLoad(cooldownTex);
	
	// Reset dead animations
	leftDeadAnim.Reset();
	rightDeadAnim.Reset();
	
	// Removing collider
	if (playerCollider != nullptr) app->colliderManager->RemoveCollider(playerCollider);

	return true;
}

bool Player::Collision(const char* side)
{
	bool ret = false;

	if (godMode == false)
	{
		ListItem<MapLayer*>* lay = app->map->data.layers.start->next;

		iPoint tilePos;

		int idTile;

		if (lay->data->properties.GetProperty("Collision") == 1)
		{
			if (side == "bottom")
			{
				for (uint i = 0; i < 3; i++)
				{
					tilePos = app->map->WorldToMap(position.x + (10 + (10 * i)), position.y + 88);
					idTile = lay->data->Get(tilePos.x, tilePos.y);
					if (CheckCollisionType(idTile, "bottom"))
					{
						return true;
					}
				}
			}
			else if (side == "top")
			{
				for (uint i = 0; i < 3; i++)
				{
					tilePos = app->map->WorldToMap(position.x + (10 + (10 * i)), position.y + 38);
					idTile = lay->data->Get(tilePos.x, tilePos.y);
					if (CheckCollisionType(idTile, "top"))
					{
						return true;
					}
				}
			}
			else if (side == "right")
			{
				for (uint i = 0; i < 3; i++)
				{
					tilePos = app->map->WorldToMap(position.x + 40, position.y + (42 + (14 * i)));
					idTile = lay->data->Get(tilePos.x, tilePos.y);
					if (CheckCollisionType(idTile, "right"))
					{
						return true;
					}
				}
			}
			else if (side == "left")
			{
				for (uint i = 0; i < 3; i++)
				{
					tilePos = app->map->WorldToMap(position.x + 6, position.y + (42 + (14 * i)));
					idTile = lay->data->Get(tilePos.x, tilePos.y);
					if (CheckCollisionType(idTile, "left"))
					{
						return true;
					}
				}
			}
		}
	}

	return ret;
}

void Player::Gravity(float dt)
{
	if (Collision("bottom") == false && godMode == false)
	{
		speedY -= gravity * dt;
		position.y -= speedY * dt;
		if (speedY < -300.0f)
		{
			speedY = -300.0f;
		}
	}
}

bool Player::Load(pugi::xml_node& load)
{
	bool ret = true;

	position.x = load.child("position").attribute("x").as_float();
	position.y = load.child("position").attribute("y").as_float();

	return ret;
}

bool Player::Save(pugi::xml_node& save) const
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
}

bool Player::CheckCollisionType(int idTile, SString direction)
{
	switch (idTile)
	{
	case 289:
		return true;
		break;
	case 290:
		lifes--;
		app->audio->PlayFx(playerHurt);
		if (lifes > 0) {
			deadPlayer = false;
			playerChangePos = true;
			app->render->ResetCam();
		}
		if (lifes == 0)
		{
			Dead();
		}
		return true;
		break;
	case 291:
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

	case 292:
		levelFinished = true;
		break;
	case 293:
		if (position.y < 400) whatCheckpoint = 1;
		else if (position.y < 1400) whatCheckpoint = 2;
		else whatCheckpoint = 3;
		if (checkpoint == false)
		{
			checkpointAnim.Reset();
			app->SaveGameRequest();
			app->audio->PlayFx(checkpointAudio);

			checkpoint = true;
			break;
		}
	}

	return false;
}

void Player::SpeedAnimations(float dt)
{
	rightIdleAnim.speed = 10.0f * dt;
	leftIdleAnim.speed = 10.0f * dt;
	rightRunAnim.speed = 10.0f * dt;
	leftRunAnim.speed = 10.0f * dt;
	rightJumpAnim.speed = 10.0f * dt;
	leftJumpAnim.speed = 10.0f * dt;
	rightDeadAnim.speed = 10.0f * dt;
	leftDeadAnim.speed = 10.0f * dt;
	rightAttackAnim.speed = 10.0f * dt;
	leftAttackAnim.speed = 10.0f * dt;
	checkpointAnim.speed = 5.0f * dt;
}

void Player::LoadAnimations()
{
	// Animation when player is static
	rightIdleAnim.PushBack({ 0, 0, 51, 87 });
	rightIdleAnim.PushBack({ 192, 0, 51, 87 });
	rightIdleAnim.PushBack({ 384, 0, 51, 87 });
	rightIdleAnim.PushBack({ 576, 0, 51, 87 });
	rightIdleAnim.PushBack({ 768, 0, 51, 87 });

	rightIdleAnim.loop = true;

	leftIdleAnim.PushBack({ 0, 87, 51, 87 });
	leftIdleAnim.PushBack({ 192, 87, 51, 87 });
	leftIdleAnim.PushBack({ 384, 87, 51, 87 });
	leftIdleAnim.PushBack({ 576, 87, 51, 87 });
	leftIdleAnim.PushBack({ 768, 87, 51, 87 });

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

	rightRunAnim.loop = true;

	leftRunAnim.PushBack({ 1347, 261, 45, 87 });
	leftRunAnim.PushBack({ 1158, 261, 42, 87 });
	leftRunAnim.PushBack({ 963, 261, 45, 87 });
	leftRunAnim.PushBack({ 768, 261, 51, 87 });
	leftRunAnim.PushBack({ 573, 261, 57, 87 });
	leftRunAnim.PushBack({ 378, 261, 63, 87 });
	leftRunAnim.PushBack({ 189, 261, 57, 87 });
	leftRunAnim.PushBack({ 0, 261, 51, 87 });

	leftRunAnim.loop = true;

	// Animation when player is jumping
	rightJumpAnim.PushBack({ 0, 348, 51, 87 });
	rightJumpAnim.PushBack({ 189, 348, 51, 87 });
	rightJumpAnim.PushBack({ 378, 348, 51, 87 });

	rightJumpAnim.loop = false;

	leftJumpAnim.PushBack({ 1341, 348, 51, 87 });
	leftJumpAnim.PushBack({ 1152, 348, 51, 87 });
	leftJumpAnim.PushBack({ 963, 348, 51, 87 });

	leftJumpAnim.loop = false;

	// Animation when player dies
	rightDeadAnim.PushBack({ 0, 783, 51, 87 });
	rightDeadAnim.PushBack({ 159, 783, 93, 87 });
	rightDeadAnim.PushBack({ 363, 783, 117, 87 });
	rightDeadAnim.PushBack({ 552, 783, 120, 87 });
	rightDeadAnim.PushBack({ 750, 783, 114, 87 });
	rightDeadAnim.PushBack({ 939, 783, 117, 87 });
	rightDeadAnim.PushBack({ 1131, 783, 117, 87 });

	rightDeadAnim.loop = false;

	leftDeadAnim.PushBack({ 1197, 870, 51, 87 });
	leftDeadAnim.PushBack({ 996, 870, 93, 87 });
	leftDeadAnim.PushBack({ 768, 870, 117, 87 });
	leftDeadAnim.PushBack({ 576, 870, 120, 87 });
	leftDeadAnim.PushBack({ 384, 870, 114, 87 });
	leftDeadAnim.PushBack({ 192, 870, 117, 87 });
	leftDeadAnim.PushBack({ 0, 870, 117, 87 });

	leftDeadAnim.loop = false;

	// Animation when player attack
	rightAttackAnim.PushBack({ 0, 522, 42, 87 });
	rightAttackAnim.PushBack({ 186, 522, 48, 87 });
	rightAttackAnim.PushBack({ 378, 522, 102, 87 });
	rightAttackAnim.PushBack({ 573, 522, 99, 87 });
	rightAttackAnim.PushBack({ 771, 522, 78, 87 });
	rightAttackAnim.PushBack({ 963, 522, 42, 87 });

	rightAttackAnim.loop = false;

	leftAttackAnim.PushBack({ 963, 609, 42, 87 });
	leftAttackAnim.PushBack({ 771, 609, 48, 87 });
	leftAttackAnim.PushBack({ 525, 609, 102, 87 });
	leftAttackAnim.PushBack({ 333, 609, 99, 87 });
	leftAttackAnim.PushBack({ 156, 609, 78, 87 });
	leftAttackAnim.PushBack({ 0, 609, 42, 87 });

	leftAttackAnim.loop = false;

	//cooldownAtk.PushBack({ 114, 2, 27,27 });
	cooldownAtk.PushBack({ 6, 6, 91, 91 });
	cooldownAtk.PushBack({ 99, 6, 91, 91 });
	cooldownAtk.PushBack({ 193, 6, 91, 91 });
	cooldownAtk.PushBack({ 286, 6, 91, 91 });
	cooldownAtk.PushBack({ 379, 6, 91, 91 });

	cooldownAtk.loop = false;

	checkpointAnim.PushBack({ 0, 0, 78, 54 });
	checkpointAnim.PushBack({ 78, 0, 78, 54 });
	checkpointAnim.PushBack({ 156, 0, 78, 54 });
	checkpointAnim.PushBack({ 234, 0, 78, 54 });
	checkpointAnim.PushBack({ 312, 0, 78, 54 });

	checkpointAnim.loop = false;
}

void Player::SwordAttack(float dt)
{
	lastAnimation = currentAnimation;
	attackCooldown = 180;
	if (currentAnimation == &rightIdleAnim || currentAnimation == &rightRunAnim || currentAnimation == &rightJumpAnim)
	{
		rightAttackAnim.Reset();
		currentAnimation = &rightAttackAnim;
		swordCollider = app->colliderManager->AddCollider({ (int)position.x + 51, (int)position.y + 20, 51, 66 }, Collider::SWORD);
	}
	else if (currentAnimation == &leftIdleAnim || currentAnimation == &leftRunAnim || currentAnimation == &leftJumpAnim)
	{
		leftAttackAnim.Reset();
		currentAnimation = &leftAttackAnim;
		swordCollider = app->colliderManager->AddCollider({ (int)position.x - 51, (int)position.y + 20, 51, 66 }, Collider::SWORD);
	}
}