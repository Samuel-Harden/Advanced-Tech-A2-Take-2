#include <Windows.h>
#include <d3dcompiler.h>
#include "VBGO.h"
#include "DrawData.h"
#include "Vertex.h"
#include "Camera.h"
#include <fstream>
#include <vector>

#define DESTROY( x ) if( x ){ x->Release(); x = nullptr;}

//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr

using namespace DirectX;

//default vertexshader
ID3D11VertexShader*			VBGO::s_vertex_shader = nullptr;
//default vertex layout
ID3D11InputLayout*			VBGO::s_vertex_layout = nullptr;
//default pixelshader
ID3D11PixelShader*			VBGO::s_pixel_shader = nullptr;
//default texture (white square)
ID3D11ShaderResourceView*	VBGO::s_texture_resource_view = nullptr;
//deafult const buffer
//GPU side
ID3D11Buffer*				VBGO::s_constant_buffer = nullptr;
//CPU side	
ConstantBuffer*				VBGO::s_pCB = nullptr;
//default sampler
ID3D11SamplerState*			VBGO::s_sampler = nullptr;
//default raster state
ID3D11RasterizerState*		VBGO::s_raster_state = nullptr;


VBGO::VBGO()
{
	//set up Buffers
	m_vertex_buffer = NULL;
	m_index_buffer = NULL;
	m_num_prims = 0;

	m_vertex_shader = nullptr;
	m_vertex_layout = nullptr;
	m_pixel_shader = nullptr;
	m_constant_buffer = nullptr;
	m_pCB = nullptr;
	m_sampler = nullptr;
	m_raster_state = nullptr;

	m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}


VBGO::~VBGO()
{
	//BUT WE DO TIDY THEM AWAY
	DESTROY(m_vertex_buffer);
	DESTROY(m_index_buffer);
	DESTROY(m_vertex_shader);
	DESTROY(m_vertex_layout);
	DESTROY(m_pixel_shader);
	DESTROY(m_constant_buffer);
	//if (m_pCB) delete m_pCB; delete this where created as there will know its type
	DESTROY(m_sampler);
	DESTROY(m_raster_state);
}


void VBGO::tick(GameData* _game_data)
{
	GameObject::tick(_game_data);
}


void VBGO::draw(DrawData* _draw_data)
{
	//set raster state
	ID3D11RasterizerState* useRasterS = m_raster_state ? m_raster_state : s_raster_state;
	_draw_data->m_pd3dImmediateContext->RSSetState(useRasterS);

	//set standard Constant Buffer to match this object
	s_pCB->world = DirectX::XMMatrixTranspose(m_world_matrix);
	s_pCB->rot = DirectX::XMMatrixTranspose(m_rotation_matrix);

	//Set vertex buffer
	UINT stride = sizeof(myVertex);
	UINT offset = 0;
	_draw_data->m_pd3dImmediateContext->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	// Set index buffer
	_draw_data->m_pd3dImmediateContext->IASetIndexBuffer(m_index_buffer, m_index_format, 0);

	//update the Constant Buffer in classes that inherit from this then push to the GPU here
	ID3D11Buffer* useCB = m_constant_buffer ? m_constant_buffer : s_constant_buffer;
	void* useCBData = m_pCB ? m_pCB : s_pCB;

	_draw_data->m_pd3dImmediateContext->UpdateSubresource(useCB, 0, NULL, useCBData, 0, 0);
	_draw_data->m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, &useCB);
	_draw_data->m_pd3dImmediateContext->PSSetConstantBuffers(0, 1, &useCB);

	//unless it has it own set them to the static defaults

	//set primitive type used
	_draw_data->m_pd3dImmediateContext->IASetPrimitiveTopology(m_topology);

	//set  vertex layout
	//note that if you do use this you'll need to change the stride and offset above
	ID3D11InputLayout* useLayout = m_vertex_layout ? m_vertex_layout : s_vertex_layout;
	_draw_data->m_pd3dImmediateContext->IASetInputLayout(useLayout);

	//set Vertex Shader
	ID3D11VertexShader* useVS = m_vertex_shader ? m_vertex_shader : s_vertex_shader;
	_draw_data->m_pd3dImmediateContext->VSSetShader(useVS, NULL, 0);

	//set Pixel Shader
	ID3D11PixelShader* usePS = m_pixel_shader ? m_pixel_shader : s_pixel_shader;
	_draw_data->m_pd3dImmediateContext->PSSetShader(usePS, NULL, 0);

	//set sampler
	ID3D11SamplerState* useSample = m_sampler ? m_sampler : s_sampler;
	_draw_data->m_pd3dImmediateContext->PSSetSamplers(0, 1, &useSample);

	//and draw
	_draw_data->m_pd3dImmediateContext->DrawIndexed(3 * m_num_prims, 0, 0);//number here will need to change depending on the primative topology!
}


