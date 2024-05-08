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

        static b8 is_key_down(u32 key) ;
        static b8 is_key_pressed(u32 key);
        static b8 is_key_up(u32 key) ;

        static b8 is_button_down(Buttons button) ;
        static b8 is_button_up(Buttons button) ;

        static i16 get_xmouse() ;
        static i16 get_ymouse() ;
        static i16 get_mouse_wheel() ;

    protected:
        static Keyboard m_keyboard;
        static Keyboard m_ctrl;
        static Mouse m_mouse;
    };

        inline b8 Input::is_key_down(const u32 key)
        { return m_keyboard.keys[key]; }

        inline b8 Input::is_key_up(const u32 key)
        { return !m_keyboard.keys[key]; }

        inline i16 Input::get_xmouse()
        { return m_mouse.x; }

        inline i16 Input::get_ymouse()
        { return m_mouse.y; }

        inline i16 Input::get_mouse_wheel()
        { return m_mouse.wheel; }

        inline b8 Input::is_button_down(const Buttons button)
        { return m_mouse.buttons[button]; }

        inline b8 Input::is_button_up(const Buttons button)
        { return !m_mouse.buttons[button]; }
}

#endif // JOJ_INPUT_H
