#ifndef JOJ_INPUT_H
#define JOJ_INPUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "keys.h"

namespace joj
{
    class JAPI Input
    {
    public:
        Input();
        virtual ~Input();

        b8 is_key_down(u32 key) const;
        b8 is_key_pressed(u32 key);
        b8 is_key_up(u32 key) const;

        b8 is_button_down(Buttons button) const;
        b8 is_button_up(Buttons button) const;

        i16 get_xmouse() const;
        i16 get_ymouse() const;
        i16 get_mouse_wheel() const;

        void move_mouse(i16 x, i16 y);
        void press_key(Keys key);
        void click_button(Buttons button);

        void release_key(u32 key);
        void release_button(Buttons button);

    protected:
        static Keyboard m_keyboard;
        static Keyboard m_ctrl;
        static Mouse m_mouse;
    };

        inline b8 Input::is_key_down(u32 key) const
        { return m_keyboard.keys[key]; }

        inline b8 Input::is_key_up(u32 key) const
        { return !m_keyboard.keys[key]; }

        inline i16 Input::get_xmouse() const
        { return m_mouse.x; }

        inline i16 Input::get_ymouse() const
        { return m_mouse.y; }

        inline i16 Input::get_mouse_wheel() const
        { return m_mouse.wheel; }

        inline void Input::move_mouse(i16 x, i16 y)
        { m_mouse.x = x; m_mouse.y = y; }

        inline void Input::press_key(Keys key)
        { m_keyboard.keys[key] = true; }

        inline void Input::click_button(Buttons button)
        { m_mouse.buttons[button] = true; }

        inline void Input::release_key(u32 key)
        { m_keyboard.keys[key] = false; }

        inline void Input::release_button(Buttons button)
        { m_mouse.buttons[button] = false; }

        inline b8 Input::is_button_down(Buttons button) const
        { return m_mouse.buttons[button]; }

        inline b8 Input::is_button_up(Buttons button) const
        { return !m_mouse.buttons[button]; }
}

#endif // JOJ_INPUT_H
