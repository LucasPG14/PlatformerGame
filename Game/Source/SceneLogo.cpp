#include "SceneLogo.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"

SceneLogo::SceneLogo()
{
	logo = { 640 - 150, 180, 300, 300 };

	state = 0;
	timer = 0.0f;
	alpha = 0.0f;
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load()
{
	texture = app->tex->Load("Assets/Textures/Backgrounds/logo_real_ambient.png");

	return true;
}

bool SceneLogo::Update(float dt)
{
	if (state == 0) state = 1;
	else if (state == 1)
	{
		alpha += (LOGO_SPEED * dt);

		if (alpha > 1.0f)
		{
			alpha = 1.0f;
			state = 2;
		}
	}
	else if (state == 2)
	{
		timer += dt;
		if (timer >= 3.0f) state = 3;
	}
	else if (state == 3)
	{
		alpha -= (LOGO_SPEED * dt);

		if (alpha < 0.0f)
		{
			alpha = 0.0f;
			TransitionToScene(SceneType::TITLE);
		}
	}

	return true;
}

bool SceneLogo::Draw()
{
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 100, 200, 200, 255);

	app->render->DrawTexture(texture, 250, 10, NULL);

	return true;
}

bool SceneLogo::Unload()
{
	app->tex->UnLoad(texture);
	return true;
}