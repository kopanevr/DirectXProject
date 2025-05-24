/**
 * @file d3d.cpp
 * @brief
 */

#include "d3d.h"

#include <Windows.h>
#include <D3DCompiler.h>
#include <DirectXTex.h>
#include <wincodec.h>

#include <cassert>

using namespace ND3D;

#define CGE

/**
 * @brief
 */
BOOL D3D::CreateDeviceAndSwapChain(HWND hWnd)
{
    assert(hWnd != nullptr);

    if (hWnd == nullptr) { return FALSE; }

    DXGI_SWAP_CHAIN_DESC sd = {};

    sd.BufferCount                          = (UINT)2U;					// Количество буферов.

    sd.BufferDesc.Width                     = (UINT)0U;                                 // Ширина.
    sd.BufferDesc.Height                    = (UINT)0U;                                 // Высота.
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;               // Формат пикселей.
    sd.BufferDesc.RefreshRate.Numerator     = (UINT)60U;                                // Чеслитель.
    sd.BufferDesc.RefreshRate.Denominator   = (UINT)1U;                                 // Знаменатель.

    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    sd.OutputWindow                         = hWnd;                                     // HANDLE окна.

    sd.SampleDesc.Count                     = (UINT)1U;                                 // Количество сэмплов на пиксель.
    sd.SampleDesc.Quality                   = (UINT)0U;                                 // Уровень качества сглаживания.

    sd.Windowed                             = TRUE;

    IDXGIAdapter* pAdapter = GetAdapter(1U);

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        pAdapter,                                                                       // Графический адаптер.
        D3D_DRIVER_TYPE_HARDWARE,                                                       // Тип драйвера.
        nullptr,                                                                        // Программный драйвер.
        (UINT)0U,                                                                       // Флаги.
        nullptr,
        (UINT)0U,
        D3D11_SDK_VERSION,                                                              // Версия SDK.
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
IDXGIAdapter* D3D::GetAdapter(const UINT i) const
{
    IDXGIFactory* pFactory = nullptr;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);          // Создание фабрики DXGI.

    assert(SUCCEEDED(hr) == TRUE);

    if (FAILED(hr)) { return nullptr; }

    IDXGIAdapter* pAdapter = nullptr;

    if (FAILED(pFactory->EnumAdapters(i, &pAdapter))) { return nullptr; }               // Получение графического адаптера.

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

/**
 * @brief
 */
BOOL D3D::SetViewport(HWND hWnd) const
{
    assert(hWnd != nullptr);

    if (hWnd == nullptr) { return FALSE; }

    RECT rect = {};

    if (GetWindowRect(hWnd, &rect) != TRUE) { return FALSE; };

    D3D11_VIEWPORT viewport = {};

    LONG width          = rect.right - rect.left;
    LONG height         = rect.bottom - rect.top;

    viewport.TopLeftX   = (FLOAT)0.0f;
    viewport.TopLeftY   = (FLOAT)0.0f;
    viewport.Width      = (FLOAT)width;
    viewport.Height     = (FLOAT)height;
    viewport.MinDepth   = (FLOAT)0.0f;
    viewport.MaxDepth   = (FLOAT)1.0f;

    if (d3DContext.pD3DDeviceContext == nullptr) { return FALSE; }

    d3DContext.pD3DDeviceContext->RSSetViewports((UINT)1U, &viewport);

    return TRUE;
}

/**
 * @brief Скомпилировать шейдер из файла.
 */
BOOL D3D::CompileShaderFromFile(LPCWSTR pFileName, LPCSTR pEntryppoint, LPCSTR pTarget, ID3DBlob** ppCode)
{
    ID3DBlob* pErrorMsgs = nullptr;

    HRESULT hr = D3DCompileFromFile(
        pFileName,
        nullptr,
        nullptr,
        pEntryppoint,
        pTarget,									// Профиль шейдера.
        D3DCOMPILE_DEBUG,
        (UINT)0U,
        ppCode,										// Код шейдера.
        &pErrorMsgs									// Сообщение об ошибке или предупрежеднии.
    );

    //

    pErrorMsgs->Release();

    assert(SUCCEEDED(hr) == TRUE);

    return SUCCEEDED(hr);
}

/**
 * @brief Создать фрагментный шейдер.
 */
BOOL D3D::SetVertexBuffer()
{
    D3D11_BUFFER_DESC bd = {};

    bd.ByteWidth                = (UINT)(3U * sizeof(Vertex));				// Размер.
    bd.Usage                    = D3D11_USAGE_DEFAULT;
    bd.BindFlags                = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags           = (UINT)0U;
    bd.MiscFlags                = (UINT)0U;
    bd.StructureByteStride      = (UINT)0U;

    D3D11_SUBRESOURCE_DATA sd = {};

    sd.pSysMem                  = (const void*)vertices;				// Данные.
    sd.SysMemPitch              = (UINT)0U;						// Шаг.
    sd.SysMemSlicePitch         = (UINT)0U;						// Шаг.

    //

    HRESULT hr = d3DContext.pD3DDevice->CreateBuffer(&bd, &sd, &d3DContext.pBuffer);

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    if (d3DContext.pD3DDeviceContext == nullptr) { return FALSE; }

    UINT stride = (UINT)sizeof(Vertex);							// Шаг.
    UINT offset = (UINT)0U;								// Смещение.

    d3DContext.pD3DDeviceContext->IASetVertexBuffers((UINT)0U, (UINT)1U, &d3DContext.pBuffer, &stride, &offset);

    return TRUE;
}

/**
 * @brief Создать вершинный шейдер.
 */
BOOL D3D::CreateVertexShader()
{
    ID3DBlob* pCode = nullptr;								// Код вершинного шейдера.

    if (CompileShaderFromFile((LPCWSTR)L"1.hlsl", (LPCSTR)"VertexMain", "vs_5_0", &pCode) != TRUE) { pCode->Release(); return FALSE; }

    HRESULT hr = d3DContext.pD3DDevice->CreateVertexShader((const void*)pCode->GetBufferPointer(), (SIZE_T)pCode->GetBufferSize(), nullptr, &d3DContext.pVertexShader);

    if (SUCCEEDED(hr) != TRUE) { pCode->Release(); return FALSE; }

    if (SetInputLayout(pCode) != TRUE) { pCode->Release(); return FALSE;}

    if (SetVertexBuffer() != TRUE) { DeInit(); return FALSE; }

    pCode->Release();

    return SUCCEEDED(hr);
}

/**
 * @brief Создать вершинный шейдер.
 */
void D3D::DestroyVertexShader()
{
    if (d3DContext.pVertexShader != nullptr)
    {
        d3DContext.pVertexShader->Release();

        d3DContext.pVertexShader = nullptr;
    }

    if (d3DContext.pInputLayout != nullptr)
    {
        d3DContext.pInputLayout->Release();

        d3DContext.pInputLayout = nullptr;
    }

    if (d3DContext.pBuffer != nullptr)
    {
        d3DContext.pBuffer->Release();

        d3DContext.pBuffer = nullptr;
    }
}

/**
 * @brief
 */
BOOL D3D::SetInputLayout(ID3DBlob* pCode)
{
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {
            (LPCSTR)"POSITION",
            (UINT)0U,									// Индекс семантики.
            DXGI_FORMAT_R8G8B8A8_UNORM,
            (UINT)0U,									// Индекс слота.
            (UINT)0U,									// Смещение в байтах.
            D3D11_INPUT_PER_VERTEX_DATA,
            (UINT)0U
        },
        {
            (LPCSTR)"TEXCOORD",
            (UINT)0U,									// Индекс семантики.
            DXGI_FORMAT_R8G8B8A8_UNORM,
            (UINT)0U,									// Индекс слота.
            (UINT)12U,									// Смещение в байтах.
            D3D11_INPUT_PER_VERTEX_DATA,
            (UINT)0U
        }
    };

    UINT numElements = (UINT)ARRAYSIZE(ied);

    HRESULT hr = d3DContext.pD3DDevice->CreateInputLayout(
        ied,
        numElements,
        (const void*)pCode->GetBufferPointer(),
        (SIZE_T)pCode->GetBufferSize(),
        &d3DContext.pInputLayout
    );

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    d3DContext.pD3DDeviceContext->IASetInputLayout(d3DContext.pInputLayout);

    return TRUE;
}

