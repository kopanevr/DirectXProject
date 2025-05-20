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
    ID3D11VertexShader*     pVertexShader           = nullptr;      // Вершинный шейдер.
    ID3D11PixelShader*      pPixelShader            = nullptr;      // Фрагментный шейдер.
    ID3D11InputLayout*      pInputLayout            = nullptr;
};

class D3D final
{
private:
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateDeviceAndSwapChain(HWND hWnd);
    void DestroyDeviceAndSwapChain();

    D3D(const D3D&)                                 = delete;
    D3D& operator=(const D3D&)                      = delete;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateTargetView();
    void DestroyTargetView();

    IDXGIAdapter* GetAdapter(const UINT i)          const;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL SetViewport(HWND hWnd)                     const;

    //

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CompileShaderFromFile(LPCWSTR pFileName, LPCSTR pEntryppoint, LPCSTR pTarget, ID3DBlob** ppCode);
    #if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL SetInputLayout(ID3DBlob* pCode);
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateVertexShader();
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreatePixelShader();

    //
private:
    D3DContext d3DContext                           = {};
public:
    D3D()                                           = default;
    ~D3D()                                          = default;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL Init(HWND hWnd);
    void DeInit();
};
}