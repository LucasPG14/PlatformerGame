#include "Bat.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "EnemyManager.h"

Bat::Bat(iPoint position) : Enemy(position, EnemyType::BAT, 3)
{
	animLeft.PushBack({ 44,170,32,30 });
	animLeft.PushBack({ 2,170,36,30 });

	animLeft.loop = true;

	animRight.PushBack({ 84,170,32,30 });
	animRight.PushBack({ 122,170,36,30 });

	animRight.loop = true;

	currentAnim = &animRight;
}

Bat::~Bat() {}

bool Bat::Start()
{
	collider = app->colliderManager->AddCollider({ this->pos.x + 6, this->pos.y + 4, 24, 21 }, Collider::Type::ENEMY_FLY);
	//SpeedX = 
	speedY = 0;

	return true;
}

bool Bat::Update(float dt)
{
	animLeft.speed = 2.0f * dt;
	animRight.speed = 2.0f * dt;

	if (currentAnim == &animRight)
	{
		currentAnim = &animLeft;
	}
	else if (currentAnim == &animLeft)
	{
		currentAnim == &animRight;
	}

	collider->SetPos(this->pos.x + 6, this->pos.y + 4, &collider->rect);

	if (this->lifes == 0)
	{
		app->enemyManager->RemoveEnemy(this);
	}

	return true;
}

bool Bat::CleanUp()
{
	app->colliderManager->RemoveCollider(collider);

	return true;
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