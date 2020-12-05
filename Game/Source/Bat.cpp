#include "Bat.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "Input.h"
#include "Pathfinding.h"
#include "EnemyManager.h"
#include "Player.h"

Bat::Bat(iPoint position) : Enemy(position, EnemyType::BAT, 2)
{
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

	currentAnim = &animRight;

	return true;
}

bool Bat::Update(float dt)
{
	this->animLeft.speed = 2.0f * dt;
	this->animRight.speed = 2.0f * dt;
	this->hitLeftAnim.speed = 3.0f * dt;
	this->hitRightAnim.speed = 3.0f * dt;
	this->deathAnim.speed = 20.0f * dt;

	this->currentAnim->Update();

	this->collider->SetPos(this->pos.x + 6, this->pos.y + 4, &collider->rect);

	if (this->lifes == 0)
	{
		this->currentAnim = &deathAnim;

		if (deathAnim.HasFinished())
		{
			app->enemyManager->RemoveEnemy(this);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		FindGoal(app->player);

	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		Move();

	if (hitLeftAnim.HasFinished())
	{
		this->currentAnim = &animLeft;
		hitLeftAnim.Reset();
	}
	else if (hitRightAnim.HasFinished())
	{
		this->currentAnim = &animRight;
		hitRightAnim.Reset();
	}

	return true;
}

bool Bat::CleanUp()
{
	app->colliderManager->RemoveCollider(this->collider);
	app->enemyManager->RemoveEnemy(this);
	batPath.Clear();

	return true;
}

void Bat::Draw()
{
	app->render->DrawTexture(this->texture, this->pos.x, this->pos.y, &this->currentAnim->GetCurrentFrame());
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
	app->pathfinding->path.Clear();

	int x = player->position.x;
	int y = player->position.y + 32;
	app->pathfinding->ResetPath(iPoint(this->pos.x / 16, this->pos.y / 16));
	app->pathfinding->PropagateDijkstra(player);

	batPath = *(app->pathfinding->ComputePath(x, y));

	indexBat = batPath.Count() - 1;

	return true;
}

bool Bat::Move()
{
	if (batPath[indexBat].x == this->pos.x / 16 && batPath[indexBat].y == this->pos.y / 16)
	{
		indexBat--;
	}
	else
	{
		if (batPath[indexBat].x > this->pos.x / 16)
		{
			this->pos.x += 6;
		}

		if (batPath[indexBat].x < this->pos.x / 16)
		{
			this->pos.x -= 6;
		}

		if (batPath[indexBat].y < this->pos.y / 16)
		{
			this->pos.y -= 6;
		}

		if (batPath[indexBat].y > this->pos.y / 16)
		{
			this->pos.y += 6;
		}
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
					tilePos = app->map->WorldToMap(this->pos.x + 32, this->pos.y + (1 + (6 * i)));
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
					tilePos = app->map->WorldToMap(this->pos.x + 2, this->pos.y + (1 + (6 * i)));
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
		return true;
		break;


	case 290:
		app->enemyManager->RemoveEnemy(this);
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
	}

	return false;
}

bool Bat::Load(pugi::xml_node& load)
{
	this->pos.x = load.child("bat").child("position").attribute("x").as_int();
	this->pos.y = load.child("bat").child("position").attribute("y").as_int();

	return true;
}

bool Bat::Save(pugi::xml_node& save) const
{
	pugi::xml_node bat = save.append_child("bat");
	bat = bat.append_child("position");

	bat.append_attribute("x").set_value(pos.x);
	bat.append_attribute("y").set_value(pos.y);

	return true;
}