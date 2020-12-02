#pragma once

#include "Enemy.h"

class Slime : public Enemy
{
public:

	Slime(iPoint position);
	~Slime();

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw() override;

	bool Collision(const char* side);

	bool CheckCollisionType(int idTile, std::string direction);

	void Gravity(float dt);

private:

	Collider* collider;
	SDL_Texture* tex = nullptr;
	Animation animLeft;
	Animation animRight;
	int deadFx = 0;
	uint speedX;
	uint speedY;
};