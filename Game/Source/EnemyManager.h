#pragma once

#include "Module.h"
#include "Enemy.h"
#include "List.h"
#include "Point.h"

struct SDL_Texture;

class EnemyManager : public Module
{
public:

	EnemyManager();
	~EnemyManager();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	Enemy* AddEnemy(iPoint point, EnemyType enemyType);

	void RemoveEnemy(Enemy* enemy);

	void Draw();

	void Lifes(Collider* coll);

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

private:

	List<Enemy*> enemyList;
	SDL_Texture* texture = nullptr;
	SDL_Texture* lifeTexture = nullptr;
	SDL_Texture* starTexture = nullptr;
};