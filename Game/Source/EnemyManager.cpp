#include "EnemyManager.h"
#include "Slime.h"
#include "Bat.h"
#include "Life.h"
#include "App.h"

EnemyManager::EnemyManager()
{
	name.Create("enemies");
}

EnemyManager::~EnemyManager()
{}

bool EnemyManager::Start()
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		enemies->data->Start();
		enemies = enemies->next;
	}

	return true;
}

bool EnemyManager::Update(float dt)
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		enemies->data->Update(dt);
		enemies = enemies->next;
	}

	return true;
}

void EnemyManager::Draw()
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		enemies->data->Draw();
		enemies = enemies->next;
	}
}

void EnemyManager::Lifes(Collider* coll)
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		if (enemies->data->collider == coll)
		{
			enemies->data->lifes--;
			break;
		}
		enemies = enemies->next;
	}
}

bool EnemyManager::LoadState(pugi::xml_node& load)
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		enemies->data->Load(load);
		enemies = enemies->next;
	}

	return true;
}

bool EnemyManager::SaveState(pugi::xml_node& save) const
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		enemies->data->Save(save);
		enemies = enemies->next;
	}

	return true;
}

bool EnemyManager::CleanUp()
{
	ListItem<Enemy*>* enemies = enemyList.start;

	while (enemies != nullptr)
	{
		RemoveEnemy(enemies->data);
		enemies = enemies->next;
	}

	return true;
}

Enemy* EnemyManager::AddEnemy(iPoint point, EnemyType enemyType)
{
	Enemy* ret = nullptr;

	switch (enemyType)
	{
	case SLIME:
		ret = new Slime(point);
		break;
	case BAT:
		ret = new Bat(point);
		ret;
		break;
	case LIFE:
		ret = new Life(point);
		ret;
		break;
	}

	enemyList.Add(ret);

	return ret;
}

void EnemyManager::RemoveEnemy(Enemy* enemy)
{
	int index;
	ListItem<Enemy*>* aux;

	index = enemyList.Find(enemy);

	if (index != -1)
	{
		aux = enemyList.At(index);
		aux->data->CleanUp();
		enemyList.Del(aux);
	}
}