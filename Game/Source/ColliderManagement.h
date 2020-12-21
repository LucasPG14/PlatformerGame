#pragma once

#include "Module.h"
#include "List.h"
#include "SDL/include/SDL.h"

class Collider
{
public:

	enum Type
	{
		NONE,
		PLAYER,
		SWORD,
		ENEMY,
		ITEM
	};

	Collider(SDL_Rect rectangle, Type type);

	void SetPos(int x, int y, SDL_Rect* rect);

	bool Intersects(const SDL_Rect* r) const;

	SDL_Rect rect;
	bool active;
	Type type;

};

class ColliderManagement : public Module
{
public:

	ColliderManagement();
	~ColliderManagement();

	bool Update(float dt);

	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, Collider::Type type);

	void RemoveCollider(Collider* collider);

	void DrawColliders();

	void OnCollision(Collider* coll1, Collider* coll2);

	bool showColliders;

private:

	List<Collider*> collidersList;
};