#include "Input.h"

#include "Core/ECSSystem.h"

bool Input::is_key_held(SDL_Keycode key) { return pressed_keys.contains(key); }

bool Input::is_key_pressed(SDL_Keycode key) { return pressed_keys_momentary.contains(key); }

bool Input::is_key_released(SDL_Keycode key) { return released_keys_momentary.contains(key); }

bool Input::is_mouse_button_held(MouseButton button) { return pressed_mouse.contains(button); }

bool Input::is_mouse_button_pressed(MouseButton button) { return pressed_mouse_momentary.contains(button); }

bool Input::is_mouse_button_released(MouseButton button) { return released_mouse_momentary.contains(button); }

void Input::process_input_event(EventType type, const SDL_Event& event)
{
    switch (type)
    {
        case EventType::KeyPressed: {
            pressed_keys.insert(event.key.keysym.sym);
            pressed_keys_momentary.insert(event.key.keysym.sym);
        }
        break;

        case EventType::KeyReleased: {
            pressed_keys.erase(event.key.keysym.sym);
            released_keys_momentary.insert(event.key.keysym.sym);
        }
        break;

        case EventType::MousePressed: {
            pressed_mouse.insert(static_cast<MouseButton>(event.button.button));
            pressed_mouse_momentary.insert(static_cast<MouseButton>(event.button.button));
        }
        break;

        case EventType::MouseReleased: {
            pressed_mouse.erase(static_cast<MouseButton>(event.button.button));
            released_mouse_momentary.insert(static_cast<MouseButton>(event.button.button));
        }
        break;
    }
}

void Input::update_momentary_keys()
{
    pressed_keys_momentary.clear();
    released_keys_momentary.clear();
    pressed_mouse_momentary.clear();
    released_mouse_momentary.clear();
}
