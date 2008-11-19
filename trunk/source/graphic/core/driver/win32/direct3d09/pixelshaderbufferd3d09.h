/*!
    @file
    @brief	ピクセルシェーダーバッファ D3D9 Ver.
 */

#ifndef graphic_core_driver_win32_direct3d09_pixelshaderbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_pixelshaderbufferd3d09_h



#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include"../../pixelshaderbuffer.h"

#include"d3d09include.h"


namespace Maid
{
	class PixelShaderBufferD3D09 : public PixelShaderBuffer
	{
	public:
		virtual void Setup( const SPD3DDEVICE& pDevice )=0;
	};

	class PixelShaderBufferD3D09HARD : public PixelShaderBufferD3D09
	{
	public:
    PixelShaderBufferD3D09HARD( IDirect3DPixelShader9* p );
		virtual void Setup( const SPD3DDEVICE& pDevice );
    SPD3DPIXELSHADERBUFFER  pBuffer;
	};

	class PixelShaderBufferD3D09_NORMAL : public PixelShaderBufferD3D09
	{
	public:
    virtual void Setup( const SPD3DDEVICE& pDevice );

  private:
	};

}

#endif