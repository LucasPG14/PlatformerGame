#include "ColliderManagement.h"
#include "EnemyManager.h"
#include "Render.h"
#include "Player.h"
#include "App.h"

ColliderManagement::ColliderManagement()
{
	showColliders = false;
}

ColliderManagement::~ColliderManagement(){}

bool ColliderManagement::Update(float dt)
{
	if (app->player->godMode == false)
	{
		ListItem<Collider*>* coll1 = collidersList.start;
		ListItem<Collider*>* coll2;

		while (coll1 != nullptr)
		{
			coll2 = coll1->next;

			while (coll2 != nullptr)
			{
				if (coll1->data->Intersects(&coll2->data->rect))
				{
					OnCollision(coll1->data, coll2->data);
				}
				else if (coll2->data->Intersects(&coll1->data->rect))
				{
					OnCollision(coll2->data, coll1->data);
				}
				coll2 = coll2->next;
			}
			coll1 = coll1->next;
		}
	}

	return true;
}

void ColliderManagement::DrawColliders()
{
	if (showColliders == false)
		return;

	Uint8 alpha = 80;
	ListItem<Collider*>* coll = collidersList.start;

	while (coll != nullptr)
	{
		switch (coll->data->type)
		{
		case Collider::Type::NONE:
			app->render->DrawRectangle(coll->data->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::PLAYER:
			app->render->DrawRectangle(coll->data->rect, 225, 0, 0, alpha);
			break;
		case Collider::Type::SWORD:
			app->render->DrawRectangle(coll->data->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::ENEMY_WALK:
			app->render->DrawRectangle(coll->data->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::ENEMY_FLY:
			app->render->DrawRectangle(coll->data->rect, 255, 255, 0, alpha);
			break;
		case Collider::Type::LIFE:
			app->render->DrawRectangle(coll->data->rect, 255, 0, 255, alpha);
			break;
		case Collider::Type::STAR:
			app->render->DrawRectangle(coll->data->rect, 255, 0, 255, alpha);
			break;
		}
		coll = coll->next;
	}
}

bool ColliderManagement::CleanUp()
{
	collidersList.Clear();

	return true;
}

Collider* ColliderManagement::AddCollider(SDL_Rect rect, Collider::Type type)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type);

	collidersList.Add(ret);

	return ret;
}

void ColliderManagement::RemoveCollider(Collider* collider)
{
	int index;
	ListItem<Collider*>* aux;

	index = collidersList.Find(collider);
	
	if (index != -1)
	{
		aux = collidersList.At(index);
		collidersList.Del(aux);
	}
}

void ColliderManagement::OnCollision(Collider* coll1, Collider* coll2)
{
	if (coll1->type == Collider::Type::PLAYER && (coll2->type == Collider::Type::ENEMY_WALK || coll2->type == Collider::Type::ENEMY_FLY))
	{
		app->player->lifes--;

		if (app->player->lifes > 0) {
			app->player->deadPlayer = false;
			app->player->playerChangePos = true;
			app->render->ResetCam();
		}
		if (app->player->lifes == 0)
		{
			RemoveCollider(coll1);
			app->player->Dead();
		}
	}
	else if (coll2->type == Collider::Type::PLAYER && (coll1->type == Collider::Type::ENEMY_WALK || coll1->type == Collider::Type::ENEMY_FLY))
	{
		app->player->lifes--;

		if (app->player->lifes > 0) {
			app->player->deadPlayer = false;
			app->player->playerChangePos = true;
			app->render->ResetCam();
		}
		if (app->player->lifes == 0)
		{
			RemoveCollider(coll2);
			app->player->Dead();
		}
	}
	else if (coll1->type == Collider::Type::SWORD && (coll2->type == Collider::Type::ENEMY_WALK || coll2->type == Collider::Type::ENEMY_FLY))
	{
		app->enemyManager->Lifes(coll2);
		RemoveCollider(coll1);
		app->player->score = app->player->score +100;
	}
	else if (coll2->type == Collider::Type::SWORD && (coll1->type == Collider::Type::ENEMY_WALK || coll1->type == Collider::Type::ENEMY_FLY))
	{
		app->enemyManager->Lifes(coll1);
		RemoveCollider(coll2);
		app->player->score = app->player->score + 100;
	}
	else if (coll1->type == Collider::Type::PLAYER && (coll2->type == Collider::Type::LIFE))
	{
		app->player->lifes++;
		coll2->active = false;
		RemoveCollider(coll2);
		
	}
	else if (coll1->type == Collider::Type::PLAYER && (coll2->type == Collider::Type::STAR))
	{
		app->player->stars++;
		coll2->active = false;
		RemoveCollider(coll2);
	}
}

Collider::Collider(SDL_Rect rectangle, Type type) : rect(rectangle), type(type)
{}

void Collider::SetPos(int x, int y, SDL_Rect* rect)
{
	rect->x = x;
	rect->y = y;
}

bool Collider::Intersects(const SDL_Rect* r) const
{
	return (rect.x < r->x + r->w &&
		rect.x + rect.w > r->x &&
		rect.y < r->y + r->h &&
		rect.h + rect.y > r->y);
}