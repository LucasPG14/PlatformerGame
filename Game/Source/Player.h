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
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	int Collision();

	void Gravity();
	void Jump();

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

private:
	// Player texture
	SDL_Texture* player = nullptr;

	// Player gravity
	float gravity = 0.015f;

	// Jump mechanic
	bool jump = false;

	// Speed to run
	float speedX = 0.1f;
	float speedY = 0.0f;
};
#endif

