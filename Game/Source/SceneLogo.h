#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

#include "Scenes.h"
#include "SDL/include/SDL.h"

#define LOGO_SPEED	1.0f

struct SDL_Texture;

class SceneLogo : public Scenes
{
public:
	SceneLogo();

	virtual ~SceneLogo();

	bool Load();

	bool Update(float dt);

	bool Draw();

	bool Unload();

private:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Rect logo;

	int state;
	float timer;
	float alpha;
};
#endif