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
class Collider;

class Position {
public:
	float x;
	float y;
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
	bool LevelFinished();
	bool IsDead();
	void SwordAttack();
	Position GetPosition();

	// Load and Save
	bool LoadState(pugi::xml_node& load);
	bool SaveState(pugi::xml_node& save) const;

	bool godMode;

	int time;

	bool deadPlayer;

	int lifes;
	int stars;
	int score;
	bool playerChangePos;

	// Player position
	Position position;

	// Checkpoint bool
	bool checkpoint;

	SDL_Texture* checkpointTex = nullptr;

	Animation checkpointAnim;

private:
	// Player texture
	SDL_Texture* player = nullptr;

	// Player gravity
	float gravity;

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

	Animation cooldownAtk;
	SDL_Texture* cooldownTex;

	// Jump mechanic
	bool jump;

	// Speed to run
	float speedX;
	float speedY;

	SString folder;
	SString playerString;

	bool jumping;

	bool levelFinished = false;

	bool sword = false;

	//Cooldown on attack
	int attackCooldown;

	Collider* playerCollider;
	Collider* swordCollider;

	uint stepFx;
	int yellowFont = -1;

	uint checkpointAudio = -1;
	uint jumpAudio = -1;

	//lifes
	SDL_Texture* lifesTex = nullptr;
	SDL_Texture* starTex = nullptr;
};
#endif