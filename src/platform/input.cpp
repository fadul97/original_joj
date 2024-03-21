#include "platform/input.h"

joj::Keyboard joj::Input::m_keyboard = { 0 };
joj::Keyboard joj::Input::m_ctrl = { 0 };
joj::Mouse joj::Input::m_mouse = { 0 };

joj::Input::Input()
{
}

joj::Input::~Input()
{
}


b8 joj::Input::is_key_pressed(u32 key)
{
    if (m_ctrl.keys[key])
    {
        if (is_key_down(key))
        {
            m_ctrl.keys[key] = false;
            return true;
        }
    }
    else if (is_key_up(key))
    {
        m_ctrl.keys[key] = true;
    }

    return false;
}
