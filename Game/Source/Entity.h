#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "App.h"

struct SDL_Texture;
class Animation;
class Collider;

enum class EntityType
{
	SLIME,
	BAT,
	STAR,
	LIFE
};

class Entity
{
public:
	Entity(iPoint pos, EntityType t)
	{
		position = pos;
		type = t;
	};

	~Entity() {};

	virtual bool Update(float dt) { return true; }

	virtual void Draw() {}

	virtual bool CleanUp() { return true; }

	virtual bool Load(pugi::xml_node& entity) { return true; }

	virtual bool Save(pugi::xml_node& entity) const { return true; }

public:
	EntityType type;
	SString name;
	uint fx;
	Collider* collider;
	SDL_Texture* texture;
	Animation* currentAnimation;
	iPoint position;
	bool alive;
	bool isOnCamera;
};

#endif