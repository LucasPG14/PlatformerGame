#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Animation.h"
#include "Module.h"

struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;

class Position
{
public:
	float x, y;
};

class Player : public Module
{
public:
	Player();
	
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Collision(const char* side);

	void Gravity(float dt);
	void Jump(float dt);
	bool CheckCollisionType(int idTile, std::string direction);

	void Dead();
	void ResetPlayer();
	bool LevelFinished();
	bool IsDead();
	Position GetPosition();

	// Load and Save
	bool LoadState(pugi::xml_node& load);
	bool SaveState(pugi::xml_node& save) const;

	bool godMode;

	int time;

	bool deadPlayer;

private:
	// Player texture
	SDL_Texture* player = nullptr;

	// Player gravity
	float gravity;

	// Player position
	Position position;

	// Player animations
	Animation* lastAnimation = nullptr;
	Animation* currentAnimation = nullptr;

	Animation leftIdleAnim;
	Animation rightIdleAnim;
	Animation leftJumpAnim;
	Animation rightJumpAnim;
	Animation leftRunAnim;
	Animation rightRunAnim;
	Animation leftDeadAnim;
	Animation rightDeadAnim;
	Animation leftAttackAnim;
	Animation rightAttackAnim;

	// Jump mechanic
	bool jump;

	// Speed to run
	float speedX;
	float speedY;

	SString folder;
	SString playerString;

	bool jumping;

	bool levelFinished = false;

	//Cooldown on attack
	int attackCooldown;

	uint stepFx;
};
#endif