#pragma once

#include "GameObject.h"
#include "ConstantBuffer.h"

//=================================================================
//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr
//=================================================================

class VBGO :public GameObject
{
public:
	VBGO();
	virtual ~VBGO();

	virtual void tick(GameData* _game_data) override;
	virtual void draw(DrawData* _draw_data) override;

	//set up and then destroy all static default render stuff for VBGOs
	static void init(ID3D11Device* _GD);//Graphics Device NOT GameData
	static void cleanUp();

	//Update static const buffer required for deafult rendering
	static void updateConstantBuffer(DrawData* _draw_data);

protected:

	//Direct X information for the model for this GameObject
	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
	DXGI_FORMAT m_index_format = DXGI_FORMAT_R16_UINT;
	UINT m_num_prims;

	//vertex topology in VB
	D3D_PRIMITIVE_TOPOLOGY m_topology;

	//default vertexshader
	static ID3D11VertexShader*     s_vertex_shader;
	//default vertex layout
	static ID3D11InputLayout*      s_vertex_layout;
	//default pixelshader
	static ID3D11PixelShader*      s_pixel_shader;
	//default texture (white square)
	static ID3D11ShaderResourceView* s_texture_resource_view;
	//deafult const buffer
	static ID3D11Buffer*           s_constant_buffer;
	static ConstantBuffer*			s_pCB;				

	static ID3D11SamplerState*		s_sampler;
	//default raster state
	static ID3D11RasterizerState*  s_raster_state;

	//my vertexshader
	ID3D11VertexShader*     m_vertex_shader;
	//my vertex layout
	ID3D11InputLayout*      m_vertex_layout;
	//my pixelshader
	ID3D11PixelShader*      m_pixel_shader;

	//my const buffer
	ID3D11Buffer*           m_constant_buffer;
	void*					m_pCB;

	ID3D11SamplerState*		m_sampler;
	//my raster state
	ID3D11RasterizerState*  m_raster_state;

	//once populated build an Index Buffer
	void buildIB(ID3D11Device* _GD, void* _indices);

	//once populated build a Vertex Buffer
	void buildVB(ID3D11Device* _GD, int _numVerts, void* _vertices);

	static HRESULT compileShaderFromFile(WCHAR* _szFileName, LPCSTR _szEntryPoint, LPCSTR _szShaderModel, ID3DBlob** _ppBlobOut);


};