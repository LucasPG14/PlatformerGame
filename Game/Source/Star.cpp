#include "Star.h"
#include "App.h"
#include "ColliderManagement.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Map.h"

Star::Star(iPoint position) : Enemy(position, EnemyType::STAR, 3)
{
	StarAnim.PushBack({ 4,8,24,40 });
	StarAnim.PushBack({ 30,8,19,40 });
	StarAnim.PushBack({ 50,8,12,40 });
	StarAnim.PushBack({ 63,8,14,40 });
	StarAnim.PushBack({ 78,8,22,40 });
	StarAnim.PushBack({ 101,8,14,40 });
	StarAnim.PushBack({ 116,8,12,40 });
	StarAnim.PushBack({ 129,8,20,40 });
	StarAnim.loop = true;

}

Star::~Star() {

}

bool Star::Start()
{
	StarTex = app->tex->Load("Assets/Textures/Characters/starAnim.png");
	StarItem = app->colliderManager->AddCollider({ this->pos.x + 4, this->pos.y + 3, 27, 17 }, Collider::Type::STAR);

	return false;
}

bool Star::Update(float dt)
{

	StarAnim.speed = 2.0f * dt;

	StarAnim.Update();

	StarItem->SetPos(this->pos.x + 6, this->pos.y + 4, &StarItem->rect);

	return false;
}

bool Star::CleanUp()
{
	return false;
}

void Star::Draw()
{
	app->render->DrawTexture(StarTex, pos.x, pos.y, &StarAnim.GetCurrentFrame());
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
