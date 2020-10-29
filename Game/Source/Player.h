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
	int x, y;
};

class Player : public Module
{
public:
	Player();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	int CollisionVertical();
	int CollisionHorizontal();

	//PLAYER VARIABLES

	//PLAYER JUMP
	//void JumpMove();

	//PLAYER POSITION
	Position position;
	//PLAYER ANIMATIONS
	Animation* lastAnimation = nullptr;
	Animation* currentAnimation = nullptr;

	Animation idle;
	Animation jump;
	Animation run;
	Animation dead;

private:
	//PLAYER TEXTURE
	SDL_Texture* player = nullptr;

	//Player gravity
	float gravity = 0.3f;

	// Jump mechanic
	bool Jump = false;

	// Speed to run
	float speedX = 0.1f;
	float speedY = 10.0f;
};
#endif

