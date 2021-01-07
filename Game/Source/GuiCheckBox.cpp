#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
	this->checked = false;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
            {
				checked = !checked;
				NotifyObserver();
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 255);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);
        if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);

        if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 150, 150, 150, 150);
    } break;
    case GuiControlState::NORMAL:
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 25, 25, 25, 255);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);

        if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
        if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 255, 0, 150);
        break;
    case GuiControlState::FOCUSED:
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);

        if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
        if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 255, 150);
        break;
    case GuiControlState::PRESSED:

        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 255);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);

        if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
        if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 0, 150);
        break;
    case GuiControlState::SELECTED:

        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 255, 255, 255, 255);
        render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)) + 5, (int)(bounds.y + (-render->camera.y)) + 5, bounds.w - 10, bounds.h - 10 }, 116, 91, 45, 255);

        if (checked) render->DrawTexture(texture, (int)(bounds.x + (-render->camera.x)) + 4, (int)(bounds.y + (-render->camera.y)) + 4, &section);
        if (this->guiDebug == true) render->DrawRectangle({ (int)(bounds.x + (-render->camera.x)), (int)(bounds.y + (-render->camera.y)), bounds.w, bounds.h }, 0, 0, 255, 150);
        break;
    default:
        break;
    }

    return false;
}
