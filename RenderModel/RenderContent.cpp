#include <string>
#include "RenderContent.h"
#include "RenderCamera.h"

template<typename T>
inline void SafeRelease(T* p)
{
	if (0 != p)
	{
		p->Release();
	}
}

template<typename T>
inline void SafeAddRef(T* p)
{
	if (0 != p)
	{
		p->AddRef();
	}
}

RenderContent::RenderContent()
{
	m_camera = std::make_shared<RenderCamera>();
}

RenderContent::~RenderContent()
{
	SafeRelease(g_pd3dDevice);
	SafeRelease(g_pImmediateContext);
	SafeRelease(g_pSwapChain);
}

#define sizeofarray(a)	(sizeof(a)/sizeof(a[0]))

void RenderContent::initDevice(int wHid, int width, int height)
{
	ID3D11Device* device = 0;
	ID3D11DeviceContext* immediateContext = 0;
	IDXGISwapChain* swapChain = 0;
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

	UINT deviceFlag = 0;// D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_FALSE;
	if (wHid > 0)
	{
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
		sd.OutputWindow = (HWND)wHid;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		hr = D3D11CreateDeviceAndSwapChain(selectedAdapter, driverType,
			0, deviceFlag, featureLevels, sizeofarray(featureLevels),
			D3D11_SDK_VERSION, &sd,
			&swapChain, &device, &featureLevel, &immediateContext);
		if (FAILED(hr))
		{
			return;
		}

		ID3D11Texture2D* pBackBuffer = 0;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return;

		hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return;
	}
	else
	{
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
		sd.OutputWindow = (HWND)wHid;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		hr = D3D11CreateDevice(selectedAdapter, driverType,
			0, deviceFlag, featureLevels, sizeofarray(featureLevels),
			D3D11_SDK_VERSION, /*&sd,
			&swapChain,*/ &device, &featureLevel, &immediateContext);
		if (FAILED(hr))
		{
			return;
		}



		ID3D11Texture2D* pBackBuffer = 0;
// 		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
// 		if (FAILED(hr))
// 			return;

		CD3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(&textureDesc, NULL, &pBackBuffer);
		if (FAILED(hr))
			return;

		hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return;

// 		ID3D11Texture2D* resTexture = 0;
// 		textureDesc.Usage = D3D11_USAGE_DEFAULT;
// 		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// 		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
// 
// 		hr = device->CreateTexture2D(&textureDesc, NULL, &resTexture);
// 		if (FAILED(hr))
// 			return;
// 		CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
// 		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
// 		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
// 		viewDesc.Texture2D.MostDetailedMip = 0;
// 		viewDesc.Texture2D.MipLevels = -1;
// 
// 		ID3D11ShaderResourceView* resView = NULL;
// 		hr = device->CreateShaderResourceView(resTexture,&viewDesc, &resView);
// 		if (FAILED(hr))
// 			return;
// 		m_copyResView = resView;
	}



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

	g_pSwapChain = swapChain;
	g_pd3dDevice = device;
	g_pImmediateContext = immediateContext;

	g_pRenderTargetView = renderTargetView;
	g_pDepthStencilView = depthStencilView;

	m_camera->setCameraParam(width, height, 0.01, 1000);
	m_camera->setWorldPosition(D3DXVECTOR3(0,0,-10));
	buildRenderCube();
}


static HRESULT CompileShaderFromFile(std::string szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
// 	if (szFileName.size() >= 2 && szFileName[1] != ':')
// 	{
// 		szFileName = getResouseDir() + '/' + szFileName;
// 	}

	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName.c_str(), NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void RenderContent::frameMove(std::uint64_t frameNumber, std::uint64_t elapsed)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	g_pImmediateContext->VSSetShader(NULL, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 0, NULL);
	g_pImmediateContext->PSSetShader(NULL, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 0, NULL);
	g_pImmediateContext->IASetInputLayout(NULL);
	g_pImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->RSSetState(NULL);

	float BlendFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	g_pImmediateContext->OMSetBlendState(NULL, BlendFactor, 0xffffffff);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED);

	renderCube(10);
	renderCube(21);
	if (g_pSwapChain)
	{
		g_pSwapChain->Present(0, 0);
	}
	else
	{
		ID3D11RenderTargetView* ppRenderTargetViews = NULL;
		ID3D11DepthStencilView* ppDepthStencilView = 0;
		g_pImmediateContext->OMGetRenderTargets(1, &ppRenderTargetViews, &ppDepthStencilView);
		ID3D11Resource* renderTarget = NULL;
		ppRenderTargetViews->GetResource(&renderTarget);
		if (renderTarget)
		{
			HRESULT rul = D3DX11SaveTextureToFile(g_pImmediateContext, renderTarget, D3DX11_IFF_PNG,"11copyTexture.png");
			int a = 0;
		}
	}
}

struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir;
	XMFLOAT4 vLightColor;
	XMFLOAT4 vOutputColor;
};

void RenderContent::buildRenderCube()
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("renderFile/renderObject.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		// 		MessageBox(NULL,
		// 			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return;
	
	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("renderFile/renderObject.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		// 		MessageBox(NULL,
		// 			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return;


	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f) },

		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	//con.g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Create index buffer
	// Create vertex buffer
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return;

	// Set index buffer
	//con.g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	//con.g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return;
}

void RenderContent::renderCube(float t)
{
	//static float t = 15;
// 	static DWORD dwTimeStart = 0;
// 	DWORD dwTimeCur = GetTickCount();
// 	if (dwTimeStart == 0)
// 		dwTimeStart = dwTimeCur;
// 	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	XMMATRIX                g_World;
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;

	// Rotate cube around the origin
	g_World = XMMatrixRotationY(t);
	g_World = XMMatrixRotationRollPitchYaw(t*2,-t,t);

	// Setup our lighting parameters
	XMFLOAT4 vLightDirs[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 vLightColors[2] =
	{
		XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),
		XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)
	};

	// Rotate the second light around the origin
	XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
	XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
	vLightDir = XMVector3Transform(vLightDir, mRotate);
	XMStoreFloat4(&vLightDirs[1], vLightDir);

	// 	//
	// 	// Clear the back buffer
	// 	//
	// 	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	// 	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	// 
	// 	//
	// 	// Clear the depth buffer to 1.0 (max depth)
	// 	//
	// 	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//
		// Update matrix variables and lighting variables
		//
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(g_World);
	g_View = *(XMMATRIX*)&(m_camera->getViewMatrix());;
	cb1.mView = XMMatrixTranspose(g_View);
	g_Projection = *(XMMATRIX*)&(m_camera->getProjectionMatrix());
	cb1.mProjection = XMMatrixTranspose(g_Projection);
	cb1.vLightDir = vLightDirs[0];
	//cb1.vLightDir[1] = vLightDirs[1];
	cb1.vLightColor = vLightColors[0];
	//cb1.vLightColor[1] = vLightColors[1];
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	//
	// Render the cube
	//
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->DrawIndexed(36, 0, 0);

	//
	// Render each light
	//
// 	for (int m = 1; m < 2; m++)
// 	{
// 		XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&vLightDirs[m]));
// 		XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
// 		mLight = mLightScale * mLight;
// 
// 		// Update the world variable to reflect the current light
// 		cb1.mWorld = XMMatrixTranspose(mLight);
// 		cb1.vOutputColor = vLightColors[m];
// 		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);
// 
// 		g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
// 		g_pImmediateContext->DrawIndexed(36, 0, 0);
// 	}
}
