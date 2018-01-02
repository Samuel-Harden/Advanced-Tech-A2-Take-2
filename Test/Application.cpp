#include "Application.h"
#include "Game.h"


#define DESTROY( x ) if( x ){ x->Release(); x = nullptr;}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


HRESULT Application::initWindow(HINSTANCE _hInstance, int _nCmdShow)
{
	float width = 800;
	float height = 600;

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "Advanced Tech";

	if ( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	// Create window
	m_hInst = _hInstance;

	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow("Advanced Tech", "Advanced Tech Swarm AI", WS_OVERLAPPEDWINDOW,
		600, 100, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, _hInstance,
		nullptr);

	GetLastError();

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	//actually display the window
	ShowWindow(m_hWnd, _nCmdShow);

	//Hide the mouse pointer
	ShowCursor(false);

	return S_OK;
}


Application::~Application()
{
	cleanupDevice();
}


HRESULT Application::initDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driver_type = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, m_driver_type, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pd3d_device, &m_featureLevel, &m_p_immediate_context);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, m_driver_type, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &m_pd3d_device, &m_featureLevel, &m_p_immediate_context);
		}

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgi_factory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pd3d_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgi_factory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}

	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgi_factory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));

	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pd3d_device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3d_device_one));
		if (SUCCEEDED(hr))
		{
			(void)m_p_immediate_context->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_p_immediate_context_one));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3d_device, m_hWnd, &sd, nullptr, nullptr, &m_p_swap_chain_one);
		if (SUCCEEDED(hr))
		{
			hr = m_p_swap_chain_one->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_p_swap_chain));
		}

		dxgiFactory2->Release();
	}

	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgi_factory->CreateSwapChain(m_pd3d_device, &sd, &m_p_swap_chain);
	}

	dxgi_factory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgi_factory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* p_back_buffer = nullptr;
	hr = m_p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&p_back_buffer));
	if (FAILED(hr))
		return hr;

	hr = m_pd3d_device->CreateRenderTargetView(p_back_buffer, nullptr, &m_p_render_target_view);
	p_back_buffer->Release();
	if (FAILED(hr))
		return hr;

	m_p_immediate_context->OMSetRenderTargets(1, &m_p_render_target_view, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_p_immediate_context->RSSetViewports(1, &vp);

	//Create depth stencil texture
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
	hr = m_pd3d_device->CreateTexture2D(&descDepth, NULL, &m_p_depth_stencil);
	if (FAILED(hr))
		return hr;

	//Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pd3d_device->CreateDepthStencilView(m_p_depth_stencil, &descDSV, &m_p_depth_stencil_view);
	if (FAILED(hr))
		return hr;

	//use the just created render target and depth stencil
	m_p_immediate_context->OMSetRenderTargets(1, &m_p_render_target_view, m_p_depth_stencil_view);

	//Setup Raster State
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = m_pd3d_device->CreateRasterizerState(&rasterDesc, &m_p_raster_state);
	if (FAILED(hr))
		return hr;

	//use the raster state
	m_p_immediate_context->RSSetState(m_p_raster_state);

	m_ClearColour[0] = 1.0f;
	m_ClearColour[1] = 1.0f;
	m_ClearColour[2] = 1.0f;
	m_ClearColour[3] = 1.0f;


	//actually create the game
	m_game = new Game(m_pd3d_device, m_hWnd, m_hInst);

	return S_OK;
}


void Application::render()
{
	// Just clear the backbuffer
	m_p_immediate_context->ClearRenderTargetView(m_p_render_target_view, m_ClearColour);

	// Clear the depth stencil
	m_p_immediate_context->ClearDepthStencilView(m_p_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0, 0);

	//Render the next frame from the game
	if (m_game) m_game->draw(m_p_immediate_context);

	m_p_swap_chain->Present(0, 0);
}


bool Application::update()
{
	if (m_game)
	{
		return m_game->tick();
	}

	return false;
}


void Application::cleanupDevice()
{
	//Destroy the Game instance
	if (m_game)
	{
		delete m_game;
		m_game = nullptr;
	}

	//Tidy up all DirectX stuff
	if (m_p_immediate_context) m_p_immediate_context->ClearState();

	DESTROY(m_p_raster_state);
	DESTROY(m_p_depth_stencil);
	DESTROY(m_p_depth_stencil_view);
	DESTROY(m_p_render_target_view);
	DESTROY(m_p_swap_chain_one);
	DESTROY(m_p_swap_chain);
	DESTROY(m_p_immediate_context_one);
	DESTROY(m_p_immediate_context);
	DESTROY(m_pd3d_device_one);
	DESTROY(m_pd3d_device);
}
