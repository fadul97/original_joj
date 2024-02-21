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

        b8 is_key_down(Keys key) const;
        b8 is_key_pressed(Keys key);
        b8 is_key_up(Keys key) const;

        i16 get_xmouse() const;
        i16 get_ymouse() const;
        i16 get_mouse_wheel() const;

        void move_mouse(i16 x, i16 y);
        void press_key(Keys key);
        void click_button(Buttons button);

        void release_key(Keys key);
        void release_button(Buttons button);

        virtual Keys translate_keycode(u32 keycode) = 0;
    
    private:
        Keyboard keyboard;
        Keyboard ctrl;
        Mouse mouse;
    };

        inline b8 Input::is_key_down(Keys key) const
        { return keyboard.keys[key]; }

        inline b8 Input::is_key_up(Keys key) const
        { return !keyboard.keys[key]; }

        inline i16 Input::get_xmouse() const
        { return mouse.x; }

        inline i16 Input::get_ymouse() const
        { return mouse.y; }

        inline i16 Input::get_mouse_wheel() const
        { return mouse.wheel; }

        inline void Input::move_mouse(i16 x, i16 y)
        { mouse.x = x; mouse.y = y; }

        inline void Input::press_key(Keys key)
        { keyboard.keys[key] = true; }

        inline void Input::click_button(Buttons button)
        { mouse.buttons[button] = true; }

        inline void Input::release_key(Keys key)
        { keyboard.keys[key] = false; }

        inline void Input::release_button(Buttons button)
        { mouse.buttons[button] = false; }
}

#endif // JOJ_INPUT_H
