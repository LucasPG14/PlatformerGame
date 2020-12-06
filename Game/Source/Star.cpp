#include "Star.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"
#include "Audio.h"

Star::Star(iPoint position) : Enemy(position, EnemyType::STAR, 3)
{
	starAnim.PushBack({ 3,7,42,42 });
	starAnim.PushBack({ 49,7,42,42 });
	starAnim.PushBack({ 91,7,42,42 });
	starAnim.PushBack({ 122,7,42,42 });
	starAnim.PushBack({ 157,7,42,42 });
	starAnim.PushBack({ 199,7,42,42 });

	starAnim.loop = true;

	//shineAnim.PushBack({1219,13,39,39});
	//shineAnim.PushBack({2513,13,39,39});
	//shineAnim.PushBack({3807,13,39,39});
	//shineAnim.PushBack({5101,13,39,39});
	//shineAnim.PushBack({6395,13,39,39});
	//shineAnim.PushBack({7689,13,39,39});

	//starAnim.loop = true;
}

Star::~Star() {}

bool Star::Start()
{
	//this->shineTex = app->tex->Load("Assets/Textures/Characters/shine_anim.png");
	this->starItem = app->colliderManager->AddCollider({ this->pos.x + 6, this->pos.y + 6, 30, 30 }, Collider::Type::STAR);
	star = app->audio->LoadFx("Assets/Audio/Fx/star.wav");

	this->starItem->active = true;

	return false;
}

bool Star::Update(float dt)
{
	this->starAnim.speed = 2.0f * dt;

	this->starAnim.Update();

	if (this->starItem->active == false)
	{
		app->audio->PlayFx(star);
		app->enemyManager->RemoveEnemy(this);
	}
	return false;
}

bool Star::CleanUp()
{
	app->colliderManager->RemoveCollider(this->collider);
	return false;
}

void Star::Draw()
{
	app->render->DrawTexture(this->texture, this->pos.x, this->pos.y, &this->starAnim.GetCurrentFrame());
}