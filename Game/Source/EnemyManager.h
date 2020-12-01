#pragma once

#include "Module.h"
#include "Enemy.h"
#include "List.h"
#include "Point.h"

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

private:

	List<Enemy*> enemyList;
};