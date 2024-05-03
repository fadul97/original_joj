#include "platform/context/dx11/context_dx11.h"

#include <stdio.h>

#include <winuser.h>

joj::DX11Context::DX11Context()
{
    m_device = nullptr;
    m_device_context = nullptr;
    m_factory = nullptr;
    m_debug = nullptr;
}

joj::DX11Context::~DX11Context()
{
    // Release factory
    if (m_factory)
        m_factory->Release();
}

b8 joj::DX11Context::create(std::unique_ptr<Win32Window>& window)
{
    // ---------------------------------------------------
    // Create DXGI infrastructure and D3D device
    // ---------------------------------------------------

    u32 factory_flags = 0;
    u32 create_device_flags = 0;

    factory_flags = DXGI_CREATE_FACTORY_DEBUG;

#ifdef _DEBUG
    create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    if (CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory)) != S_OK)
    {
        // TODO: Use own logger and return value
        printf("Failed to CreateDXGIFactory2.\n");
        return false;
    }
    
    // Level of D3D features supported by hardware
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    if (D3D11CreateDevice(
        NULL,                            // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,        // D3D driver type (Hardware, Reference or Software)
        NULL,                            // Pointer to software rasterizer - No software device
        create_device_flags,             // Debug or Normal Mode
        NULL,                            // Default feature level (NULL = max supported)
        0,                               // Size of feature level array
        D3D11_SDK_VERSION,               // Direct3D SDK version
        m_device.GetAddressOf(),         // Stores D3D device created
        &feature_level,                  // Current Direct3D version in use,
        m_device_context.GetAddressOf()) // D3D context device
        != S_OK)
    {
        if (D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
            NULL, create_device_flags, NULL, 0, D3D11_SDK_VERSION,
            m_device.GetAddressOf(), &feature_level, m_device_context.GetAddressOf()) != S_OK)
        {
            // TODO: Use own logger and return value
            printf("Failed to create device, using WARP Adapter.\n");
        }
    }

#if defined _DEBUG
    if (m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_debug) != S_OK)
    {
        // TODO: Use own logger
        printf("Failed to QueryInterface of ID3D11Debug.\n");
    }
#endif // _DEBUG

    IDXGIDevice* dxgi_device = nullptr;
    if (m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device) != S_OK)
    {
        // TODO: Use own logger and return value
        printf("Failed to QueryInterface of device..\n");
        return false;
    }

    // Get Adpapter from Direct3D device (d3d11Device)
    IDXGIAdapter* dxgi_adapter = nullptr;
    if (dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter) != S_OK)
    {
        // TODO: Use own logger and return value
        printf("Failed to GetParent of dxgi_device..\n");
        return false;
    }

    // Get pointer to adapter DXGIFactory
    IDXGIFactory2* dxgi_factory = nullptr;
    if (dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgi_factory) != S_OK)
    {
        // TODO: Use own logger and return value
        printf("Failed to GetParent of dxgi_adapter.\n");
        return false;
    }

#ifdef _DEBUG
    log_hardware_info();
#endif 

    dxgi_device->Release();
    dxgi_adapter->Release();
    dxgi_factory->Release();

    return true;
}

void joj::DX11Context::make_current(std::unique_ptr<Win32Window>& window)
{
    printf("TODO()!\n");
}

void joj::DX11Context::destroy()
{
    
}

void joj::DX11Context::log_hardware_info()
{
    const u32 bytes_in_megabyte = 1048576U;

    // --------------------------------------
    // Video adapter (Graphics card)
    // --------------------------------------
    IDXGIAdapter* adapter = nullptr;
    if (m_factory->EnumAdapters(0, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        char graphics_card[128];
        size_t converted_chars = 0;
        wcstombs_s(&converted_chars, graphics_card, sizeof(graphics_card), desc.Description, _TRUNCATE);

        // TODO: Use own logger
        printf("---> Graphics card: %s.\n", graphics_card);
    }

    IDXGIAdapter4* adapter4 = nullptr;
    if (SUCCEEDED(adapter->QueryInterface(IID_PPV_ARGS(&adapter4))))
    {
        DXGI_QUERY_VIDEO_MEMORY_INFO mem_info;
        adapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &mem_info);

        // TODO: Use own logger
        printf("---> Video memory (free): %lluMB.\n", mem_info.Budget / bytes_in_megabyte);

        // FIXME: Memory used is wrong
        printf("---> Video memory(used) : %lluMB.\n", mem_info.CurrentUsage / bytes_in_megabyte);

        adapter4->Release();
    }

    // -----------------------------------------
    // TODO: Max feature Level supported by GPU
    // -----------------------------------------

    // Instructions block
    {
        // TODO: Use own logger
        printf("---> Feature Level: 11_0.\n");
    }

    // -----------------------------------------
    // Video output (monitor)
    // -----------------------------------------

    IDXGIOutput* output = nullptr;
    if (adapter->EnumOutputs(0, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        char device_name[32];
        size_t converted_chars = 0;
        wcstombs_s(&converted_chars, device_name, sizeof(device_name), desc.DeviceName, _TRUNCATE);

        // TODO: Use own logger
        printf("---> Monitor: %s.\n", device_name);
    }

    // ------------------------------------------
    // Video mode (resolution)
    // ------------------------------------------

    // Get screen dimensions
    u32 dpi = GetDpiForSystem();
    u32 screen_width = GetSystemMetricsForDpi(SM_CXSCREEN, dpi);
    u32 screen_height = GetSystemMetricsForDpi(SM_CYSCREEN, dpi);

    // Get screen update frequency
    DEVMODE dev_mode = { 0 };
    dev_mode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev_mode);
    u32 refresh = dev_mode.dmDisplayFrequency;

    // TODO: Use own logger
    printf("---> Resolution: %dx%d %d Hz.\n", screen_width, screen_height, refresh);

    // Release used DXGI interfaces
    if (adapter) adapter->Release();
    if (output) output->Release();
}
