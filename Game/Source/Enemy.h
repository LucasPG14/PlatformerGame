#pragma once
#include "Animation.h"
#include "App.h"
#include "Render.h"
#include "ColliderManagement.h"
#include "Animation.h"

enum EnemyType
{
	SLIME,
	BAT,
	LIFE,
	STAR
};

enum EnemyState
{
	SLEEP,
	AWAKE,
	ATTACK
};

class Player;

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
	
	virtual bool Start() { return true; }

	virtual bool Update(float dt) { return true; }

	virtual bool CleanUp() { return true; }

	virtual void Draw() {}

	virtual void Hit() {}

	virtual bool Load(pugi::xml_node&) { return true; }

	virtual bool Save(pugi::xml_node&) const { return true; }

	virtual bool FindGoal(Player* player) { return true; }

	virtual bool Move(float dt) { return true; }

	virtual bool Sleep(float dt) { return true; }

public:

	SString name;
	Collider* collider;
	SDL_Texture* texture = nullptr;
	int lifes;
	iPoint pos;
	EnemyType type;
	EnemyState state;
	bool alive;
	int time;
	bool block;
	// A ptr to the current animation
	Animation* currentAnim = nullptr;
};