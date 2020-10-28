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

class position
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

	//PLAYER VARIABLES

	//PLAYER JUMP
	//void JumpMove();

	//PLAYER POSITION
	position Position;
	//PLAYER ANIMATIONS
	Animation* lastanimation = nullptr;
	Animation* currentAnimation = nullptr;

	Animation Static;
	Animation Jump;
	Animation Run;
	Animation Dead;

private:
	//PLAYER TEXTURE
	SDL_Texture* player = nullptr;

};
#endif

