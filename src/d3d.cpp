/**
 * @file d3d.cpp
 * @brief
 */

#include "d3d.h"

#include <iostream>
#include <cmath> 

#include <Windows.h>
#include <D3DCompiler.h>
#include <DirectXTex.h>
#include <wincodec.h>

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

    sd.BufferCount                          = (UINT)2U;					// Количество буферов.

    sd.BufferDesc.Width                     = (UINT)0U;                                 // Ширина.
    sd.BufferDesc.Height                    = (UINT)0U;                                 // Высота.
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;               // Формат пикселей.
    sd.BufferDesc.RefreshRate.Numerator     = (UINT)60U;                                // Числитель.
    sd.BufferDesc.RefreshRate.Denominator   = (UINT)1U;                                 // Знаменатель.

    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    sd.OutputWindow                         = hWnd;                                     // HANDLE окна.

    sd.SampleDesc.Count                     = (UINT)1U;                                 // Количество сэмплов на пиксель.
    sd.SampleDesc.Quality                   = (UINT)0U;                                 // Уровень качества сглаживания.

    sd.Windowed                             = TRUE;

    IDXGIAdapter* pAdapter = GetAdapter(0U);

    D3D_FEATURE_LEVEL featureLevel;

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0
    };

    //

    UINT createDeviceFlags = (UINT)0U;

#ifndef NDEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    //

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        pAdapter,                                                                       // Графический адаптер.
        D3D_DRIVER_TYPE_UNKNOWN,                                                        // Тип драйвера.
        nullptr,                                                                        // Программный драйвер.
        createDeviceFlags,                                                              // Флаги.
        featureLevels,
        (UINT)ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,                                                              // Версия SDK.
        &sd,
        &d3DContext.pSwapChain,
        &d3DContext.pD3DDevice,
        &featureLevel,
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
void D3D::SetTargetView()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pRenderTargetView == nullptr) { return; }

    d3DContext.pD3DDeviceContext->OMSetRenderTargets((UINT)1U, &d3DContext.pRenderTargetView, nullptr);
}

/**
 * @brief
 */
void D3D::ClearTargetView()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pRenderTargetView == nullptr) { return; }

    const FLOAT сolorRGBA[4] = { (FLOAT)0.2f, (FLOAT)0.298f, (FLOAT)0.29f, (FLOAT)1.0f };

    d3DContext.pD3DDeviceContext->ClearRenderTargetView(d3DContext.pRenderTargetView, сolorRGBA);
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

    //

    DXGI_ADAPTER_DESC dsc = {};

    hr = pAdapter->GetDesc(&dsc);

    assert(SUCCEEDED(hr) == true);

    if (FAILED(hr) == true) {  }

    std::cout << "Adapter:          " << i << std::endl;
    std::cout << "Id:               0x" << reinterpret_cast<const void*>(dsc.DeviceId) << std::endl;
    std::wcout << "Description:      " << dsc.Description << std::endl;

    //

    return pAdapter;
}

/**
 * @brief
 */
void D3D::SetViewport(HWND hWnd) const
{
    assert(hWnd != nullptr);

    if (hWnd == nullptr) { return; }

    RECT rect = {};

    if (GetWindowRect(hWnd, &rect) != TRUE) { return; };

    D3D11_VIEWPORT viewport = {};

    LONG width          = rect.right - rect.left;
    LONG height         = rect.bottom - rect.top;

    viewport.TopLeftX   = (FLOAT)0.0f;
    viewport.TopLeftY   = (FLOAT)0.0f;
    viewport.Width      = (FLOAT)width;
    viewport.Height     = (FLOAT)height;
    viewport.MinDepth   = (FLOAT)0.0f;
    viewport.MaxDepth   = (FLOAT)1.0f;

    if (d3DContext.pD3DDeviceContext == nullptr) { return; }

    d3DContext.pD3DDeviceContext->RSSetViewports((UINT)1U, &viewport);
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

    if (pErrorMsgs != nullptr) { pErrorMsgs->Release(); return FALSE; }

    assert(SUCCEEDED(hr) == TRUE);

    return SUCCEEDED(hr);
}

