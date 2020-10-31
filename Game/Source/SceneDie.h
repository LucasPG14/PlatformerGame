#ifndef __SCENE_DIE_H__
#define __SCENE_DIE_H__

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

class SceneDie : public Module
{
public:
	SceneDie();

	~SceneDie();


	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;

	Animation dieAnim;

};

#endif