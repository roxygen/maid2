/*!	
	@file
	@brief	レンダリングドライバ Direct3D9 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d09_deviced3d09_h
#define maid2_graphics_core_win32_direct3d09_deviced3d09_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include<vector>

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../auxiliary/thread/win32/threadmutex.h"
#include"common.h"

#include"object.h"

namespace Maid { namespace Graphics {

	class DeviceD3D09 : public IDevice
	{
	public:
    DeviceD3D09( IDirect3DDevice9* pDev, Window& Windw );


    void Initialize();
    void Finalize();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode );
    virtual void SerchEnableFormat( ENABLEFORMAT& caps );
    virtual void Reset( const SCREENMODE& mode );
    virtual DEVICESTATE TestCooperativeLevel();
    virtual void Flip();
    virtual SCREENMODE GetScreenMode();
    virtual void ReleaseObject( const IObject& Object );

    virtual const IVertex& CreateVertex( const CREATEVERTEXPARAM& param );
    virtual const IIndex& CreateIndex( const CREATEREINDEXPARAM& param );
    virtual const ITexture2D& CreateTexture2D( const CREATERETEXTURE2DPARAM& param );
    virtual const IConstant& CreateConstant( const CREATERECONSTANTPARAM& param );
    virtual const IRenderTarget& CreateRenderTarget( const IResource& resource, const CREATERENDERTARGETPARAM& param );
    virtual const IDepthStencil& CreateDepthStencil( const IResource& resource, const CREATEDEPTHSTENCILPARAM& param );
    virtual const IMaterial& CreateShaderMaterial( const IResource& resource, const CREATESHADERMATERIALPARAM& param );
    virtual const IVertexShader& CreateVertexShader( const void* pData, size_t Length );
    virtual const IPixelShader& CreatePixelShader( const void* pData, size_t Length );
    virtual const ISamplerState& CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual const IRasterizerState& CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual const IBlendState& CreateBlendState( const BLENDSTATEPARAM& Option );
    virtual const IInputLayout& CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );

    virtual const IDrawCommandExecute& GetDrawCommandExecute()=0;
    virtual const IDrawCommandCapture& CreateDrawCommandCapture()=0;
    virtual const IRenderTarget& GetDefaultRenderTarget();
    virtual const IDepthStencil& GetDefaultDepthStencil();

	private:
    SPD3D09 GetD3D();

    OBJECT::INSTANCEID CreateObject( const SPOBJECTINFO& pObj );


	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

    OBJECTMAP m_ObjectMap;

    std::vector<void*>   m_DeleteObjectList; //  次のフレームで削除されるオブジェクトリスト

    SPD3D09SURFACE  m_pDefaultDepthStencil;

	private:
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd , LPARAM lp);

    struct OTHERWINDOWINFO
    {
	    HWND    hWnd;
	    RECT2DI	Rect;
    };
    std::vector<OTHERWINDOWINFO>	s_WindowInfoList;
	};

}}


#endif
