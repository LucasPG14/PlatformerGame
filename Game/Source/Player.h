#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "App.h"
#include "Render.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;
class Collider;

class Player : public Entity
{
public:
	Player(iPoint pos);
	
	bool Awake(pugi::xml_node& config);
	bool Update(float dt);
	void Draw();
	bool CleanUp();

	// Load and Save
	bool Load(pugi::xml_node& load);
	bool Save(pugi::xml_node& save) const;

	bool Collision(const char* side);

	void Gravity(float dt);
	bool CheckCollisionType(int idTile, SString direction);

	void Dead();
	void SwordAttack(float dt);
	void SpeedAnimations(float dt);
	void LoadAnimations();

	inline bool LevelFinished() { return levelFinished; }
	inline bool IsDead() { return deadPlayer; }
	inline iPoint GetPosition() { return position; }

	uint playerHurt;
	bool godMode;

	int time;

	int lifes;
	bool playerChangePos;

	// Checkpoint bool
	bool checkpoint;
	int whatCheckpoint;

	SDL_Texture* checkpointTex = nullptr;

	Animation checkpointAnim;

	bool deadPlayer;

private:
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

	uint swordFx;
	uint stepFx;

	uint checkpointAudio = -1;
	uint jumpAudio = -1;
};
#endif