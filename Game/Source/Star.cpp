#include "Star.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Map.h"

Star::Star(iPoint position) : Enemy(position, EnemyType::STAR, 3)
{
	starAnim.PushBack({ 4,8,24,22 });
	starAnim.PushBack({ 4,35,24,22 });
	starAnim.PushBack({ 44,8,24,22 });
	starAnim.PushBack({ 44,35,24,22 });
	starAnim.PushBack({ 77,7,24,22 });
	starAnim.PushBack({ 76,35,24,22 });
	starAnim.PushBack({ 110,7,24,22 });
	starAnim.PushBack({ 110,36,24,22 });
	starAnim.loop = true;

}

Star::~Star() {

}

bool Star::Start()
{
	this->starTex = app->tex->Load("Assets/Textures/Characters/star_anim.png");
	this->starItem = app->colliderManager->AddCollider({ this->pos.x + 7, this->pos.y, 10, 22 }, Collider::Type::STAR);

	return false;
}

bool Star::Update(float dt)
{

	this->starAnim.speed = 2.0f * dt;

	this->starAnim.Update();

	if (this->starItem->active == false)
	{
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
