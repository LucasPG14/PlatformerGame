#ifndef __SCENE_DIE_H__
#define __SCENE_DIE_H__

#include "Scenes.h"
#include "Animation.h"


struct SDL_Texture;

class SceneDie : public Scenes
{
public:
	SceneDie();

	~SceneDie();


	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* starTex = nullptr;

	Animation dieAnim;

	int time = 0;
	uint dieFx = -1;
	int redFont = -1;
};
#endif