/**
 * @brief
 */
BOOL D3D::CreateVertexBuffer()
{
    DestroyVertexBuffer();

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

    return SUCCEEDED(d3DContext.pD3DDevice->CreateBuffer(&bd, &sd, &d3DContext.pBuffer));
}

/**
 * @brief
 */
void D3D::SetVertexBuffer()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }

    UINT stride = (UINT)sizeof(Vertex);							// Шаг.
    UINT offset = (UINT)0U;								// Смещение.

    d3DContext.pD3DDeviceContext->IASetVertexBuffers((UINT)0U, (UINT)1U, &d3DContext.pBuffer, &stride, &offset);
}

/**
 * @brief
 */
void D3D::DestroyVertexBuffer()
{
    if (d3DContext.pBuffer != nullptr)
    {
        d3DContext.pBuffer->Release();

        d3DContext.pBuffer = nullptr;
    }
}

/**
 * @brief Создать вершинный шейдер.
 */
BOOL D3D::CreateVertexShader()
{
    if (CompileShaderFromFile((LPCWSTR)L"src/1.hlsl", (LPCSTR)"VertexMain", "vs_5_0", &d3DContext.pVertexCode) != TRUE) { return FALSE; }

    HRESULT hr = d3DContext.pD3DDevice->CreateVertexShader((const void*)d3DContext.pVertexCode->GetBufferPointer(), (SIZE_T)d3DContext.pVertexCode->GetBufferSize(), nullptr, &d3DContext.pVertexShader);

    return SUCCEEDED(hr);
}

/**
 * @brief
 */
void D3D::DestroyVertexShader()
{
    if (d3DContext.pVertexShader != nullptr)
    {
        d3DContext.pVertexShader->Release();

        d3DContext.pVertexShader = nullptr;
    }
}

/**
 * @brief
 */
void D3D::SetVertexShader()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pVertexShader == nullptr) { return; }

    d3DContext.pD3DDeviceContext->VSSetShader(d3DContext.pVertexShader, nullptr, (UINT)0);
}

/**
 * @brief
 */
BOOL D3D::CreateInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {
            (LPCSTR)"POSITION",
            (UINT)0U,									// Индекс семантики.
            DXGI_FORMAT_R32G32B32_FLOAT,
            (UINT)0U,									// Индекс слота.
            (UINT)0U,									// Смещение в байтах.
            D3D11_INPUT_PER_VERTEX_DATA,
            (UINT)0U
        },
        {
            (LPCSTR)"TEXCOORD",
            (UINT)0U,									// Индекс семантики.
            DXGI_FORMAT_R32G32_FLOAT,
            (UINT)0U,									// Индекс слота.
            (UINT)12U,									// Смещение в байтах.
            D3D11_INPUT_PER_VERTEX_DATA,
            (UINT)0U
        }
    };

    return SUCCEEDED(d3DContext.pD3DDevice->CreateInputLayout(
        ied,
        (UINT)ARRAYSIZE(ied),
        (const void*)d3DContext.pVertexCode->GetBufferPointer(),
        (SIZE_T)d3DContext.pVertexCode->GetBufferSize(),
        &d3DContext.pInputLayout
    ));
}

/**
 * @brief
 */
void D3D::DestroyInputLayout()
{
    if (d3DContext.pInputLayout != nullptr)
    {
        d3DContext.pInputLayout->Release();

        d3DContext.pInputLayout = nullptr;
    }
}

/**
 * @brief
 */
void D3D::SetInputLayout()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pInputLayout == nullptr) { return; }

    d3DContext.pD3DDeviceContext->IASetInputLayout(d3DContext.pInputLayout);
}

/**
 * @brief Создать фрагментный шейдер.
 */
