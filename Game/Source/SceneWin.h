#ifndef __SCENE_WIN_H__
#define __SCENE_WIN_H__

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

class SceneWin : public Module
{
public:
	SceneWin();

	~SceneWin();


	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;

	Animation winAnim;

	int time;
	uint winFx;

};

#endif