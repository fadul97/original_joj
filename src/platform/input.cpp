#include "platform/input.h"

joj::Input::Input()
{
    keyboard = { 0 };
    ctrl = { 0 };
    mouse = { 0 };
}

joj::Input::~Input()
{
}


b8 joj::Input::is_key_pressed(Keys key)
{
    if (ctrl.keys[key])
    {
        if (is_key_down(key))
        {
            ctrl.keys[key] = false;
            return true;
        }
    }
    else if (is_key_up(key))
    {
        ctrl.keys[key] = true;
    }

    return false;
}
