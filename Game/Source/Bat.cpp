#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Player.h"
#include "Bat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Render.h"

Bat::Bat(iPoint pos) : Enemy(pos)
{
	name.Create("bat");

	animLeft.PushBack({ 44,170,32,30 });
	animLeft.PushBack({ 2,170,36,30 });

	animLeft.loop = true;

	animRight.PushBack({ 84,170,32,30 });
	animRight.PushBack({ 122,170,36,30 });

	animRight.loop = true;

	hitLeftAnim.PushBack({ 2,135,36,30 });
	hitLeftAnim.PushBack({ 44,135,32,30 });
	hitLeftAnim.PushBack({ 84,135,36,30 });

	hitLeftAnim.loop = false;

	hitRightAnim.PushBack({ 125,135,36,30 });
	hitRightAnim.PushBack({ 169,135,32,30 });
	hitRightAnim.PushBack({ 207,135,36,30 });

	hitRightAnim.loop = false;

	deathAnim.PushBack({ 4,103,32,30 });
	deathAnim.PushBack({ 44,103,32,30 });
	deathAnim.PushBack({ 81,103,32,20 });

	deathAnim.loop = false;

	this->collider = app->colliderManager->AddCollider({ this->position.x + 6, this->position.y + 4, 24, 21 }, Collider::Type::ENEMY);

	currentAnimation = &animRight;
	this->moveRight = true;

	this->state = SLEEP;
	this->lifes = 2;
	this->alive = true;
	this->path.Clear();
	this->time = 0;
}

Bat::~Bat() {}

bool Bat::Update(float dt)
{
	if (this->alive == true)
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
				app->entityManager->RemoveEntity(this);
				app->audio->PlayFx(this->fx);
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

		this->collider->SetPos(this->position.x + 6, this->position.y + 4, &collider->rect);
	}

	return true;
}

void Bat::Hit()
{
	if (this->currentAnimation == &animRight)
	{
		this->currentAnimation = &hitRightAnim;
	}
	else if (this->currentAnimation == &animLeft)
	{
		this->currentAnimation = &hitLeftAnim;
	}
}

bool Bat::FindGoal(Player* player)
{
	this->path.Clear();

	int x = player->GetPosition().x;
	int y = player->GetPosition().y;
	app->pathfinding->ResetPath(iPoint(this->position.x / 16, this->position.y / 16));
	bool found = app->pathfinding->PropagateAStar(x, y + 35);

	if (found == true)
	{
		this->path = *(app->pathfinding->ComputePath(x, y + 35));
		this->index = this->path.Count() - 1;
		return true;
	}

	return false;
}

bool Bat::Move(float dt)
{
	if (this->path.Count() > 0 && this->index >= 0)
	{
		if (this->path[this->index].x == this->position.x / 16 &&
			this->path[this->index].y == this->position.y / 16)
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
					this->position.x -= 125 * dt;
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

			if (this->path[this->index].y < this->position.y / 16)
			{
				if (Collision("top") == false)
				{
					this->position.y -= 125 * dt;
				}
				else
					return false;

				return true;
			}

			if (this->path[this->index].y > this->position.y / 16)
			{
				if (Collision("bottom") == false)
				{
					this->position.y += 125 * dt;
				}
				else
					return false;

				return true;
			}
		}
	}

	return false;
}

bool Bat::Sleep(float dt)
{
	if (Collision("right") == true)
	{
		this->moveRight = false;
	}
	else if (Collision("left") == true)
	{
		this->moveRight = true;
	}

	if (this->moveRight == true)
	{
		this->currentAnimation = &animRight;
		this->position.x += 100 * dt;
	}
	else
	{
		this->currentAnimation = &animLeft;
		this->position.x -= 100 * dt;
	}

	//int range;

	//range = sqrt(pow((double)app->player->GetPosition().x - this->position.x, 2) + pow((double)app->player->GetPosition().y - this->position.y, 2));
	//bool right = false;
	//bool left = false;
	//if (this->position.x - app->player->GetPosition().x < 100 &&
	//	this->position.x > app->player->GetPosition().x)
	//{
	//	right = true;
	//}
	//else if (app->player->GetPosition().x - this->position.x < 100 &&
	//	app->player->GetPosition().x > this->position.x)
	//{
	//	left = true;
	//}

	//if (range < 300 && app->player->godMode == false &&
	//	(right == true || left == true) && app->player->GetPosition().y + 81 > this->position.y)
	//{
	//	return true;
	//}

	return false;
}

bool Bat::Collision(const char* side)
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
				tilePos = app->map->WorldToMap(this->position.x + 33, this->position.y + (1 + (6 * i)));
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
				tilePos = app->map->WorldToMap(this->position.x + 1, this->position.y + (1 + (6 * i)));
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

bool Bat::CheckCollisionType(int idTile, SString direction)
{
	switch (idTile)
	{
	case 289:
		return true;
		break;
	case 290:
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

bool Bat::Load(pugi::xml_node& load)
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
	this->time = 0;

	return true;
}

bool Bat::Save(pugi::xml_node& save) const
{
	pugi::xml_node bat = save.append_child("position");
	
	bat.append_attribute("x").set_value(this->position.x);
	bat.append_attribute("y").set_value(this->position.y);

	pugi::xml_node state = save.append_child("state");
	state.append_attribute("value").set_value((int)this->state);

	pugi::xml_node alive = save.append_child("alive");
	alive.append_attribute("value").set_value(this->alive);

	pugi::xml_node lifes = save.append_child("lifes");
	lifes.append_attribute("value").set_value(this->lifes);

	return true;
}