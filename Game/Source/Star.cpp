#include "Textures.h"
#include "Audio.h"
#include "Star.h"
#include "Player.h"
#include "ColliderManagement.h"
#include "EntityManager.h"

Star::Star(iPoint pos) : Item(pos)
{
	name.Create("star");

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

	this->alive = true;
}

Star::~Star() {}

bool Star::Update(float dt)
{
	this->starAnim.speed = 2.0f * dt;

	this->currentAnimation->Update();

	if (this->collider->active == false)
	{
		//app->player->SetStars(1);
		app->audio->PlayFx(this->fx);
		app->entityManager->RemoveEntity(this);
	}

	return true;
}

bool Star::Load(pugi::xml_node& load)
{
	pugi::xml_node alive = load.child("alive");

	this->alive = alive.attribute("value").as_bool();

	if (this->alive == false) app->entityManager->RemoveEntity(this);

	return true;
}

bool Star::Save(pugi::xml_node& save) const
{
	pugi::xml_node alive = save.append_child("alive");

	alive.append_attribute("value").set_value(this->alive);

	return true;
}
