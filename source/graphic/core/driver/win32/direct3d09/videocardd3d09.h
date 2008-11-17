/*!	
	@file
	@brief	レンダリングドライバ Direct3D9 Ver.
*/

#ifndef graphic_core_driver_win32_direct3d09_videocarddirect3d09_h
#define graphic_core_driver_win32_direct3d09_videocarddirect3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include<vector>

#include"../../videocard.h"

#include"../../../../../framework/win32/window.h"
#include"direct3d09include.h"


namespace Maid
{
	class VideoCardDirect3D09 : public VideoCard
	{
	public:
    VideoCardDirect3D09( const SPD3DDEVICE& pDev, Window& Windw );

    virtual void Reset( const SCREENMODE& mode );
    virtual void Restore();
    virtual DEVICESTATE TestCooperativeLevel();
    virtual void Flip();
    virtual SPSURFACEBUFFER CreateSurface( const SIZE2DI& size, PIXELFORMAT fmt );
    virtual SPTEXTUREBUFFER CreateTextureBuffer( const TextureBufferMemory& buffer );
    virtual SPTEXTUREBUFFER CreateRTTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt );
    virtual SPINDEXBUFFER CreateIndexBuffer( const std::vector<unt08>& data, int Format );
    virtual SPVERTEXBUFFER CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format );
    virtual SPVERTEXSHADERBUFFER CreateVertexShader( const std::vector<unt08>& Code );
    virtual SPPIXELSHADERBUFFER CreatePixelShader( const std::vector<unt08>& Code );
    virtual SPVERTEXDECLARATIONBUFFER CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer );
    virtual void SetRenderTarget( const SurfaceBuffer* pBuffer );
    virtual void ResetRenderTarget();
    virtual void SetDepthStencil( const SurfaceBuffer* pBuffer );
    virtual void ResetDepthStencil();
    virtual void UnsetTextureBuffer( int stage );
    virtual void SetTextureBuffer( const SPTEXTUREBUFFER& pBuffer, int stage );
    virtual void SetIndexBuffer( const SPINDEXBUFFER& pBuffer );
    virtual void SetVertexBuffer( const SPVERTEXBUFFER& pBuffer, int pos );
    virtual void SetVertexShader( const SPVERTEXSHADERBUFFER& pShader );
    virtual void SetPixelShader( const SPPIXELSHADERBUFFER& pShader );
    virtual void SetVertexDeclaration( const SPVERTEXDECLARATIONBUFFER& pDecl );
    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec );
    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;
    virtual void CopySurface( const SPSURFACEBUFFER& pSrc, SPSURFACEBUFFERMEMORY& pDst );
    virtual String GetVideoInfo();
    virtual void SetRenderState( RENDERSTATE state, unt value );
    virtual void SetSamplerState( int Stage, SAMPLERSTATE state, unt value );
    virtual void BeginScene();
    virtual void EndScene();
    virtual void ClearColor( const COLOR_A32B32G32R32F& Color );
    virtual void ClearZ( float val );
    virtual void DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount );
    virtual void DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount );
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount );
    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  );

  protected:
    virtual void SerchVideoCaps( VIDEOCAPS& caps );

	private:
		Window&	      m_Window;
		SPD3DDEVICE   m_pDevice;		      //!<	レンダリングデバイス

    D3DPRESENT_PARAMETERS		m_DeviceInfo;   //!<	作成したデバイス情報
		D3DCAPS9                m_DeviceCaps;

		bool		m_IsSoftTnL;	//	頂点処理を ソフトウェアで行うか？

		SPD3DSURFACEBUFFER	m_pDefaultRenderTarget;
		SPD3DSURFACEBUFFER	m_pDefaultDepthStencil;

		SCREENMODE			m_ScreenMode;

		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;
	};
}


#endif
