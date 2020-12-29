#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Render.h"
#include "ColliderManagement.h"

class Item : public Entity
{
public:

	Item(iPoint pos) : Entity(pos) {}
	~Item() {}

	bool CleanUp() override
	{
		if (this->collider != nullptr) app->colliderManager->RemoveCollider(this->collider);

		this->alive = false;

		return true;
	}

	void Draw() override
	{
		if (this->alive) app->render->DrawTexture(this->texture, this->position.x, this->position.y, &this->currentAnimation->GetCurrentFrame());
	}
};

#endif