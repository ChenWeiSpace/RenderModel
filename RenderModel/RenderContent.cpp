#include "RenderContent.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

RenderContent::RenderContent()
{

}

RenderContent::~RenderContent()
{

}

#define sizeofarray(a)	(sizeof(a)/sizeof(a[0]))

void RenderContent::initDevice(int width, int height)
{
	ID3D11Device* device = 0;
	ID3D11DeviceContext* immediateContext = 0;
	IDXGISwapChain* swapChain = 0;
	ID3D11Texture2D* backBuffer = 0;
	ID3D11Texture2D* depthStencilBuffer = 0;
	ID3D11RenderTargetView* renderTargetView = 0;
	ID3D11DepthStencilView* depthStencilView = 0;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


	UINT nAdapter = 0;
	IDXGIAdapter* adapter = NULL;
	IDXGIAdapter* selectedAdapter = NULL;
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	//sd.OutputWindow = (HWND)wHid;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT deviceFlag = 0;// D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(selectedAdapter, driverType,
		0, deviceFlag, featureLevels, sizeofarray(featureLevels),
		D3D11_SDK_VERSION, &sd,
		&swapChain, &device, &featureLevel, &immediateContext);
	if (FAILED(hr))
	{
		return;
	}
	//g_pSwapChain = swapChain;

	ID3D11Texture2D* pBackBuffer = 0;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return;
	//g_pd3dDevice = device;

	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	ID3D11Texture2D* pDepthStencil = NULL;
	hr = device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		return;


	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(pDepthStencil, &descDSV, &depthStencilView);
	if (FAILED(hr))
		return;
	pDepthStencil->Release();

	//g_pImmediateContext = immediateContext;

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	renderTargetView->Release();
	depthStencilView->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	immediateContext->RSSetViewports(1, &vp);
	m_width = width;
	m_height = height;
}
