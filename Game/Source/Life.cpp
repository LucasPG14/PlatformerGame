#include "Life.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Map.h"
#include "Audio.h"

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
	this->lifeItem = app->colliderManager->AddCollider({ this->pos.x + 11, this->pos.y, 20, 40 }, Collider::Type::LIFE);
	this->lifeItem->active = true;
	life = app->audio->LoadFx("Assets/Audio/Fx/life.wav");

	return true;
}

bool Life::Update(float dt)
{

	lifeAnim.speed = 2.0f * dt;

	lifeAnim.Update();

	if (this->lifeItem->active == false)
	{
		app->audio->PlayFx(life);
		app->enemyManager->RemoveEnemy(this);
	}

	return true;
}

bool Life::CleanUp()
{
	app->colliderManager->RemoveCollider(this->lifeItem);
	return true;
}

void Life::Draw()
{
	app->render->DrawTexture(this->texture, this->pos.x, this->pos.y, &this->lifeAnim.GetCurrentFrame());
}