#include "Bat.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "Input.h"
#include "Pathfinding.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Audio.h"

Bat::Bat(iPoint position) : Enemy(position, EnemyType::BAT, 2)
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
}

Bat::~Bat() {}

bool Bat::Start()
{
	this->collider = app->colliderManager->AddCollider({ this->pos.x + 6, this->pos.y + 4, 24, 21 }, Collider::Type::ENEMY_FLY);
	bat = app->audio->LoadFx("Assets/Audio/Fx/flying_enemy_die.wav");

	currentAnim = &animRight;

	this->state = SLEEP;
	this->alive = true;
	this->batPath.Clear();

	return true;
}

bool Bat::Update(float dt)
{
	this->animLeft.speed = 5.0f * dt;
	this->animRight.speed = 5.0f * dt;
	this->hitLeftAnim.speed = 7.0f * dt;
	this->hitRightAnim.speed = 7.0f * dt;
	this->deathAnim.speed = 5.0f * dt;

	this->currentAnim->Update();

	this->collider->SetPos(this->pos.x + 6, this->pos.y + 4, &collider->rect);

	if (this->lifes == 0)
	{
		this->currentAnim = &deathAnim;

		if (this->deathAnim.HasFinished())
		{
			app->enemyManager->RemoveEnemy(this);
			app->audio->PlayFx(bat);
		}
	}

	if (this->state == SLEEP && this->currentAnim != &deathAnim)
	{
		if (Sleep(dt) == true)
			this->state = AWAKE;
	}

	else if (this->state == AWAKE && this->currentAnim != &deathAnim)
	{
		if (FindGoal(app->player) == true)
			this->state = ATTACK;
		else this->state = SLEEP;
	}

	else if (this->state == ATTACK && this->currentAnim != &deathAnim)
	{
		if (Move(dt) == false)
		{
			this->state = SLEEP;
			this->batPath.Clear();
		}
	}

	if (hitLeftAnim.HasFinished())
	{
		this->currentAnim = &animLeft;
		this->hitLeftAnim.Reset();
		app->audio->PlayFx(bat);
	}
	else if (hitRightAnim.HasFinished())
	{
		this->currentAnim = &animRight;
		this->hitRightAnim.Reset();
		app->audio->PlayFx(bat);
	}

	return true;
}

bool Bat::CleanUp()
{
	if (this->collider != nullptr)
	{
		app->colliderManager->RemoveCollider(this->collider);
	}
	batPath.Clear();

	return true;
}

void Bat::Draw()
{
	app->render->DrawTexture(this->texture, this->pos.x, this->pos.y, &this->currentAnim->GetCurrentFrame());
	if(app->map->viewCollisions == true)
		app->pathfinding->DrawPath(this->batPath);
}

void Bat::Hit()
{
	if (this->currentAnim == &animRight)
	{
		this->currentAnim = &hitRightAnim;
	}
	else if (this->currentAnim == &animLeft)
	{
		this->currentAnim = &hitLeftAnim;
	}
}

bool Bat::FindGoal(Player* player)
{
	batPath.Clear();

	int x = player->position.x;
	int y = player->position.y;
	app->pathfinding->ResetPath(iPoint(this->pos.x / 16, this->pos.y / 16));
	bool found = app->pathfinding->PropagateAStar(x, y + 35);

	if (found == true)
	{
		this->batPath = *(app->pathfinding->ComputePath(x, y + 35));
		this->indexBat = this->batPath.Count() - 1;
		return true;
	}

	return false;
}

bool Bat::Move(float dt)
{
	if (this->batPath.Count() > 0 && this->indexBat >= 0)
	{
		if (this->batPath[this->indexBat].x == this->pos.x / 16 &&
			this->batPath[this->indexBat].y == this->pos.y / 16)
		{
			this->indexBat--;
			return true;
		}
		else
		{
			if (this->batPath[this->indexBat].x > this->pos.x / 16)
			{
				if (Collision("right") == false)
				{
					this->pos.x += 150 * dt;
				}
				else
					return false;
				if (this->hitRightAnim.HasFinished())
				{
					this->currentAnim = &animRight;
					this->hitRightAnim.Reset();
				}
				else if (currentAnim == &animLeft)
					this->currentAnim = &animRight;
				return true;
			}

			if (this->batPath[this->indexBat].x < this->pos.x / 16)
			{
				if (Collision("left") == false)
				{
					this->pos.x -= 125 * dt;
				}
				else
					return false;
				if (this->hitLeftAnim.HasFinished())
				{
					this->currentAnim = &animLeft;
					this->hitLeftAnim.Reset();
				}
				else if (currentAnim == &animRight)
					this->currentAnim = &animLeft;

				return true;
			}

			if (this->batPath[this->indexBat].y < this->pos.y / 16)
			{
				if (Collision("top") == false)
				{
					this->pos.y -= 125 * dt;
				}
				else
					return false;

				return true;
			}

			if (this->batPath[this->indexBat].y > this->pos.y / 16)
			{
				if (Collision("bottom") == false)
				{
					this->pos.y += 125 * dt;
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
		this->currentAnim = &animRight;
		this->pos.x += 125 * dt;
	}
	else
	{
		this->currentAnim = &animLeft;
		this->pos.x -= 100 * dt;
	}

	int range;

	range = sqrt(pow((double)app->player->GetPosition().x - this->pos.x, 2) + pow((double)app->player->GetPosition().y - this->pos.y, 2));
	bool right = false;
	bool left = false;
	if (this->pos.x - app->player->GetPosition().x < 100 &&
		this->pos.x > app->player->GetPosition().x)
	{
		right = true;
	}
	else if (app->player->GetPosition().x - this->pos.x < 100 &&
		app->player->GetPosition().x > this->pos.x)
	{
		left = true;
	}

	if (range < 300 && app->player->godMode == false &&
		(right == true || left == true))
	{
		return true;
	}

	return false;
}

bool Bat::Collision(const char* side)
{
	bool ret = false;

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
					tilePos = app->map->WorldToMap(this->pos.x + (2 + (10 * i)), this->pos.y + 20);
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
					tilePos = app->map->WorldToMap(this->pos.x + (2 + (10 * i)), this->pos.y + 1);
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
					tilePos = app->map->WorldToMap(this->pos.x + 33, this->pos.y + (1 + (6 * i)));
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
					tilePos = app->map->WorldToMap(this->pos.x + 1, this->pos.y + (1 + (6 * i)));
					idTile = lay->data->Get(tilePos.x, tilePos.y);
					if (CheckCollisionType(idTile, "left"))
					{
						return true;
					}
				}
			}
		}
		lay = lay->next;
	}

	return ret;
}

bool Bat::CheckCollisionType(int idTile, std::string direction)
{
	switch (idTile)
	{
	case 289:
		block = true;
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
	this->pos.x = load.child("position").attribute("x").as_int();
	this->pos.y = load.child("position").attribute("y").as_int();
	this->state = (EnemyState)load.child("state").attribute("value").as_int();
	this->alive = load.child("alive").attribute("value").as_bool();

	return true;
}

bool Bat::Save(pugi::xml_node& save) const
{
	pugi::xml_node bat = save.append_child("position");
	
	bat.append_attribute("x").set_value(this->pos.x);
	bat.append_attribute("y").set_value(this->pos.y);

	pugi::xml_node state = save.append_child("state");
	state.append_attribute("value").set_value((int)this->state);

	pugi::xml_node alive = save.append_child("alive");
	alive.append_attribute("value").set_value(this->alive);

	return true;
}