HRESULT VBGO::compileShaderFromFile(WCHAR* _szFileName, LPCSTR _szEntryPoint, LPCSTR _szShaderModel, ID3DBlob** _ppBlobOut)
{
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
	hr = D3DCompileFromFile(_szFileName, NULL, NULL, _szEntryPoint, _szShaderModel,
		dwShaderFlags, 0, _ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}

	return S_OK;
}

void VBGO::init(ID3D11Device* _GD)
{
	using namespace std;
	
	//default vertex shader
	ID3DBlob* pVertexShaderBuffer = NULL;
	HRESULT hr = compileShaderFromFile(L"../Test/shader.fx", "VS", "vs_4_0_level_9_1", &pVertexShaderBuffer);
	_GD->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &s_vertex_shader);

	//default pixelshader
	ID3DBlob* pPixelShaderBuffer = NULL;
	hr = compileShaderFromFile(L"../Test/shader.fx", "PS", "ps_4_0_level_9_1", &pPixelShaderBuffer);
	_GD->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &s_pixel_shader);
	
	//default vertex layout
	_GD->CreateInputLayout(myVertexLayout, ARRAYSIZE(myVertexLayout), pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), &s_vertex_layout);
	
	//default const buffer
	//GPU side
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _GD->CreateBuffer(&bd, NULL, &s_constant_buffer);
	//CPU side
	s_pCB = new ConstantBuffer();
	ZeroMemory(s_pCB, sizeof(ConstantBuffer));

	//default sampler
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _GD->CreateSamplerState(&SamDesc, &s_sampler);

	//Setup Raster State
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = _GD->CreateRasterizerState(&rasterDesc, &s_raster_state);
}


void VBGO::updateConstantBuffer(DrawData* _draw_data)
{
	s_pCB->view = XMMatrixTranspose(_draw_data->m_cam->getViewMatrix());
	s_pCB->projection = XMMatrixTranspose(_draw_data->m_cam->getProjMatrix());
}


void VBGO::cleanUp()
{
	//clear away all the static objects
	DESTROY(s_vertex_shader);
	DESTROY(s_vertex_layout);
	DESTROY(s_pixel_shader);
	DESTROY(s_texture_resource_view);
	DESTROY(s_constant_buffer);

	if (s_pCB)
	{
		delete s_pCB;
		s_pCB = nullptr;
	}

	DESTROY(s_sampler);
	DESTROY(s_raster_state);
}


void VBGO::buildIB(ID3D11Device* _GD, void* _indices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build index buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 3 * m_num_prims;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = _indices;
	hr = _GD->CreateBuffer(&bd, &InitData, &m_index_buffer);
}


void VBGO::buildVB(ID3D11Device* _GD, int _numVerts, void* _vertices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build vertex buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(myVertex) * _numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _vertices;
	hr = _GD->CreateBuffer(&bd, &InitData, &m_vertex_buffer);
}