#pragma once

#include "Enemy.h"
#include "DynArray.h"

class Bat : public Enemy
{
public:

	Bat(iPoint position);
	~Bat();

	bool Update(float dt) override;

	bool Collision(const char* side);

	bool CheckCollisionType(int idTile, SString direction);

	void Hit() override;

	bool FindGoal(Player* player) override;

	bool Move(float dt) override;

	bool Sleep(float dt) override;

	// Load and Save
	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save) const;

private:

	Animation animLeft;
	Animation animRight;
	Animation hitLeftAnim;
	Animation hitRightAnim;
	Animation deathAnim;

	bool moveRight;
};