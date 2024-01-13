#ifndef JOJ_INPUT_H
#define JOJ_INPUT_H

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
}

#endif // JOJ_INPUT_H
