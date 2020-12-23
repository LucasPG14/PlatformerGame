#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "Point.h"

struct SDL_Texture;

class EntityManager : public Module
{
public:

	EntityManager();
	~EntityManager();

	bool Start();

	bool Update(float dt);

	bool Draw();

	bool CleanUp();

	bool EnemyLifes(Collider* coll);

	bool ItemPowerUp(Collider* coll);

	Entity* AddEntity(iPoint point, EntityType entityType);

	void RemoveEntity(Entity* entity);

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

public:

	List<Entity*> entities;

private:

	SDL_Texture* enemy;
	SDL_Texture* life;
	SDL_Texture* star;

	uint batFx;
	uint slimeFx;
	uint starFx;
	uint lifeFx;
};
#endif