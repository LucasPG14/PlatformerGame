#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Scenes.h"
#include "Animation.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

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

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	
	Animation introAnim;

	bool exitRequest;
	bool settingsEnabled;
	
	SDL_Texture* guiTexture = nullptr;
	GuiButton* startBtn;
	GuiButton* continueBtn;
	GuiButton* settingsBtn;
	GuiButton* creditsBtn;
	GuiButton* exitBtn;
	GuiSlider* musicVolumeSlider;
	GuiSlider* fxVolumeSlider;
	GuiCheckBox* fullscreenSting;
	GuiCheckBox* vsyncSting;
};
#endif