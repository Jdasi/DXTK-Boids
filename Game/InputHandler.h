#pragma once
#include <dinput.h>

/* Utility/helper class to decouple Game from Input handling.
 * Like other manager classes, InputHandler needs to be ticked for its
 * functionality to be properly updated.
 * InputHandler contains all DirectInput devices used for input, and provides
 * streamlined functions for detecting specific key presses.
 */
class InputHandler
{
public:
    InputHandler(HWND& _hWnd, HINSTANCE& _hInstance);
    ~InputHandler();

    void tick();

    bool get_key(int _dikb) const;
    bool get_key_down(int _dikb) const;
    bool get_key_up(int _dikb) const;

    DIMOUSESTATE& get_mouse_state();
    bool get_mouse_button(int _button) const;
    bool get_mouse_button_down(int _button) const;
    bool get_mouse_button_up(int _button) const;

private:
    bool read_keyboard();
    bool read_mouse();

    IDirectInput8* direct_input;
    IDirectInputDevice8* keyboard;
    IDirectInputDevice8* mouse;

    unsigned char keyboard_state[256];
    unsigned char prev_keyboard_state[256];
    DIMOUSESTATE mouse_state;
    DIMOUSESTATE prev_mouse_state;

};
