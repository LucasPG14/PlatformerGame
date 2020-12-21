#include "Textures.h"
#include "Life.h"
#include "Player.h"
#include "ColliderManagement.h"
#include "EntityManager.h"
#include "Audio.h"

Life::Life(iPoint pos, EntityType entityType) : Item(pos, entityType)
{
	lifeAnim.PushBack({ 11,9,43,40 });
	lifeAnim.PushBack({ 60,9,43,40 });
	lifeAnim.PushBack({ 98,9,43,40 });
	lifeAnim.PushBack({ 131,9,42,40 });
	lifeAnim.PushBack({ 168,9,43,40 });

	lifeAnim.loop = true;

	this->currentAnimation = &lifeAnim;

	this->collider = app->colliderManager->AddCollider({ this->position.x + 11, this->position.y, 20, 40 }, Collider::Type::ITEM);
	this->collider->active = true;
}

Life::~Life() {

}

bool Life::Update(float dt)
{

	lifeAnim.speed = 2.0f * dt;

	this->currentAnimation->Update();

	if (this->collider->active == false)
	{
		app->player->lifes++;
		app->audio->PlayFx(this->fx);
		app->entityManager->RemoveEntity(this);
	}

	return true;
}