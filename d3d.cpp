/**
 * @file d3d.cpp
 * @brief
 */

#include "d3d.h"

#include <cassert>

using namespace ND3D;

/**
 * @brief
 */
BOOL D3D::CreateDeviceAndSwapChain(HWND hWnd)
{
    assert(hWnd != nullptr);

    if (hWnd == nullptr) { return FALSE; }

    DXGI_SWAP_CHAIN_DESC sd = {};

    sd.BufferCount                          = (UINT)2U;                                     // Количество буферов.

    sd.BufferDesc.Width                     = (UINT)0U;                                     // Ширина.
    sd.BufferDesc.Height                    = (UINT)0U;                                     // Высота.
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;                   // Формат пикселей.
    sd.BufferDesc.RefreshRate.Numerator     = (UINT)60U;                                    // Чеслитель.
    sd.BufferDesc.RefreshRate.Denominator   = (UINT)1U;                                     // Знаменатель.

    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    sd.OutputWindow                         = hWnd;                                         // HANDLE окна.

    sd.SampleDesc.Count                     = (UINT)1U;                                     // Количество сэмплов на пиксель.
    sd.SampleDesc.Quality                   = (UINT)0U;                                     // Уровень качества сглаживания.

    sd.Windowed                             = TRUE;

    IDXGIAdapter* pAdapter = GetAdapter(1U);

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        pAdapter,                                                                           // Графический адаптер.
        D3D_DRIVER_TYPE_HARDWARE,                                                           // Тип драйвера.
        nullptr,                                                                            // Программный драйвер.
        (UINT)0U,                                                                           // Флаги.
        nullptr,
        (UINT)0U,
        D3D11_SDK_VERSION,                                                                  // Версия SDK.
        &sd,
        &d3DContext.pSwapChain,
        &d3DContext.pD3DDevice,
        nullptr,
        &d3DContext.pD3DDeviceContext
    );

    pAdapter->Release();
    pAdapter = nullptr;

    assert(d3DContext.pSwapChain != nullptr);
    assert(d3DContext.pD3DDevice != nullptr);

    return SUCCEEDED(hr);
}

/**
 * @brief
 */
void D3D::DestroyDeviceAndSwapChain()
{
    if (d3DContext.pD3DDevice != nullptr)
    {
        d3DContext.pD3DDevice->Release();

        d3DContext.pD3DDevice = nullptr;
    }

    if (d3DContext.pSwapChain != nullptr)
    {
        d3DContext.pSwapChain->Release();

        d3DContext.pSwapChain = nullptr;
    }

    if (d3DContext.pD3DDeviceContext != nullptr)
    {
        d3DContext.pD3DDeviceContext->Release();

        d3DContext.pD3DDeviceContext = nullptr;
    }
}

/**
 * @brief
 */
BOOL D3D::CreateTargetView()
{
    ID3D11Texture2D* pBackBuffer = nullptr;

    if (d3DContext.pSwapChain == nullptr) { return FALSE; }

    HRESULT hr = d3DContext.pSwapChain->GetBuffer((UINT)0U, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    if (FAILED(hr) == TRUE) { return FALSE; }

    hr = d3DContext.pD3DDevice->CreateRenderTargetView((ID3D11Resource*)&pBackBuffer, nullptr, &d3DContext.pRenderTargetView);

    pBackBuffer->Release();

    return SUCCEEDED(hr);
}

/**
 * @brief Получить графический адаптер.
 */
IDXGIAdapter* D3D::GetAdapter(const UINT index) const
{
    IDXGIFactory* pFactory = nullptr;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);          // Создание фабрики DXGI.

    assert(SUCCEEDED(hr) == TRUE);

    if (FAILED(hr)) { return nullptr; }

    IDXGIAdapter* pAdapter = nullptr;

    if (FAILED(pFactory->EnumAdapters(index, &pAdapter))) { return nullptr; }           // Получение графического адаптера.

    pFactory->Release();
    pFactory = nullptr;

    return pAdapter;
}

/**
 * @brief
 */
void D3D::DestroyTargetView()
{
    if (d3DContext.pRenderTargetView != nullptr)
    {
        d3DContext.pRenderTargetView->Release();

        d3DContext.pRenderTargetView = nullptr;
    }
}

//

/**
 * @brief
 */
BOOL D3D::CreateDeviceD3D(HWND hWnd)
{
    if (CreateDeviceAndSwapChain(hWnd) != TRUE) { return FALSE; }

    if (CreateTargetView() != TRUE) { return FALSE; }

    return TRUE;
}

/**
 * @brief
 */
void D3D::DestroyDeviceD3D()
{
    DestroyTargetView();

    DestroyDeviceAndSwapChain();
}
