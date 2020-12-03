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
}

Bat::~Bat() {}

bool Bat::Start()
{
	tex = app->tex->Load("Assets/Textures/Characters/enemies_spritesheet.png");
	collider = app->colliderManager->AddCollider({ this->pos.x + 6, this->pos.y + 4, 24, 21 }, Collider::Type::ENEMY_FLY);

	//SpeedX = 
	speedY = 0;

	return true;
}

bool Bat::Update(float dt)
{
	animLeft.speed = 2.0f * dt;
	animRight.speed = 2.0f * dt;

	animLeft.Update();
	animRight.Update();

	collider->SetPos(this->pos.x + 6, this->pos.y + 4, &collider->rect);

	return true;
}

void Bat::Draw()
{
	app->render->DrawTexture(tex, this->pos.x, this->pos.y, &animLeft.GetCurrentFrame());
}

bool Bat::CleanUp()
{
	app->colliderManager->RemoveCollider(collider);
	app->tex->UnLoad(tex);

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