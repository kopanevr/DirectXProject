/**
 * @file d3d.h
 * @brief
 */

#pragma once

#include <d3d11.h>
#include <dxgi.h>

namespace ND3D
{
struct D3DContext final
{
    ID3D11Device*           pD3DDevice              = nullptr;
    IDXGISwapChain*         pSwapChain              = nullptr;
    ID3D11DeviceContext*    pD3DDeviceContext       = nullptr;
    ID3D11RenderTargetView* pRenderTargetView       = nullptr;
};

class D3D final
{
private:
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateDeviceAndSwapChain(HWND hWnd);
    void DestroyDeviceAndSwapChain();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    D3D(const D3D&)                                 = delete;
    D3D& operator=(const D3D&)                      = delete;

    BOOL CreateTargetView();
    void DestroyTargetView();

    IDXGIAdapter* GetAdapter(const UINT index)      const;
private:
    D3DContext d3DContext                           = {};
public:
    D3D()                                           = default;
    ~D3D()                                          = default;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateDeviceD3D(HWND hWnd);
    void DestroyDeviceD3D();
};
}
