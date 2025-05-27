 /**
 * @file d3d.h
 * @brief
 */

#pragma once

#include <d3d11.h>
#include <dxgi.h>

#include "data.h"

using namespace NDATA;

namespace ND3D
{
struct Vertex
{
    FLOAT x = (FLOAT)0.0f;
    FLOAT y = (FLOAT)0.0f;
    FLOAT z = (FLOAT)0.0f;

    FLOAT u = (FLOAT)0.0f;						// Текстурная координата.
    FLOAT v = (FLOAT)0.0f;						// Текстурная координата.
};

/**
 * @brief Вершины фигуры.
 */
static Vertex vertices[] = {
//                           u      v
    { 0.0f,  0.5f,  0.0f,    0.5f,  0.0f },
    { 0.5f, -0.5f,  0.0f,    1.0f,  1.0f },
    {-0.5f, -0.5f,  0.0f,    0.0f,  1.0f }
};

struct D3DContext final
{
    ID3D11Device*               pD3DDevice              = nullptr;
    IDXGISwapChain*             pSwapChain              = nullptr;
    ID3D11DeviceContext*        pD3DDeviceContext       = nullptr;
    ID3D11RenderTargetView*     pRenderTargetView       = nullptr;
    ID3DBlob*                   pVertexCode             = nullptr;
    ID3D11Buffer*               pBuffer                 = nullptr;      // Вершинный буфер.
    ID3D11VertexShader*         pVertexShader           = nullptr;      // Вершинный шейдер.
    ID3D11PixelShader*          pPixelShader            = nullptr;      // Фрагментный шейдер.
    ID3DBlob*                   pPixelCode              = nullptr;
    ID3D11InputLayout*          pInputLayout            = nullptr;
    ID3D11SamplerState*         pSamplerState           = nullptr;
    ID3D11Texture2D*            pTexture2D              = nullptr;      // Текстура.
    ID3D11ShaderResourceView*   pShaderResourceView     = nullptr;      // Ресурс.
};

class D3D final
{
private:
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateDeviceAndSwapChain(HWND hWnd);
    void DestroyDeviceAndSwapChain();

    D3D(const D3D&)                                     = delete;
    D3D& operator=(const D3D&)                          = delete;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateTargetView();
    void DestroyTargetView();

    

    IDXGIAdapter* GetAdapter(const UINT i)              const;

    //

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CompileShaderFromFile(LPCWSTR pFileName, LPCSTR pEntryppoint, LPCSTR pTarget, ID3DBlob** ppCode);

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateVertexBuffer();
    void DestroyVertexBuffer();
    void SetVertexBuffer();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateVertexShader();
    void DestroyVertexShader();
    void SetVertexShader();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateInputLayout();
    void DestroyInputLayout();
    void SetInputLayout();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreatePixelShader();
    void DestroyPixelShader();
    void SetPixelShader();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateSamplerState();
    void DestroySamplerState();
    void SetSamplerState();

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL CreateShaderResourceView(const wchar_t* szFile);
    void DestroyShaderResourceView();
    void SetShaderResourceView();

    //

    void Draw();
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL Present();
private:
    D3DContext d3DContext                               = {};
public:
    D3D()                                               = default;
    ~D3D()                                              = default;

#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL Init(HWND hWnd);
    void DeInit();
    void ClearTargetView();
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    BOOL SetViewport(HWND hWnd)                         const;
#if __cplusplus > 201703L
    [[nodiscard]]
#endif
    D3DContext* GetD3DContext();

    void Render();
public:
    Data data                                           = {};
};
}
