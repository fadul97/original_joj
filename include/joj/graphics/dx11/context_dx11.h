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
#include <wrl/client.h> // Include the ComPtr header

namespace joj
{
    class JAPI DX11Context : public GraphicsContext<Win32Window>
    {
    public:
        DX11Context();
        ~DX11Context();

        Microsoft::WRL::ComPtr<ID3D11Device> get_device() const;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> get_device_context() const;

        IDXGIFactory6* get_factory() const;
        ID3D11Debug* get_debug() const;

        b8 create(std::unique_ptr<Win32Window>& window) override;
        void make_current(std::unique_ptr<Win32Window>& window) override;
        void destroy() override;
          
    private:  
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;

        IDXGIFactory6* m_factory;
        ID3D11Debug* m_debug;

        void log_hardware_info() override;
    };
    
    inline Microsoft::WRL::ComPtr<ID3D11Device> DX11Context::get_device() const
    { return m_device; }
    
    inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> DX11Context::get_device_context() const
    { return m_device_context; }

    inline IDXGIFactory6* DX11Context::get_factory() const
    { return m_factory; }

    inline ID3D11Debug* DX11Context::get_debug() const
    { return m_debug; }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_CONTEXT_H
