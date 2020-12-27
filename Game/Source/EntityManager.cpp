#include "EntityManager.h"
#include "SceneManager.h"

#include "Textures.h"
#include "Audio.h"
#include "Entity.h"
#include "Player.h"
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
	app->tex->UnLoad(enemy);
	app->tex->UnLoad(life);
	app->tex->UnLoad(star);
}

bool EntityManager::Start()
{
	bool ret = true;

	enemy = app->tex->Load("Assets/Textures/enemies_spritesheet.png");
	life = app->tex->Load("Assets/Textures/life_anim.png");
	star = app->tex->Load("Assets/Textures/star_anim.png");

	slimeFx = app->audio->LoadFx("Assets/Audio/Fx/walking_enemy_die.wav");
	batFx = app->audio->LoadFx("Assets/Audio/Fx/flying_enemy_die.wav");
	starFx = app->audio->LoadFx("Assets/Audio/Fx/star.wav");
	lifeFx = app->audio->LoadFx("Assets/Audio/Fx/life.wav");

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr && !app->sceneManager->pause)
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
	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr)
	{
		entity->data->CleanUp();
		entity = entity->next;
	}

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

bool EntityManager::PlayerLifes()
{
	ListItem<Entity*>* entity = entities.start;

	while (entity != nullptr)
	{
		if (entity->data->name == "player")
		{
			Player* player = (Player*)entity->data;
			player->lifes++;
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
	case EntityType::PLAYER:
		entity = new Player(point);
		break;
	case EntityType::SLIME:
		entity = new Slime(point);
		entity->texture = enemy;
		entity->fx = slimeFx;
		break;
	case EntityType::BAT:
		entity = new Bat(point);
		entity->texture = enemy;
		entity->fx = batFx;
		break;
	case EntityType::STAR:
		entity = new Star(point);
		entity->texture = star;
		entity->fx = starFx;
		break;
	case EntityType::LIFE:
		entity = new Life(point);
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
	pugi::xml_node player = entity.child("player");
	pugi::xml_node slime = entity.child("slime");
	pugi::xml_node bat = entity.child("bat");
	pugi::xml_node lifes = entity.child("life");
	pugi::xml_node stars = entity.child("star");

	while (item != nullptr)
	{
		if (item->data->name == "player") item->data->Load(player);

		if (item->data->name == "slime")
		{
			item->data->Load(slime);
			slime = slime.next_sibling("slime");
		}
		if (item->data->name == "bat")
		{
			item->data->Load(bat);
			bat = bat.next_sibling("bat");
		}
		if (item->data->name == "life")
		{
			item->data->Load(lifes);
			lifes = lifes.next_sibling("life");
		}
		if (item->data->name == "star")
		{
			item->data->Load(stars);
			stars = stars.next_sibling("star");
		}
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
		ret = item->data->Save(entity.append_child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}