#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

class SceneIntro : public Module
{
public:
	SceneIntro();

	~SceneIntro();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* logoTexture = nullptr;
	Animation introAnim;

	Animation* currentAnimation = nullptr;
	int time = 0;
	//Animation fAnim;
	//Animation* menuAnim = nullptr;

};

#endif