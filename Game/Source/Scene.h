#ifndef __SCENE_H__
#define __SCENE_H__

#include "Scenes.h"
#include "ColliderManagement.h"
#include "PugiXml\src\pugixml.hpp"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

struct SDL_Texture;

class Scene : public Scenes
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all updates
	bool Draw();

	// Called before quitting
	bool Unload();

	void LoadingEntities();

	void PlayerPosition();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	SDL_Texture* bg;
	SDL_Texture* bg2;
	SDL_Texture* bg3;
	SDL_Texture* lifesTex;
	SDL_Texture* starTex;

	GuiButton* resumeBtn;
	GuiButton* backToTitleBtn;
	GuiButton* settingsBtn;
	GuiButton* exitBtn;
	GuiSlider* musicVolumeSlider;
	GuiSlider* fxVolumeSlider;
	GuiCheckBox* fullscreenSting;
	GuiCheckBox* vsyncSting;
	GuiButton* backBtn;

	SDL_Texture* guiTexture;

	bool settingsEnabled;

	// Font
	int yellowFont = -1;

	bool exit;

	float time;

	Player* player;
};
#endif // __SCENE_H__