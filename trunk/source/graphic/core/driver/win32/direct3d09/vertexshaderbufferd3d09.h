/*!
    @file
    @brief	バーテックスシェーダーバッファ D3D9 Ver.
 */

#ifndef graphic_core_driver_win32_direct3d09_vertexshaderbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_vertexshaderbufferd3d09_h



#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include"../../vertexshaderbuffer.h"

#include"d3d09include.h"


namespace Maid
{
	class VertexShaderBufferD3D09 : public VertexShaderBuffer
	{
	public:
		virtual void Setup( const SPD3DDEVICE& pDevice )=0;
	};

	class VertexShaderBufferD3D09HARD : public VertexShaderBufferD3D09
	{
	public:
    VertexShaderBufferD3D09HARD( IDirect3DVertexShader9* p );
		virtual void Setup( const SPD3DDEVICE& pDevice );
    SPD3DVERTEXSHADERBUFFER  pBuffer;
	};

	class VertexShaderBufferD3D09_NORMAL : public VertexShaderBufferD3D09
	{
	public:
    virtual void Setup( const SPD3DDEVICE& pDevice );

  private:
	};

}

#endif