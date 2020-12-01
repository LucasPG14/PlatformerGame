#pragma once
#include "Animation.h"
#include "Render.h"
#include "ColliderManagement.h"

enum EnemyType
{
	SLIME,
	BAT
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

	int lifes;
	iPoint pos;
	EnemyType type;
};