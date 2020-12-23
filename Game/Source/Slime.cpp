#include "Slime.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Player.h"
#include "Audio.h"

Slime::Slime(iPoint pos) : Enemy(pos)
{
	name.Create("slime");

	animLeft.PushBack({ 44,20,34,20 });
	animLeft.PushBack({ 2,20,36,20 });

	animLeft.loop = true;

	animRight.PushBack({ 82,20,36,20 });
	animRight.PushBack({ 122,20,34,20 });

	animRight.loop = true;

	hitLeftAnim.PushBack({ 2,48,36,20 });
	hitLeftAnim.PushBack({ 44,48,34,20 });
	hitLeftAnim.PushBack({ 82,48,36,20 });

	hitLeftAnim.loop = false;

	hitRightAnim.PushBack({ 121,48,36,20 });
	hitRightAnim.PushBack({ 161,48,34,20 });
	hitRightAnim.PushBack({ 201,48,36,20 });

	hitRightAnim.loop = false;

	deathAnim.PushBack({ 2,74, 32,20 });
	deathAnim.PushBack({ 44,74, 32,20 });
	deathAnim.PushBack({ 84,74, 32,20 });

	deathAnim.loop = false;

	this->collider = app->colliderManager->AddCollider({ this->position.x + 4, this->position.y + 3, 27, 17 }, Collider::Type::ENEMY);
	this->speedY = 0;
	this->currentAnimation = &animRight;
	this->lifes = 3;
	this->state = SLEEP;
	this->alive = true;
	this->moveRight = true;
	this->path.Clear();
}

Slime::~Slime() {}

bool Slime::Update(float dt)
{
	if (this->alive)
	{
		this->animLeft.speed = 5.0f * dt;
		this->animRight.speed = 5.0f * dt;
		this->hitLeftAnim.speed = 7.0f * dt;
		this->hitRightAnim.speed = 7.0f * dt;
		this->deathAnim.speed = 5.0f * dt;

		if (this->lifes == 0)
		{
			this->currentAnimation = &deathAnim;

			if (this->deathAnim.HasFinished())
			{
				this->alive = false;
				this->CleanUp();
			}
		}

		if (this->state == SLEEP && this->currentAnimation != &deathAnim)
		{
			if (Sleep(dt) == true)
				this->state = AWAKE;
		}

		else if (this->state == AWAKE && this->currentAnimation != &deathAnim)
		{
			//if (FindGoal(app->player) == true)
			//	this->state = ATTACK;
			//else this->state = SLEEP;
		}

		else if (this->state == ATTACK && this->currentAnimation != &deathAnim)
		{
			if (Move(dt) == false)
			{
				this->state = SLEEP;
				this->path.Clear();
			}
		}

		if (hitLeftAnim.HasFinished())
		{
			this->currentAnimation = &animLeft;
			this->hitLeftAnim.Reset();
			app->audio->PlayFx(this->fx);

		}
		else if (hitRightAnim.HasFinished())
		{
			this->currentAnimation = &animRight;
			this->hitRightAnim.Reset();
			app->audio->PlayFx(this->fx);
		}

		this->currentAnimation->Update();

		Gravity(dt);

		this->collider->SetPos(this->position.x + 4, this->position.y + 3, &this->collider->rect);
	}

	return true;
}

void Slime::Hit()
{
	if (this->currentAnimation == &animRight)
	{
		this->currentAnimation = &hitRightAnim;
		app->audio->PlayFx(this->fx);
	}
	else if (this->currentAnimation == &animLeft)
	{
		this->currentAnimation = &hitLeftAnim;
		app->audio->PlayFx(this->fx);
	}
}

bool Slime::FindGoal(Player* player)
{
	app->pathfinding->path.Clear();

	int x = player->GetPosition().x;
	int y = player->GetPosition().y;
	app->pathfinding->ResetPath(iPoint(this->position.x / 16, this->position.y / 16));
	bool found = app->pathfinding->PropagateAStar(x, y);

	if (found == true)
	{
		this->path = *(app->pathfinding->ComputePath(x, y));
		this->index = this->path.Count() - 1;
		return true;
	}

	return false;
}

