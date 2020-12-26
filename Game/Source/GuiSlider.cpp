#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->minValue = min;
    this->maxValue = max;
    this->slider = { bounds.x, bounds.y, 10, bounds.h };
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
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                slider.x = mouseX;
                if (slider.x < bounds.x) slider.x = bounds.x;
                if (slider.x + slider.w > bounds.x + bounds.w) slider.x = (bounds.x + bounds.w) - slider.w;
                Value();
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return true;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255 );
        break;
    case GuiControlState::NORMAL: 
        render->DrawRectangle(bounds, 0, 255, 0, 255 );
        render->DrawRectangle(slider, 0, 0, 0, 255 );
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255 );
        break;
    case GuiControlState::PRESSED: 
        render->DrawRectangle(bounds, 0, 255, 255, 255 );
        render->DrawRectangle(slider, 0, 0, 0, 255 );
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255 );
        break;
    default:
        break;
    }

    return true;
}

void GuiSlider::Value()
{
    int value = maxValue - minValue;

    this->value = value * slider.x / 100;

    if (this->value > maxValue) this->value = maxValue;
    else if (this->value < minValue) this->value = minValue;
}

int GuiSlider::GetValue()
{
    return this->value;
}