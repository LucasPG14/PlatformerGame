#include "SceneIntro.h"

#include "Window.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Player.h"
#include "SceneManager.h"
#include "Render.h"
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
	startBtn->section = { 450, 0, 390, 40 };

	continueBtn = new GuiButton(2, { 440, 457, 400, 50 }, "CONTINUE");
	continueBtn->SetObserver(this);
	continueBtn->state = GuiControlState::DISABLED;
	continueBtn->section = { 450,40,390,40 };

	settingsBtn = new GuiButton(3, { 375, 533, 250, 50 }, "SETTINGS");
	settingsBtn->SetObserver(this);
	settingsBtn->state = GuiControlState::DISABLED;
	settingsBtn->section = { 450, 80, 240, 40 };

	creditsBtn = new GuiButton(4, { 655, 533, 250, 50 }, "CREDITS");
	creditsBtn->SetObserver(this);
	creditsBtn->state = GuiControlState::DISABLED;
	creditsBtn->section = { 450, 120, 240, 40 };

	exitBtn = new GuiButton(5, { 515, 601, 250, 50 }, "EXIT");
	exitBtn->SetObserver(this);
	exitBtn->state = GuiControlState::DISABLED;
	exitBtn->section = { 450, 160, 240, 40 };
	
	musicVolumeSlider = new GuiSlider(6, { 601, 417, 250, 25 }, "MUSIC VOLUME", 0, 128);
	musicVolumeSlider->SetObserver(this);
	musicVolumeSlider->state = GuiControlState::DISABLED;
	musicVolumeSlider->section = { 450, 200, 240, 15 };

	fxVolumeSlider = new GuiSlider(7, { 601, 453, 250, 25 }, "FX VOLUME", 0, 128);
	fxVolumeSlider->SetObserver(this);
	fxVolumeSlider->state = GuiControlState::DISABLED;
	fxVolumeSlider->section = { 450, 200, 240, 15 };

	fullscreenSting = new GuiCheckBox(8, { 826, 489, 25, 25 }, "FULLSCREEN");
	fullscreenSting->SetObserver(this);
	fullscreenSting->state = GuiControlState::DISABLED;
	fullscreenSting->section = { 729, 80, 17, 17 };

	vsyncSting = new GuiCheckBox(9, { 826, 525, 25, 25 }, "VSYNC");
	vsyncSting->SetObserver(this);
	vsyncSting->state = GuiControlState::DISABLED;
	vsyncSting->section = { 729, 80, 17, 17 };

	backBtn = new GuiButton(10, { 801, 561, 50, 50 }, "BACK");
	backBtn->SetObserver(this);
	backBtn->state = GuiControlState::DISABLED;
	backBtn->section = { 689, 80, 40, 40 };

	backCreditsBtn = new GuiButton(11, { 430, 75, 50, 50 }, "BACK");
	backCreditsBtn->SetObserver(this);
	backCreditsBtn->state = GuiControlState::DISABLED;
	backCreditsBtn->section = { 689, 80, 40, 40 };
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
	app->render->offset.x = 0;
	app->render->offset.y = 0;

	guiTexture = app->tex->Load("Assets/Hud/intro_menu.png");
	creditsTexture = app->tex->Load("Assets/Hud/credits.png");

	exitRequest = false;
	settingsEnabled = false;	
	creditsEnabled = false;
	
	startBtn->texture = guiTexture;
	startBtn->state = GuiControlState::NORMAL;
	continueBtn->texture = guiTexture;
	settingsBtn->texture = guiTexture;
	settingsBtn->state = GuiControlState::NORMAL;
	creditsBtn->texture = guiTexture;
	creditsBtn->state = GuiControlState::NORMAL;
	exitBtn->texture = guiTexture;
	exitBtn->state = GuiControlState::NORMAL;
	musicVolumeSlider->texture = guiTexture;
	fxVolumeSlider->texture = guiTexture;
	fullscreenSting->texture = guiTexture;
	vsyncSting->texture = guiTexture;
	backBtn->texture = guiTexture;
	backCreditsBtn->texture = guiTexture;

	app->LoadGameRequest();

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	introAnim.speed = 5.0f * dt;

	introAnim.Update();

	if (app->sceneManager->saved) continueBtn->state = GuiControlState::NORMAL;
	
	if (!settingsEnabled && !creditsEnabled)
	{
		startBtn->Update(app->input, dt);
		continueBtn->Update(app->input, dt);
		settingsBtn->Update(app->input, dt);
		creditsBtn->Update(app->input, dt);
		exitBtn->Update(app->input, dt);
	}

	if (settingsEnabled)
	{
		musicVolumeSlider->state = GuiControlState::NORMAL;
		fxVolumeSlider->state = GuiControlState::NORMAL;
		fullscreenSting->state = GuiControlState::NORMAL;
		vsyncSting->state = GuiControlState::NORMAL;
		backBtn->state = GuiControlState::NORMAL;
		musicVolumeSlider->Update(app->input, dt);
		fxVolumeSlider->Update(app->input, dt);
		fullscreenSting->Update(app->input, dt);
		vsyncSting->Update(app->input, dt);
		backBtn->Update(app->input, dt);
	}

	else if (creditsEnabled)
	{
		backCreditsBtn->state = GuiControlState::NORMAL;
		backCreditsBtn->Update(app->input, dt);
	}

	else
	{
		musicVolumeSlider->state = GuiControlState::DISABLED;
		fxVolumeSlider->state = GuiControlState::DISABLED;
		fullscreenSting->state = GuiControlState::DISABLED;
		vsyncSting->state = GuiControlState::DISABLED;
		backBtn->state = GuiControlState::DISABLED;
		backCreditsBtn->state = GuiControlState::DISABLED;
	}

	if (exitRequest == true) ret = false;

	return ret;
}

