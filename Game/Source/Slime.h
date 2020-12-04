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

	bool Collision(const char* side);

	bool CheckCollisionType(int idTile, std::string direction);

	void Gravity(float dt);

	// Load and Save
	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save) const;

private:

	//SDL_Texture* tex = nullptr;
	Animation animLeft;
	Animation animRight;
	int deadFx = 0;
	uint speedX;
	uint speedY;
};