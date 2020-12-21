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

	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

public:
	
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* starTex = nullptr;

	Animation winAnim;
	int yellowFont = -1;
	int time = 0;
	uint winFx = 0;
};
#endif