/**
 * @brief Создать фрагментный шейдер.
 */
BOOL D3D::CreatePixelShader()
{
    ID3DBlob* pCode = nullptr;								// Код фрагментного шейдера.

    if (CompileShaderFromFile((LPCWSTR)L"1.hlsl", (LPCSTR)"PixelMain", "ps_5_0", &pCode) != TRUE) { pCode->Release(); return FALSE; }

    if (d3DContext.pPixelShader == nullptr) { pCode->Release(); return FALSE; }

    HRESULT hr = d3DContext.pD3DDevice->CreatePixelShader((const void*)pCode->GetBufferPointer(), (SIZE_T)pCode->GetBufferSize(), nullptr, &d3DContext.pPixelShader);

    pCode->Release();

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    if (SetSamplerState() != TRUE) { return FALSE; }

    if (SetShaderResourceView() != TRUE) { return FALSE; }

    return TRUE;
}

/**
 * @brief
 */
BOOL D3D::SetSamplerState()
{
    D3D11_SAMPLER_DESC sd = {};

    sd.Filter           = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    sd.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;

    sd.ComparisonFunc   = D3D11_COMPARISON_NEVER;

    sd.MinLOD           = (FLOAT)0.0f;
    sd.MaxLOD           = (FLOAT)D3D11_FLOAT32_MAX;

    HRESULT hr = d3DContext.pD3DDevice->CreateSamplerState(&sd, &d3DContext.pSamplerState);

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    d3DContext.pD3DDeviceContext->PSSetSamplers((UINT)0U, (UINT)1U, &d3DContext.pSamplerState);

    return TRUE;
}

