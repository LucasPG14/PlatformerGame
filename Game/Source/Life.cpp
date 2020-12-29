#include "Textures.h"
#include "Life.h"
#include "Player.h"
#include "ColliderManagement.h"
#include "EntityManager.h"
#include "Audio.h"

Life::Life(iPoint pos) : Item(pos)
{
	name.Create("life");

	lifeAnim.PushBack({ 11,9,43,40 });
	lifeAnim.PushBack({ 60,9,43,40 });
	lifeAnim.PushBack({ 98,9,43,40 });
	lifeAnim.PushBack({ 131,9,42,40 });
	lifeAnim.PushBack({ 168,9,43,40 });

	lifeAnim.loop = true;

	this->currentAnimation = &lifeAnim;

	this->collider = app->colliderManager->AddCollider({ this->position.x + 11, this->position.y, 20, 40 }, Collider::Type::ITEM);
	this->collider->active = true;
	this->alive = true;
}

Life::~Life() {

}

bool Life::Update(float dt)
{
	if (this->alive)
	{
		lifeAnim.speed = 2.0f * dt;

		this->currentAnimation->Update();

		if (this->collider->active == false)
		{
			app->entityManager->PlayerLifes();
			app->audio->PlayFx(this->fx);
			this->CleanUp();
		}
	}

	return true;
}

bool Life::Load(pugi::xml_node& load)
{
	pugi::xml_node alive = load.child("alive");

	this->alive = alive.attribute("value").as_bool();

	if (this->alive == false) app->entityManager->RemoveEntity(this);

	return true;
}

bool Life::Save(pugi::xml_node& save) const
{
	pugi::xml_node alive = save.append_child("alive");

	alive.append_attribute("value").set_value(this->alive);

	return true;
}