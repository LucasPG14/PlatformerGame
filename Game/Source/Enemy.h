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

	virtual bool Update(float dt) 
	{ 
		if (currentAnim != nullptr)
			currentAnim->Update();

		return true; 
	}

	virtual bool CleanUp() { return true; }

	virtual void Draw()
	{
		if (this->currentAnim != nullptr && this->lifes != 0)
			app->render->DrawTexture(texture, this->pos.x, this->pos.y, &(this->currentAnim->GetCurrentFrame()));
	}

	virtual bool Load(pugi::xml_node&) { return true; }

	virtual bool Save(pugi::xml_node&) const { return true; }


	Collider* collider;
	SDL_Texture* texture = nullptr;
	int lifes;
	iPoint pos;
	EnemyType type;
	// A ptr to the current animation
	Animation* currentAnim = nullptr;
};