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
	starAnim.PushBack({ 7,7,42,42 });
	starAnim.PushBack({ 63,7,37,42 });
	starAnim.PushBack({ 114,7,20,42 });
	starAnim.PushBack({ 148,7,6,42 });
	starAnim.PushBack({ 168,7,20,42 });
	starAnim.PushBack({ 202,7,37,42 });

	starAnim.loop = true;

	//shineAnim.PushBack({1219,13,39,39});
	//shineAnim.PushBack({2513,13,39,39});
	//shineAnim.PushBack({3807,13,39,39});
	//shineAnim.PushBack({5101,13,39,39});
	//shineAnim.PushBack({6395,13,39,39});
	//shineAnim.PushBack({7689,13,39,39});

	//starAnim.loop = true;


}

Star::~Star() {

}

bool Star::Start()
{
	this->starTex = app->tex->Load("Assets/Textures/Characters/star_anim.png");
	//this->shineTex = app->tex->Load("Assets/Textures/Characters/shineAnim.png");
	this->starItem = app->colliderManager->AddCollider({ this->pos.x + 7, this->pos.y, 10, 22 }, Collider::Type::STAR);
	star = app->audio->LoadFx("Assets/Audio/Fx/star.wav");

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
	return false;
}

void Star::Draw()
{
	app->render->DrawTexture(this->starTex, this->pos.x, this->pos.y, &this->starAnim.GetCurrentFrame());



}

bool Star::Collision(const char* side)
{
	return false;
}

bool Star::CheckCollisionType(int idTile, std::string direction)
{
	return false;
}

void Star::Gravity(float dt)
{
}
