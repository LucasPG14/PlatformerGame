#include "Slime.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "EnemyManager.h"

Slime::Slime(iPoint position) : Enemy(position, EnemyType::SLIME, 3)
{
	animLeft.PushBack({ 22,10,17,10 });
	animLeft.PushBack({ 1,10,18,10 });

	animLeft.loop = true;

	animRight.PushBack({ 41,10,18,10 });
	animRight.PushBack({ 61,10,17,10 });

	animRight.loop = true;
}

Slime::~Slime() {}

bool Slime::Start()
{
	tex = app->tex->Load("Assets/Textures/Characters/enemies_spritesheet.png");
	collider = app->colliderManager->AddCollider({ pos.x + 2, pos.y + 2, 12, 8 }, Collider::Type::ENEMY_WALK);

	//SpeedX = 
	speedY = 0;

	return true;
}

bool Slime::Update(float dt)
{
	animLeft.speed = 2.0f * dt;
	animRight.speed = 2.0f * dt;

	animLeft.Update();
	animRight.Update();

	Gravity(dt);

	return true;
}

void Slime::Draw()
{
	app->render->DrawTexture(tex, pos.x, pos.y, &animLeft.GetCurrentFrame());
}

bool Slime::CleanUp()
{
	app->colliderManager->RemoveCollider(collider);
	app->tex->UnLoad(tex);

	return true;
}

void Slime::Gravity(float dt)
{
	if (Collision("bottom") == false)
	{
		speedY -= 20.0f * dt;
		pos.y -= speedY;
		if (speedY < -5.0f)
		{
			speedY = -5.0f;
		}
	}
}

bool Slime::Collision(const char* side)
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
					tilePos = app->map->WorldToMap(pos.x + (2 + (4 * i)), pos.y + 8);
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
					tilePos = app->map->WorldToMap(pos.x + (10 + (10 * i)), pos.y + 40);
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
					tilePos = app->map->WorldToMap(pos.x + 40, pos.y + (42 + (14 * i)));
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
					tilePos = app->map->WorldToMap(pos.x + 6, pos.y + (42 + (14 * i)));
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

bool Slime::CheckCollisionType(int idTile, std::string direction)
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