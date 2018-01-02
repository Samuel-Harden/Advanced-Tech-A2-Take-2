#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


class Game;

class Application
{
public:
	Application() {};
	~Application();

	HRESULT initWindow(HINSTANCE _hInstance, int _nCmdShow);

	HRESULT initDevice();
	void cleanupDevice();

	bool update();
	void render();

protected:

	//Window handles
	HINSTANCE               m_hInst = nullptr;
	HWND                    m_hWnd = nullptr;

	//DirectX Resources
	D3D_DRIVER_TYPE         m_driver_type = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3d_device = nullptr;
	ID3D11Device1*          m_pd3d_device_one = nullptr;
	ID3D11DeviceContext*    m_p_immediate_context = nullptr;
	ID3D11DeviceContext1*   m_p_immediate_context_one = nullptr;
	IDXGISwapChain*         m_p_swap_chain = nullptr;
	IDXGISwapChain1*        m_p_swap_chain_one = nullptr;
	ID3D11RenderTargetView* m_p_render_target_view = nullptr;
	ID3D11Texture2D*        m_p_depth_stencil = nullptr;
	ID3D11DepthStencilView* m_p_depth_stencil_view = nullptr;
	ID3D11RasterizerState*  m_p_raster_state = nullptr;

	float m_ClearColour[4];

	//pointer to current game
	Game*					m_game = nullptr;
};