/**
 * @brief
 */
void D3D::DestroyPixelShader()
{
    if (d3DContext.pPixelShader != nullptr)
    {
        d3DContext.pPixelShader->Release();

        d3DContext.pPixelShader = nullptr;
    }

    if (d3DContext.pSamplerState != nullptr)
    {
        d3DContext.pSamplerState->Release();

        d3DContext.pSamplerState = nullptr;
    }

    if (d3DContext.pShaderResourceView != nullptr)
    {
        d3DContext.pShaderResourceView->Release();

        d3DContext.pShaderResourceView = nullptr;
    }
}

/**
 * @brief
 */
BOOL D3D::CreateShaderResourceView(const wchar_t* szFile)
{
    DirectX::TexMetadata texMetadata        = {};
    DirectX::ScratchImage scratchImage      = {};

    HRESULT hr = DirectX::LoadFromWICFile(
        szFile,
        DirectX::WIC_FLAGS_NONE,
        &texMetadata,
        scratchImage);

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    hr = DirectX::CreateShaderResourceView(
        d3DContext.pD3DDevice,
        scratchImage.GetImage((size_t)0U, (size_t)0U, (size_t)0U),
        scratchImage.GetImageCount(),
        texMetadata,
        &d3DContext.pShaderResourceView);

    assert(d3DContext.pShaderResourceView != nullptr);

    assert(d3DContext.pShaderResourceView == nullptr);

    assert(SUCCEEDED(hr) == TRUE);

    return SUCCEEDED(hr);
}

/**
 * @brief Установить ресурс.
 */
BOOL D3D::SetShaderResourceView()
{
    if (CreateShaderResourceView(L"1.PNG") != TRUE) { return FALSE; }

    d3DContext.pD3DDeviceContext->PSSetShaderResources((UINT)0U, (UINT)1U, &d3DContext.pShaderResourceView);

    return TRUE;
}

//

/**
 * @brief
 */
BOOL D3D::Init(HWND hWnd)
{
    if (CreateDeviceAndSwapChain(hWnd) != TRUE) { DeInit(); return FALSE; }

    if (CreateTargetView() != TRUE) { DeInit(); return FALSE; }

    if (SetViewport(hWnd) != TRUE) { DeInit(); return FALSE; }

    //

    if (CreateVertexShader() != TRUE) { DeInit(); return FALSE; }
    if (CreatePixelShader() != TRUE) { DeInit(); return FALSE; }

    //

    return TRUE;
}

/**
 * @brief
 */
void D3D::DeInit()
{
    DestroyPixelShader();
    DestroyVertexShader();

    DestroyTargetView();

    DestroyDeviceAndSwapChain();
}
