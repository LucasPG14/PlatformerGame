#pragma once
#include "Animation.h"
#include "Render.h"
#include "ColliderManagement.h"

enum EnemyType
{
	SLIME,
	BAT,
	LIFE
};

class Enemy
{
public:

	Enemy(iPoint position, EnemyType enemyType, int life) 
	{ 
		pos = position;
		type = enemyType;
		lifes = life;
	};
	~Enemy() {};
	
	virtual bool Start() { return true; };

	virtual bool Update(float dt) { return true; };

	virtual bool CleanUp() { return true; };

	virtual void Draw() {};

	virtual bool Load(pugi::xml_node&) { return true; }

	virtual bool Save(pugi::xml_node&) const { return true; }


	Collider* collider;
	int lifes;
	iPoint pos;
	EnemyType type;
};