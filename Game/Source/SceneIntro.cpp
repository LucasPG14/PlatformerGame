#include "SceneIntro.h"
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
	startBtn->SetObserver((Scene*)this);
	startBtn->state = GuiControlState::DISABLED;

	continueBtn = new GuiButton(2, { 440, 457, 400, 50 }, "CONTINUE");
	continueBtn->SetObserver((Scene*)this);
	continueBtn->state = GuiControlState::DISABLED;

	settingsBtn = new GuiButton(3, { 375, 533, 250, 50 }, "SETTINGS");
	settingsBtn->SetObserver((Scene*)this);
	settingsBtn->state = GuiControlState::DISABLED;

	creditsBtn = new GuiButton(4, { 655, 533, 250, 50 }, "CREDITS");
	creditsBtn->SetObserver((Scene*)this);
	creditsBtn->state = GuiControlState::DISABLED;

	exitBtn = new GuiButton(5, { 515, 601, 250, 50 }, "EXIT");
	exitBtn->SetObserver((Scene*)this);
	exitBtn->state = GuiControlState::DISABLED;
	
	musicVolumeSting = new GuiSlider(6, { 40, 141, 250, 25 }, "MUSIC VOLUME");
	musicVolumeSting->SetObserver((Scene*)this);
	musicVolumeSting->state = GuiControlState::DISABLED;

	fxVolumeSting = new GuiSlider(6, { 40, 224, 250, 25 }, "FX VOLUME");
	fxVolumeSting->SetObserver((Scene*)this);
	fxVolumeSting->state = GuiControlState::DISABLED;

	fullscreenSting = new GuiCheckBox(7, { 40,313,25,25 }, "FULLSCREEN");
	fullscreenSting->SetObserver((Scene*)this);
	fullscreenSting->state = GuiControlState::DISABLED;

	vsyncSting = new GuiCheckBox(8, { 40,399,25,25 }, "VSYNC");
	vsyncSting->SetObserver((Scene*)this);
	vsyncSting->state = GuiControlState::DISABLED;
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Load()
{
	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Backgrounds/background_intro.png");
	app->audio->PlayMusic("Assets/Audio/Music/twin_musicom_iron_is_laughter.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	exitRequest = false;
	settingsEnabled = false;

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	introAnim.speed = 5.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		TransitionToScene(SceneType::GAMEPLAY);
	}

	introAnim.Update();

	startBtn->state = GuiControlState::NORMAL;
	settingsBtn->state = GuiControlState::NORMAL;
	creditsBtn->state = GuiControlState::NORMAL;
	exitBtn->state = GuiControlState::NORMAL;
	startBtn->Update(app->input, dt);
	continueBtn->Update(app->input, dt);
	settingsBtn->Update(app->input, dt);
	creditsBtn->Update(app->input, dt);
	exitBtn->Update(app->input, dt);

	if (settingsEnabled)
	{
		musicVolumeSting->state = GuiControlState::NORMAL;
		fxVolumeSting->state = GuiControlState::NORMAL;
		fullscreenSting->state = GuiControlState::NORMAL;
		vsyncSting->state = GuiControlState::NORMAL;
		musicVolumeSting->Update(app->input, dt);
		fxVolumeSting->Update(app->input, dt);
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
		app->render->DrawRectangle({ 5, 13, 320, 694 }, 255, 255, 255, 160);
		musicVolumeSting->Draw(app->render);
		fxVolumeSting->Draw(app->render);
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

	startBtn->state = GuiControlState::DISABLED;
	continueBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::DISABLED;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::DISABLED;
	musicVolumeSting->state = GuiControlState::DISABLED;
	fxVolumeSting->state = GuiControlState::DISABLED;
	
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
		else if (control->id == 5)
		{
			exitRequest = true;
			return false;
		}
	}
	default: break;
	}

	exitRequest = false;
	return true;
}