// PostUpdate: draw background
bool SceneIntro::Draw()
{
	bool ret = true;

	// Draw everything 
	app->render->DrawTexture(bgTexture, -app->render->camera.x, -app->render->camera.y, &introAnim.GetCurrentFrame());
	
	if (!settingsEnabled && !creditsEnabled)
	{
		startBtn->Draw(app->render);
		continueBtn->Draw(app->render);
		settingsBtn->Draw(app->render);
		creditsBtn->Draw(app->render);
		exitBtn->Draw(app->render);
	}

	if (settingsEnabled)
	{
		SDL_Rect rect = { 0, 0, 450, 250 };
		app->render->DrawRectangle({ (int)app->render->offset.x - 1, (int)app->render->offset.y, 1280, 720 }, 0, 0, 0, 100.0f);
		app->render->DrawTexture(guiTexture, (int)(app->render->offset.x + 415), (int)(app->render->offset.y + 370), &rect);
		musicVolumeSlider->Draw(app->render);
		fxVolumeSlider->Draw(app->render);
		fullscreenSting->Draw(app->render);
		vsyncSting->Draw(app->render);
		backBtn->Draw(app->render);
	}

	if (creditsEnabled)
	{
		SDL_Rect rect = { 0, 0, 450,600 };
		app->render->DrawRectangle({ (int)app->render->offset.x - 1, (int)app->render->offset.y, 1280, 720 }, 0, 0, 0, 100.0f);
		app->render->DrawTexture(creditsTexture, (int)(app->render->offset.x + 415), (int)(app->render->offset.y + 60), &rect);
		backCreditsBtn->Draw(app->render);
	}

	if (app->sceneManager->guiDebug == true)
	{
		startBtn->guiDebug = true;
		continueBtn->guiDebug = true;
		settingsBtn->guiDebug = true;
		creditsBtn->guiDebug = true;
		exitBtn->guiDebug = true;
		musicVolumeSlider->guiDebug = true;
		fxVolumeSlider->guiDebug = true;
		fullscreenSting->guiDebug = true;
		vsyncSting->guiDebug = true;
		backBtn->guiDebug = true;
		backCreditsBtn->guiDebug = true;
	}
	else
	{
		startBtn->guiDebug = false;
		continueBtn->guiDebug = false;
		settingsBtn->guiDebug = false;
		creditsBtn->guiDebug = false;
		exitBtn->guiDebug = false;
		musicVolumeSlider->guiDebug = false;
		fxVolumeSlider->guiDebug = false;
		fullscreenSting->guiDebug = false;
		vsyncSting->guiDebug = false;
		backBtn->guiDebug = false;
		backCreditsBtn->guiDebug = false;
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
	delete fullscreenSting;
	delete vsyncSting;
	delete backBtn;
	delete backCreditsBtn;
	app->tex->UnLoad(guiTexture);
	app->tex->UnLoad(creditsTexture);

	settingsEnabled = false;	
	creditsEnabled = false;

	return true;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::LEVEL_1);
		else if (control->id == 2)
		{
			TransitionToScene(SceneType::LEVEL_1);
			app->sceneManager->continueClicked = true;
		}
		else if (control->id == 3) settingsEnabled = !settingsEnabled;
		else if (control->id == 4) creditsEnabled = !creditsEnabled;
		else if (control->id == 5) exitRequest = true;
		else if (control->id == 10) settingsEnabled = !settingsEnabled;
		else if (control->id == 11) creditsEnabled = !creditsEnabled;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 6) app->audio->SetMusicVolume(musicVolumeSlider->GetValue());
		else if (control->id == 7) app->audio->SetFxVolume(fxVolumeSlider->GetValue());
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 8) app->win->fullscreen = !app->win->fullscreen;
		else if (control->id == 9) app->render->vsync = !app->render->vsync;
	}
	default: break;
	}

	return true;
}