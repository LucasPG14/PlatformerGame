#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Scenes.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Scenes
{
public:
	SceneIntro();

	~SceneIntro();

	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

private:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* logoTexture = nullptr;
	
	Animation introAnim;

	int time = 0;
};
#endif