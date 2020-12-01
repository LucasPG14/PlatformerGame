#ifndef __SCENE_WIN_H__
#define __SCENE_WIN_H__

#include "Scenes.h"
#include "Animation.h"

struct SDL_Texture;

class SceneWin : public Scenes
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

	int time = 0;
	uint winFx = 0;
};
#endif