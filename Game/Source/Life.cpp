#include "Life.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Map.h"

Life::Life(iPoint position) : Enemy(position, EnemyType::LIFE, 3)
{
	lifeAnim.PushBack({ 11,9,43,52 });
	lifeAnim.PushBack({ 66,9,33,52 });
	lifeAnim.PushBack({ 110,9,21,52 });
	lifeAnim.PushBack({ 141,9,21,52 });
	lifeAnim.PushBack({ 173,9,32,52 });

	lifeAnim.loop = true;

}

Life::~Life() {

}

bool Life::Start()
{
	lifeTex = app->tex->Load("Assets/Textures/Characters/lifeAnim.png");
	lifeItem = app->colliderManager->AddCollider({ this->pos.x + 4, this->pos.y + 3, 27, 17 }, Collider::Type::LIFE);

	return false;
}

bool Life::Update(float dt)
{

	lifeAnim.speed = 2.0f * dt;

	lifeAnim.Update();

	lifeItem->SetPos(this->pos.x + 6, this->pos.y + 4, &lifeItem->rect);

	return false;
}

bool Life::CleanUp()
{
	return false;
}

void Life::Draw()
{
	app->render->DrawTexture(lifeTex, pos.x, pos.y, &lifeAnim.GetCurrentFrame());
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
