#include "Textures.h"
#include "App.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Slime.h"
#include "Bat.h"
#include "Star.h"
#include "Life.h"

EntityManager::EntityManager()
{
	name.Create("entities");
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Start()
{
	bool ret = true;
	if (active)
	{
		enemy = app->tex->Load("Assets/Textures/enemies_spritesheet.png");
		life = app->tex->Load("Assets/Textures/life_anim.png");
		star = app->tex->Load("Assets/Textures/star_anim.png");

		slimeFx = app->audio->LoadFx("Assets/Audio/Fx/walking_enemy_die.wav");
		batFx = app->audio->LoadFx("Assets/Audio/Fx/flying_enemy_die.wav");
		starFx = app->audio->LoadFx("Assets/Audio/Fx/star.wav");
		lifeFx = app->audio->LoadFx("Assets/Audio/Fx/life.wav");
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr)
	{
		ret = entity->data->Update(dt);
		entity = entity->next;
	}

	return ret;
}

bool EntityManager::Draw()
{
	bool ret = true;

	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr)
	{
		entity->data->Draw();
		entity = entity->next;
	}

	return ret;
}

bool EntityManager::CleanUp()
{
	app->tex->UnLoad(enemy);
	app->tex->UnLoad(life);
	app->tex->UnLoad(star);

	entities.Clear();

	return true;
}

bool EntityManager::EnemyLifes(Collider* coll)
{
	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr)
	{
		if (entity->data->collider == coll)
		{
			Enemy* enemy = (Enemy*)entity->data;
			enemy->Lifes();
			break;
		}
		entity = entity->next;
	}

	return true;
}

Entity* EntityManager::AddEntity(iPoint point, EntityType entityType)
{
	Entity* entity = nullptr;

	// Fix enum type
	switch (entityType)
	{
	case EntityType::SLIME:
		entity = new Slime(point, entityType);
		entity->texture = enemy;
		entity->fx = slimeFx;
		break;
	case EntityType::BAT:
		entity = new Bat(point, entityType);
		entity->texture = enemy;
		entity->fx = batFx;
		break;
	case EntityType::STAR:
		entity = new Star(point, entityType);
		entity->texture = star;
		entity->fx = starFx;
		break;
	case EntityType::LIFE:
		entity = new Life(point, entityType);
		entity->texture = life;
		entity->fx = lifeFx;
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::RemoveEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data == entity)
		{
			item->data->CleanUp();
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}

bool EntityManager::LoadState(pugi::xml_node& entity)
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		ret = item->data->Load(entity);
		item = item->next;
	}

	return ret;
}

bool EntityManager::SaveState(pugi::xml_node& entity) const
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		ret = item->data->Save(entity);
		item = item->next;
	}

	return ret;
}