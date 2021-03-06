#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->minValue = min;
	this->maxValue = max;
	this->slider = { bounds.x + 5, bounds.y, 20, bounds.h - 10 };
	audioFx = false;
	clickFx = false;
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/mouse_click.wav");
	hoverFx = app->audio->LoadFx("Assets/Audio/Fx/mouse_hover.wav");
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				slider.x = mouseX;
				if (slider.x < bounds.x + 5) slider.x = bounds.x + 5;
				if (slider.x + slider.w > bounds.x + bounds.w - 5) slider.x = (bounds.x + bounds.w) - slider.w - 5;
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;

		Value();
	}

	return true;
}

bool GuiSlider::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 255);
		render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);
		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);

		if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);
		break;
	case GuiControlState::NORMAL:

		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
		render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);

		if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 255, 0, 150);
		audioFx = false;
		clicked = false;
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
		render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);

		if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 255, 150);
	   
		if (audioFx == false)
		{
			audioFx = true;
			app->audio->PlayFx(hoverFx);
		}
		break;
	case GuiControlState::PRESSED:

		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 255);
		render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 203, 155, 27, 255);

		if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 150);
		
		if (clicked == false)
		{
			clicked = true;
			app->audio->PlayFx(clickFx);
		}
		break;
	case GuiControlState::SELECTED:

		render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
		render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, &section);
		render->DrawRectangle({ (int)(slider.x + (-render->camera.x)), (int)(slider.y + (-render->camera.y)) + 5, slider.w, slider.h }, 41, 19, 8, 255);

		if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 255, 150);
		break;
	default:
		break;
	}

	return true;
}

void GuiSlider::Value()
{
	this->value = ((slider.x + app->render->offset.x) - bounds.x)/2;

	if (this->bounds.x + 5 == this->slider.x) this->value = 0;
}

int GuiSlider::GetValue()
{
	return this->value;
}