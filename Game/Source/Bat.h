#pragma once

#include "Enemy.h"
#include "DynArray.h"

class Bat : public Enemy
{
public:

	Bat(iPoint position);
	~Bat();

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	bool Collision(const char* side);

	bool CheckCollisionType(int idTile, std::string direction);

	void Draw() override;

	void Hit() override;

	bool FindGoal(Player* player) override;

	bool Move(float dt) override;

	bool Sleep(float dt) override;

	// Load and Save
	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save) const;

private:

	//SDL_Texture* tex = nullptr;
	Animation animLeft;
	Animation animRight;
	Animation hitLeftAnim;
	Animation hitRightAnim;
	Animation deathAnim;

	uint bat = -1;
	int deadFx = 0;
	uint speedMove;
	DynArray<iPoint> batPath;
	int indexBat;
};