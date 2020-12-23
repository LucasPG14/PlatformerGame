#pragma once
#include "Animation.h"
#include "Render.h"
#include "ColliderManagement.h"
#include "Animation.h"
#include "Entity.h"
#include "Map.h"
#include "Pathfinding.h"
#include "DynArray.h"

enum EnemyState
{
	SLEEP,
	AWAKE,
	ATTACK
};

class Player;

class Enemy : public Entity
{
public:
	Enemy(iPoint pos) : Entity(pos) {};
	virtual ~Enemy() {};

	virtual void Hit() { return; }

	virtual bool FindGoal(Player* player) { return true; }

	void Draw() override
	{
		if (this->alive)
			app->render->DrawTexture(this->texture, this->position.x, this->position.y, &this->currentAnimation->GetCurrentFrame());

		if (app->map->viewCollisions == true)
			app->pathfinding->DrawPath(this->path);
	}

	bool CleanUp() override
	{
		if (this->collider != nullptr)
			app->colliderManager->RemoveCollider(this->collider);
		path.Clear();

		return true;
	}

	virtual bool Move(float dt) { return true; }

	virtual bool Sleep(float dt) { return true; }

	virtual void Lifes()
	{
		this->lifes--;
	}

	virtual bool Load(pugi::xml_node& entity) { return true; }

	virtual bool Save(pugi::xml_node& entity) const { return true; }

public:

	int lifes;
	DynArray<iPoint> path;
	int index;
	EnemyState state;
};