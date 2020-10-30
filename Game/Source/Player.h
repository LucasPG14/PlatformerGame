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

	void Gravity();
	void Jump();

	bool checkCollisionType(int idTile);

	void Dead();
	// Load and Save
	bool LoadState(pugi::xml_node& load);
	bool SaveState(pugi::xml_node& save) const;

	// Player position

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
	Animation deadAnim;

	bool godMode;

private:
	// Player texture
	SDL_Texture* player = nullptr;

	// Player gravity
	float gravity;

	// Jump mechanic
	bool jump;

	// Speed to run
	float speedX;
	float speedY;

	SString folder;
	SString playerString;
};
#endif

