#include "SceneIntro.h"
#include "Window.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Animation.h"

#include "SDL/include/SDL_scancode.h"


SceneIntro::SceneIntro() : Scenes()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			introAnim.PushBack({ 1280 * j, 720 * i, 1280, 720 });
		}
	}
	introAnim.loop = false;

	startBtn = new GuiButton(1, {440, 386, 400, 50}, "START");
	startBtn->SetObserver(this);
	startBtn->state = GuiControlState::DISABLED;

	continueBtn = new GuiButton(2, { 440, 457, 400, 50 }, "CONTINUE");
	continueBtn->SetObserver(this);
	continueBtn->state = GuiControlState::DISABLED;

	settingsBtn = new GuiButton(3, { 375, 533, 250, 50 }, "SETTINGS");
	settingsBtn->SetObserver(this);
	settingsBtn->state = GuiControlState::DISABLED;

	creditsBtn = new GuiButton(4, { 655, 533, 250, 50 }, "CREDITS");
	creditsBtn->SetObserver(this);
	creditsBtn->state = GuiControlState::DISABLED;

	exitBtn = new GuiButton(5, { 515, 601, 250, 50 }, "EXIT");
	exitBtn->SetObserver(this);
	exitBtn->state = GuiControlState::DISABLED;
	
	musicVolumeSlider = new GuiSlider(6, { 40, 141, 250, 25 }, "MUSIC VOLUME", 0, 128);
	musicVolumeSlider->SetObserver(this);
	musicVolumeSlider->state = GuiControlState::DISABLED;

	fxVolumeSlider = new GuiSlider(7, { 40, 224, 250, 25 }, "FX VOLUME", 0, 128);
	fxVolumeSlider->SetObserver(this);
	fxVolumeSlider->state = GuiControlState::DISABLED;

	fullscreenSting = new GuiCheckBox(8, { 40,313,25,25 }, "FULLSCREEN");
	fullscreenSting->SetObserver(this);
	fullscreenSting->state = GuiControlState::DISABLED;

	vsyncSting = new GuiCheckBox(9, { 40,399,25,25 }, "VSYNC");
	vsyncSting->SetObserver(this);
	vsyncSting->state = GuiControlState::DISABLED;
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Load()
{
	bool ret = true;

	app->audio->PlayMusic("Assets/Audio/Music/silence.ogg");
	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_intro.png");
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_iron_is_laughter.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	exitRequest = false;
	settingsEnabled = false;

	startBtn->state = GuiControlState::NORMAL;
	settingsBtn->state = GuiControlState::NORMAL;
	creditsBtn->state = GuiControlState::NORMAL;
	exitBtn->state = GuiControlState::NORMAL;
	musicVolumeSlider->state = GuiControlState::NORMAL;
	fxVolumeSlider->state = GuiControlState::NORMAL;
	fullscreenSting->state = GuiControlState::NORMAL;
	vsyncSting->state = GuiControlState::NORMAL;

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	introAnim.speed = 5.0f * dt;

	introAnim.Update();

	startBtn->Update(app->input, dt);
	continueBtn->Update(app->input, dt);
	settingsBtn->Update(app->input, dt);
	creditsBtn->Update(app->input, dt);
	exitBtn->Update(app->input, dt);

	if (settingsEnabled)
	{
		musicVolumeSlider->Update(app->input, dt);
		fxVolumeSlider->Update(app->input, dt);
		fullscreenSting->Update(app->input, dt);
		vsyncSting->Update(app->input, dt);
	}

	if (exitRequest == true) ret = false;

	return ret;
}

// PostUpdate: draw background
bool SceneIntro::Draw()
{
	bool ret = true;

	// Draw everything 
	app->render->DrawTexture(bgTexture, 0, 0, &introAnim.GetCurrentFrame());
	startBtn->Draw(app->render);
	continueBtn->Draw(app->render);
	settingsBtn->Draw(app->render);
	creditsBtn->Draw(app->render);
	exitBtn->Draw(app->render);

	if (settingsEnabled)
	{
		app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, 720 }, 0, 0, 0, 100.0f);
		app->render->DrawRectangle({ 5, 13, 320, 694 }, 255, 255, 255, 160);
		musicVolumeSlider->Draw(app->render);
		fxVolumeSlider->Draw(app->render);
		fullscreenSting->Draw(app->render);
		vsyncSting->Draw(app->render);
	}

	return ret;
}

bool SceneIntro::Unload() 
{
	bool ret = true;

	app->audio->PlayMusic("Assets/Audio/Music/silence.ogg");

	app->tex->UnLoad(bgTexture);
	introAnim.Reset();

	delete startBtn;
	delete continueBtn;
	delete settingsBtn;
	delete creditsBtn;
	delete exitBtn;
	delete musicVolumeSlider;
	delete fxVolumeSlider;
	
	settingsEnabled = false;

	return true;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);
		else if (control->id == 2);
		else if (control->id == 3) settingsEnabled = !settingsEnabled;
		else if (control->id == 4);
		else if (control->id == 5) exitRequest = true;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 6) app->audio->SetMusicVolume(musicVolumeSlider->GetValue());
		else if (control->id == 7) app->audio->SetFxVolume(fxVolumeSlider->GetValue());
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 8) app->win->fullscreen = !app->win->fullscreen;
		else if (control->id == 9);
	}
	default: break;
	}

	return true;
}