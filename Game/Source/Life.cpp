#include "Life.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Map.h"

Life::Life(iPoint position) : Enemy(position, EnemyType::LIFE, 3)
{
	lifeAnim.PushBack({ 11,9,43,40 });
	lifeAnim.PushBack({ 60,9,43,40 });
	lifeAnim.PushBack({ 98,9,43,40 });
	lifeAnim.PushBack({ 131,9,42,40 });
	lifeAnim.PushBack({ 168,9,43,40 });

	lifeAnim.loop = true;

}

Life::~Life() {

}

bool Life::Start()
{
	this->lifeTex = app->tex->Load("Assets/Textures/Characters/life_anim.png");
	this->lifeItem = app->colliderManager->AddCollider({ this->pos.x + 11, this->pos.y, 20, 40 }, Collider::Type::LIFE);
	this->lifeItem->active = true;

	return false;
}

bool Life::Update(float dt)
{

	lifeAnim.speed = 2.0f * dt;

	lifeAnim.Update();

	if (this->lifeItem->active == false)
	{
		app->enemyManager->RemoveEnemy(this);
	}

	return false;
}

bool Life::CleanUp()
{
	app->tex->UnLoad(this->lifeTex);
	app->colliderManager->RemoveCollider(this->lifeItem);
	return false;
}

void Life::Draw()
{
	app->render->DrawTexture(this->lifeTex, this->pos.x, this->pos.y, &this->lifeAnim.GetCurrentFrame());
}

bool Life::Collision(const char* side)
{
	return false;
}

bool Life::CheckCollisionType(int idTile, std::string direction)
{
	return false;
}

void Life::Gravity(float dt)
{
}