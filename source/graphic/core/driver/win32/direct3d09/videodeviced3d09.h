/*!	
	@file
	@brief	レンダリングドライバ Direct3D9 Ver.
*/

#ifndef graphic_core_driver_win32_direct3d09_videodeviced3d09_h
#define graphic_core_driver_win32_direct3d09_videodeviced3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include<vector>

#include"../../videodevice.h"

#include"../../../../../framework/win32/window.h"
#include"d3d09include.h"


namespace Maid
{
	class VideoDeviceD3D09 : public VideoDevice
	{
	public:
    VideoDeviceD3D09( IDirect3DDevice9* pDev, Window& Windw );

    virtual void Reset( const SCREENMODE& mode );
    virtual void Restore();
    virtual DEVICESTATE TestCooperativeLevel();
    virtual void Flip();
    virtual SCREENMODE GetScreenMode();


    virtual SurfaceBuffer* CreateRenderTargetSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality );
    virtual SurfaceBuffer* CreateDepthStencileSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality );
    virtual TextureBuffer* CreateTextureBuffer( const TextureBufferMemory& buffer );
    virtual TextureBuffer* CreateRenderTargetTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt );
    virtual IndexBuffer* CreateIndexBuffer( const std::vector<unt08>& data, int Format );
    virtual VertexBuffer* CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format );
    virtual VertexShaderBuffer* CreateVertexShader( const std::vector<unt08>& Code );
    virtual PixelShaderBuffer* CreatePixelShader( const std::vector<unt08>& Code );
    virtual VertexDeclarationBuffer* CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer );

    virtual void SetRenderTarget( const SurfaceBuffer* pBuffer );
    virtual void ResetRenderTarget();
    virtual void SetDepthStencil( const SurfaceBuffer* pBuffer );
    virtual void ResetDepthStencil();

    virtual void SetTextureBuffer( int stage, const TextureBuffer* pBuffer );
    virtual void SetIndexBuffer( const IndexBuffer* pBuffer );
    virtual void SetVertexBuffer( int pos, const VertexBuffer* pBuffer );
    virtual void SetVertexShader( const VertexShaderBuffer* pShader );
    virtual void SetPixelShader( const PixelShaderBuffer* pShader );
    virtual void SetVertexDeclaration( const VertexDeclarationBuffer* pDecl );
    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec );
    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ );
    virtual void SetRenderState( RENDERSTATE state, unt value );
    virtual void SetSamplerState( int Stage, SAMPLERSTATE state, unt value );
    virtual void CopySurface( const SurfaceBuffer* pSrc, SurfaceBufferMemory& Dst );
    virtual void ClearColor( const COLOR_A32B32G32R32F& Color );
    virtual void ClearZ( float val );

    virtual void BeginScene();
    virtual void EndScene();
    virtual void DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount );
    virtual void DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount );
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount );
    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  );

  protected:
    virtual void SetupDevice();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode );
    virtual void SerchEnableFormat( ENABLEFORMAT& caps );

	private:
    SPD3D09 GetD3D();

	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

		SPD3D09SURFACEBUFFER	m_pDefaultRenderTarget;
		SPD3D09SURFACEBUFFER  m_pDefaultDepthStencil;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

	private:
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd , LPARAM lp);

    struct OTHERWINDOWINFO
    {
	    HWND    hWnd;
	    RECT2DI	Rect;
    };
    std::vector<OTHERWINDOWINFO>	s_WindowInfoList;
	};
}


#endif
