#include "Slime.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "EnemyManager.h"

Slime::Slime(iPoint position) : Enemy(position, EnemyType::SLIME, 3)
{
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
}

Slime::~Slime() {}

bool Slime::Start()
{
	//tex = app->tex->Load("Assets/Textures/Characters/enemies_spritesheet.png");
	this->collider = app->colliderManager->AddCollider({ this->pos.x + 4, this->pos.y + 3, 27, 17 }, Collider::Type::ENEMY_WALK);

	//SpeedX = 
	this->speedY = 0;

	this->currentAnim = &animRight;

	return true;
}

bool Slime::Update(float dt)
{
	this->animLeft.speed = 2.0f * dt;
	this->animRight.speed = 2.0f * dt;
	this->hitLeftAnim.speed = 3.0f * dt;
	this->hitRightAnim.speed = 3.0f * dt;
	this->deathAnim.speed = 2.0f * dt;

	this->currentAnim->Update();

	Gravity(dt);

	this->collider->SetPos(this->pos.x + 4, this->pos.y + 3, &this->collider->rect);

	if (this->lifes == 0)
	{
		this->currentAnim = &deathAnim;

		if (deathAnim.HasFinished())
		{
			app->enemyManager->RemoveEnemy(this);
		}
	}

	if (hitLeftAnim.HasFinished())
	{
		this->currentAnim = &animLeft;
		this->hitLeftAnim.Reset();
	}
	else if (hitRightAnim.HasFinished())
	{
		this->currentAnim = &animRight;
		this->hitRightAnim.Reset();
	}

	return true;
}

bool Slime::CleanUp()
{
	app->colliderManager->RemoveCollider(this->collider);
	return true;
}

void Slime::Draw()
{
	app->render->DrawTexture(this->texture, this->pos.x, this->pos.y, &this->currentAnim->GetCurrentFrame());
}

void Slime::Hit()
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

void Slime::Gravity(float dt)
{
	if (Collision("bottom") == false)
	{
		speedY -= 20.0f * dt;
		this->pos.y -= speedY;
		if (speedY < -5.0f)
		{
			speedY = -5.0f;
		}
	}
}

bool Slime::Load(pugi::xml_node& load)
{
	pugi::xml_node slime = load.child("slime");
	pos.x = slime.child("position").attribute("x").as_int();
	pos.y = slime.child("position").attribute("y").as_int();

	return true;
}

bool Slime::Save(pugi::xml_node& save) const
{
	pugi::xml_node slime = save.append_child("slime");
	slime = slime.append_child("position");

	slime.append_attribute("x").set_value(pos.x);
	slime.append_attribute("y").set_value(pos.y);

	return true;
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