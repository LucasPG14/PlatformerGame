#include "Textures.h"
#include "Audio.h"
#include "Star.h"
#include "Player.h"
#include "ColliderManagement.h"
#include "EntityManager.h"

Star::Star(iPoint pos, EntityType entityType) : Item(pos, entityType)
{
	starAnim.PushBack({ 3,7,42,42 });
	starAnim.PushBack({ 49,7,42,42 });
	starAnim.PushBack({ 91,7,42,42 });
	starAnim.PushBack({ 122,7,42,42 });
	starAnim.PushBack({ 157,7,42,42 });
	starAnim.PushBack({ 199,7,42,42 });

	starAnim.loop = true;

	this->collider = app->colliderManager->AddCollider({ this->position.x + 6, this->position.y + 6, 30, 30 }, Collider::Type::ITEM);

	this->collider->active = true;

	this->currentAnimation = &starAnim;
}

Star::~Star() {}

bool Star::Update(float dt)
{
	this->starAnim.speed = 2.0f * dt;

	this->currentAnimation->Update();

	if (this->collider->active == false)
	{
		app->player->SetStars(1);
		app->audio->PlayFx(this->fx);
		app->entityManager->RemoveEntity(this);
	}

	return true;
}