BOOL D3D::CreatePixelShader()
{
    if (CompileShaderFromFile((LPCWSTR)L"src/1.hlsl", (LPCSTR)"PixelMain", "ps_5_0", &d3DContext.pPixelCode) != TRUE) { return FALSE; }

    if (d3DContext.pPixelCode == nullptr) { return FALSE; }

    return SUCCEEDED(d3DContext.pD3DDevice->CreatePixelShader((const void*)d3DContext.pPixelCode->GetBufferPointer(), (SIZE_T)d3DContext.pPixelCode->GetBufferSize(), nullptr, &d3DContext.pPixelShader));
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
}

/**
 * @brief
 */
void D3D::SetPixelShader()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pPixelShader == nullptr) { return; }

    d3DContext.pD3DDeviceContext->PSSetShader(d3DContext.pPixelShader, nullptr, (UINT)0);
}

/**
 * @brief
 */
BOOL D3D::CreateSamplerState()
{
    D3D11_SAMPLER_DESC sd = {};

    sd.Filter           = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    sd.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;

    sd.ComparisonFunc   = D3D11_COMPARISON_NEVER;

    sd.MinLOD           = (FLOAT)0.0f;
    sd.MaxLOD           = D3D11_FLOAT32_MAX;

    return SUCCEEDED(d3DContext.pD3DDevice->CreateSamplerState(&sd, &d3DContext.pSamplerState));
}

/**
 * @brief
 */
void D3D::DestroySamplerState()
{
    if (d3DContext.pSamplerState != nullptr)
    {
        d3DContext.pSamplerState->Release();

        d3DContext.pSamplerState = nullptr;
    }
}

/**
 * @brief
 */
void D3D::SetSamplerState()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pSamplerState == nullptr) { return; }

    d3DContext.pD3DDeviceContext->PSSetSamplers((UINT)0U, (UINT)1U, &d3DContext.pSamplerState);
}

/**
 * @brief
 */
BOOL D3D::CreateShaderResourceView(const wchar_t* szFile)
{
    DestroyShaderResourceView();

    DirectX::TexMetadata texMetadata        = {};
    DirectX::ScratchImage scratchImage      = {};

    HRESULT hr = CoInitialize(nullptr);

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    hr = DirectX::LoadFromWICFile(
        szFile,
        DirectX::WIC_FLAGS_NONE,
        &texMetadata,
        scratchImage);

    assert(SUCCEEDED(hr) == TRUE);

    if (SUCCEEDED(hr) != TRUE) { return FALSE; }

    return SUCCEEDED(DirectX::CreateShaderResourceView(
        d3DContext.pD3DDevice,
        scratchImage.GetImage((size_t)0U, (size_t)0U, (size_t)0U),
        scratchImage.GetImageCount(),
        texMetadata,
        &d3DContext.pShaderResourceView
    ));
}

/**
 * @brief
 */
void D3D::DestroyShaderResourceView()
{
    if (d3DContext.pShaderResourceView != nullptr)
    {
        d3DContext.pShaderResourceView->Release();

        d3DContext.pShaderResourceView = nullptr;
    }
}

/**
 * @brief Установить ресурс.
 */
void D3D::SetShaderResourceView()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }
    if (d3DContext.pShaderResourceView == nullptr) { return; }

    d3DContext.pD3DDeviceContext->PSSetShaderResources((UINT)0U, (UINT)1U, &d3DContext.pShaderResourceView);
}

/**
 * @brief
 */
void D3D::SetPrimitiveTopoligy()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }

    d3DContext.pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/**
 * @brief Отрендерить.
 */
void D3D::Draw()
{
    if (d3DContext.pD3DDeviceContext == nullptr) { return; }

    d3DContext.pD3DDeviceContext->Draw((UINT)3U, (UINT)0U);
}

/**
 * @brief
 */
