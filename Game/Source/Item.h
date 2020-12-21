#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Render.h"
#include "ColliderManagement.h"

class Item : public Entity
{
public:

	Item(iPoint pos, EntityType t) : Entity(pos, t) {}
	~Item() {}

	bool CleanUp() override
	{
		if (this->collider != nullptr)
			app->colliderManager->RemoveCollider(this->collider);
		return true;
	}

	void Draw() override
	{
		app->render->DrawTexture(this->texture, this->position.x, this->position.y, &this->currentAnimation->GetCurrentFrame());
	}
};

#endif