bool Slime::Move(float dt)
{
	if (this->path.Count() > 0 && this->index >= 0)
	{
		if (this->path[this->index].x == this->position.x / 16 && this->path[this->index].y == this->position.y / 16)
		{
			this->index--;
			return true;
		}
		else
		{
			if (this->path[this->index].x > this->position.x / 16)
			{
				if (Collision("right") == false)
				{
					this->position.x += 125 * dt;
				}
				else
					return false;
				if (this->hitRightAnim.HasFinished())
				{
					this->currentAnimation = &animRight;
					this->hitRightAnim.Reset();
				}
				else if (currentAnimation == &animLeft)
					this->currentAnimation = &animRight;
				return true;
			}

			if (this->path[this->index].x < this->position.x / 16)
			{
				if (Collision("left") == false)
				{
					this->position.x -= 100 * dt;
				}
				else
					return false;
				if (this->hitLeftAnim.HasFinished())
				{
					this->currentAnimation = &animLeft;
					this->hitLeftAnim.Reset();
				}
				else if (currentAnimation == &animRight)
					this->currentAnimation = &animLeft;

				return true;
			}
		}
	}

	return false;
}

bool Slime::Sleep(float dt)
{
	if (Collision("right") == true)
	{
		this->moveRight = false;
	}
	else if (Collision("left") == true)
	{
		this->moveRight = true;
	}

	if (moveRight == true)
	{
		this->currentAnimation = &animRight;
		this->position.x += 100 * dt;
	}

	else
	{
		this->currentAnimation = &animLeft;
		this->position.x -= 75 * dt;
	}

	//int range;

	//range = sqrt(pow((double)app->player->GetPosition().x - this->position.x, 2) + pow((double)app->player->GetPosition().y - this->position.y, 2));

	//bool up = false;
	//bool down = false;
	//if (this->position.y - app->player->GetPosition().y < 70 &&
	//	this->position.y > app->player->GetPosition().y)
	//{
	//	up = true;
	//}
	//else if (app->player->GetPosition().y - this->position.y < 10 &&
	//	app->player->GetPosition().y > this->position.y)
	//{
	//	down = true;
	//}

	//if (range < 300 && app->player->godMode == false &&
	//	(up == true || down == true))
	//{
	//	return true;
	//}

	return false;
}

void Slime::Gravity(float dt)
{
	if (Collision("bottom") == false)
	{
		this->speedY -= 20.0f * dt;
		this->position.y -= this->speedY;
		if (this->speedY < -5.0f)
		{
			this->speedY = -5.0f;
		}
	}
}

bool Slime::Load(pugi::xml_node& load)
{
	this->alive = load.child("alive").attribute("value").as_bool();
	if (this->alive == false)
	{
		app->entityManager->RemoveEntity(this);
	}
	else
	{
		this->position.x = load.child("position").attribute("x").as_int();
		this->position.y = load.child("position").attribute("y").as_int();
		this->state = (EnemyState)load.child("state").attribute("value").as_int();
		this->lifes = load.child("lifes").attribute("value").as_int();
	}

	return true;
}

bool Slime::Save(pugi::xml_node& save) const
{
	pugi::xml_node pos = save.append_child("position");

	pos.append_attribute("x").set_value(this->position.x);
	pos.append_attribute("y").set_value(this->position.y);

	pugi::xml_node state = save.append_child("state");
	state.append_attribute("value").set_value((int)this->state);

	pugi::xml_node alive = save.append_child("alive");
	alive.append_attribute("value").set_value(this->alive);

	pugi::xml_node lifes = save.append_child("lifes");
	lifes.append_attribute("value").set_value(this->lifes);

	return true;
}

bool Slime::Collision(const char* side)
{
	bool ret = false;

	ListItem<MapLayer*>* lay = app->map->data.layers.start->next;

	iPoint tilePos;

	int idTile;

	if (lay->data->properties.GetProperty("Collision") == 1)
	{
		if (side == "bottom")
		{
			for (uint i = 0; i < 3; i++)
			{
				tilePos = app->map->WorldToMap(this->position.x + (2 + (10 * i)), this->position.y + 20);
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
				tilePos = app->map->WorldToMap(this->position.x + (2 + (10 * i)), this->position.y + 1);
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
				tilePos = app->map->WorldToMap(this->position.x + 32, this->position.y + (1 + (6 * i)));
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
				tilePos = app->map->WorldToMap(this->position.x + 2, this->position.y + (1 + (6 * i)));
				idTile = lay->data->Get(tilePos.x, tilePos.y);
				if (CheckCollisionType(idTile, "left"))
				{
					return true;
				}
			}
		}
	}

	return ret;
}

bool Slime::CheckCollisionType(int idTile, SString direction)
{
	switch (idTile)
	{
	case 289:
		return true;
		break;


	case 290:
		this->lifes = 0;
		return true;
		break;

	case 291:
		if (direction == "bottom")
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 294:
		if (direction == "right" || direction == "left")
		{
			return true;
		}
		break;
	}
	return false;
}