void D3D::SetCoordinate() const
{
    vertices[0].x = d->payload.x;
    vertices[1].x = d->payload.x + 0.5f;
    vertices[2].x = d->payload.x - 0.5f;

    vertices[0].y = d->payload.y + 0.5f;
    vertices[1].y = d->payload.y - 0.5f;
    vertices[2].y = d->payload.y - 0.5f;
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

    hr = d3DContext.pD3DDevice->CreateRenderTargetView((ID3D11Resource*)pBackBuffer, nullptr, &d3DContext.pRenderTargetView);

    pBackBuffer->Release();

    return SUCCEEDED(hr);
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
BOOL D3D::ResizeSwapChaninBuffers(HWND hWnd)
{
    if (d3DContext.pSwapChain == nullptr) { return FALSE; }

    assert(hWnd != nullptr);

    if (hWnd == nullptr) { return FALSE; }

    RECT rect = {};

    if (GetWindowRect(hWnd, &rect) != TRUE) { return FALSE; };

    LONG width = rect.right - rect.left;
    LONG height = rect.bottom - rect.top;

    return SUCCEEDED(d3DContext.pSwapChain->ResizeBuffers(
        (UINT)0U,
        (UINT)width,
        (UINT)height,
        DXGI_FORMAT_UNKNOWN,
        (UINT)0U
    ));
}

/**
 * @brief
 */
BOOL D3D::Init(HWND hWnd)
{
    if (CreateDeviceAndSwapChain(hWnd) != TRUE) { DeInit(); return FALSE; }

    if (CreateTargetView() != TRUE) { DeInit(); return FALSE; }

    SetViewport(hWnd);

    //

    if (CreateVertexShader() != TRUE) { DeInit(); return FALSE; }

    //

    if (CreateInputLayout() != TRUE) { DeInit(); return FALSE; }

    SetInputLayout();

    if (CreateSamplerState() != TRUE) { DeInit(); return FALSE; }

    //

    if (CreatePixelShader() != TRUE) { DeInit(); return FALSE; }

    //

    return TRUE;
}

/**
 * @brief
 */
void D3D::DeInit()
{
    DestroyShaderResourceView();

    //

    DestroyPixelShader();

    //

    DestroySamplerState();

    DestroyVertexBuffer();

    DestroyInputLayout();

    //

    DestroyVertexShader();

    //

    DestroyTargetView();

    DestroyDeviceAndSwapChain();
}

/**
 * @brief
 */
D3DContext* D3D::GetD3DContext()
{
    return &d3DContext;
}

/**
 * @brief
 */
void D3D::HandleData()
{
    static bool flag = false;
    static TEXTURES previousValueTexture    = d->payload.texture;
    static float previousX                  = d->payload.x;
    static float previousY                  = d->payload.y;

    if (flag == true)
    {
        if (previousValueTexture != d->payload.texture)
        {
            previousValueTexture = d->payload.texture;

            if (d->payload.texture == TEXTURES::TEXTURE_0)
            {
                if (CreateShaderResourceView(L"src/1.PNG") != TRUE) { return; }
            }
            else
            {
                if (CreateShaderResourceView(L"src/2.PNG") != TRUE) { return; }
            }
        }

        //

        if (previousX != d->payload.x || previousY != d->payload.y)
        {
            previousX = d->payload.x;
            previousY = d->payload.y;

            SetCoordinate();

            if (CreateVertexBuffer() != TRUE) { return; }
        }
    }
    else
    {
        previousValueTexture = d->payload.texture;

        if (d->payload.texture == TEXTURES::TEXTURE_0)
        {
            if (CreateShaderResourceView(L"src/1.PNG") != TRUE) { return; }
        }
        else
        {
            if (CreateShaderResourceView(L"src/2.PNG") != TRUE) { return; }
        }

        //

        SetCoordinate();

        if (CreateVertexBuffer() != TRUE) { return; }

        //

        flag = true;
    }
}

/**
 * @brief
 */
void D3D::Render()
{
    ClearTargetView();

    //

    HandleData();


    SetTargetView();
    SetVertexShader();
    SetPixelShader();
    SetVertexBuffer();
    SetSamplerState();
    SetShaderResourceView();
    SetPrimitiveTopoligy();

    //

    Draw();                                 						// Отрендерить.
}

/**
 * @brief Переключить буферы.
 */
BOOL D3D::Present()
{
    if (d3DContext.pSwapChain == nullptr) { return FALSE; }

    return SUCCEEDED(d3DContext.pSwapChain->Present((UINT)0U, (UINT)0U));
}
