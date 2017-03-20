#include "InputHandler.h"

#include <iostream>

/* Initialises the Keyboard and Mouse devices based on window instance information
 * passed in through Application & Game classes.
 */
InputHandler::InputHandler(HWND& _hWnd, HINSTANCE& _hInstance)
    : direct_input(nullptr)
    , keyboard(nullptr)
    , mouse(nullptr)
{
    HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&direct_input, NULL);
    hr = direct_input->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
    hr = keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hr = direct_input->CreateDevice(GUID_SysMouse, &mouse, NULL);
    hr = mouse->SetDataFormat(&c_dfDIMouse);
    hr = mouse->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}


/* Cleans up any dynamically allocated memory during the creation of
 * input device objects.
 */
InputHandler::~InputHandler()
{
    if (keyboard)
    {
        keyboard->Unacquire();
        keyboard->Release();
    }

    if (mouse)
    {
        mouse->Unacquire();
        mouse->Release();
    }

    if (direct_input)
    {
        direct_input->Release();
    }
}


/* Should be called once per update cycle.
 * Polls the registered Keyboard and Mouse devices for input and fills
 * the respective data arrays for each device.
 */
void InputHandler::tick()
{
    read_keyboard();
    read_mouse();
}


/* Check if a key is pressed. Returns true as long as the key is pressed.
 */
bool InputHandler::get_key(int _dikb) const
{
    if (keyboard_state[_dikb] & 0x80)
    {
        return true;
    }

    return false;
}


/* Check if a key has just been pressed.
 * Returns true the first time it was pressed, otherwise returns false.
 */
bool InputHandler::get_key_down(int _dikb) const
{
    if ((keyboard_state[_dikb] & 0x80) && !(prev_keyboard_state[_dikb] & 0x80))
    {
        return true;
    }

    return false;
}


/* Check if a key has just been released.
 * Returns true the first time it was released, otherwise returns false.
 */
bool InputHandler::get_key_up(int _dikb) const
{
    if (!(keyboard_state[_dikb] & 0x80) && prev_keyboard_state[_dikb] & 0x80)
    {
        return true;
    }

    return false;
}

/* Check if a mouse button is pressed. Returns true as long as the button is pressed.
 */
bool InputHandler::get_mouse_button(int _button) const
{
    if (mouse_state.rgbButtons[_button] & 0x80)
    {
        return true;
    }

    return false;
}

/* Check if a mouse button has just been pressed.
 * Returns true the first time it was pressed, otherwise returns false.
 */
bool InputHandler::get_mouse_button_down(int _button) const
{
    if ((mouse_state.rgbButtons[_button] & 0x80) && !(prev_mouse_state.rgbButtons[_button] & 0x80))
    {
        return true;
    }

    return false;
}

/* Check if a mouse button has just been released.
 * Returns true the first time it was released, otherwise returns false.
 */
bool InputHandler::get_mouse_button_up(int _button) const
{
    if (!(mouse_state.rgbButtons[_button] & 0x80) && (prev_mouse_state.rgbButtons[_button] & 0x80))
    {
        return true;
    }

    return false;
}

DIMOUSESTATE& InputHandler::get_mouse_state()
{
    return mouse_state;
}

/* Assimilates the current keyboard state by reading into the keyboard_state array.
 * The prev_keyboard_state array keeps track of the previous keyboard state.
 */
bool InputHandler::read_keyboard()
{
    // Copy previous keyboard state.
    memcpy(prev_keyboard_state, keyboard_state, sizeof(unsigned char) * 256);

    // Clear out previous state.
    ZeroMemory(&keyboard_state, sizeof(keyboard_state));

    // Read the keyboard device.
    HRESULT hr = keyboard->GetDeviceState(sizeof(keyboard_state), (LPVOID)&keyboard_state);

    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
        {
            keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}


/* Assimilates the current mouse state by reading into the Direct Input mouse state.
 * The prev_mouse_state value keeps track of the previous mouse state.
 */
bool InputHandler::read_mouse()
{
    // Copy previous mouse state.
    prev_mouse_state = mouse_state;

    // Clear out previous state.
    ZeroMemory(&mouse_state, sizeof(mouse_state));

    // Read the mouse device.
    HRESULT hr = mouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);

    if (FAILED(hr))
    {
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
        {
            mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}
