#ifndef __LIFE_H__
#define __LIFE_H__
#include "Animation.h"
#include "Enemy.h"

class Life : public Enemy
{
public:

	Life(iPoint position);
	~Life();
	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw() override;

	bool Collision(const char* side);

	bool CheckCollisionType(int idTile, std::string direction);

	void Gravity(float dt);

private:

	Collider* lifeItem = nullptr;
	Animation lifeAnim;
	SDL_Texture* lifeTex;
};
#endif
