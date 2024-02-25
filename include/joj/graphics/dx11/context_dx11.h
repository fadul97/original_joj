#ifndef JOJ_DX11_CONTEXT_H
#define JOJ_DX11_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "graphics/context.h"
#include "platform/win32/window_win32.h"
#include <dxgi1_6.h>
#include <d3d11.h>
#include <dxgidebug.h>

namespace joj
{
    class JAPI DX11Context : public GraphicsContext<Win32Window>
    {
    public:
        DX11Context();
        ~DX11Context();

        b8 create(std::unique_ptr<Win32Window>& window) override;
        void make_current(std::unique_ptr<Win32Window>& window) override;
        void destroy() override;
          
    private:  
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_context;

        IDXGIFactory6* m_factory;
        ID3D11Debug* m_debug;

        void log_hardware_info() override;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_CONTEXT_H
