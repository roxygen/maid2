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
    virtual void SetScreenMode( const SCREENMODE& mode );
    virtual void Present();
    virtual SCREENMODE GetScreenMode();
    virtual void DeleteObject( const hObject& Object );
    virtual void GetObjectDesc( const hObject& Object, ObjectDesc& desc );


    virtual hVertex CreateVertex( const CREATEVERTEXPARAM& param );
    virtual hIndex CreateIndex( const CREATEREINDEXPARAM& param );
    virtual hTexture2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param );
    virtual hConstant CreateConstant( const CREATERECONSTANTPARAM& param );

    virtual hRenderTarget CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param );
    virtual hDepthStencil CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param );
    virtual hMaterial CreateMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param );
    virtual hVertexShader CreateVertexShader( const void* pData, size_t Length );
    virtual hPixelShader CreatePixelShader( const void* pData, size_t Length );
    virtual hSamplerState CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual hRasterizerState CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual hBlendState CreateBlendState( const BLENDSTATEPARAM& Option );
    virtual hInputLayout CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );

    virtual IDrawCommandExecute* CreateDrawCommandExecute();
    virtual IDrawCommandCapture* CreateDrawCommandCapture();

    virtual hRenderTarget GetDefaultRenderTarget();
    virtual hDepthStencil GetDefaultDepthStencil();

    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage );


    const SPD3D09DEVICE& GetDevice() const { return m_pDevice; }
    const D3DDEVICE_CREATION_PARAMETERS& GetD3DDEVICE_CREATION_PARAMETERS() const { return m_CreateParams; }
    const D3DCAPS9& GetD3DCAPS9() const { return m_DeviceCaps; }
    const D3DPRESENT_PARAMETERS& GetD3DPRESENT_PARAMETERS() const { return m_CurrentMode; }
    const OBJECTDESCMAP& GetOBJECTDESCMAP() const { return m_ObjectDescMap; }

	private:
    SPD3D09 GetD3D();

    void UpdateDeleteObject();

    void RegistObject( void* p, const SPOBJECTDESC& pDesc );
    void DeleteObject( hObject::INSTANCEID id );

	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

    SPD3D09SURFACE  m_pDefaultRenderTarget;
    SPD3D09SURFACE  m_pDefaultDepthStencil;


	private:  //  スレッドをまたぐメンバ
    ThreadMutex   m_ObjectMutex;
    OBJECTDESCMAP m_ObjectDescMap;

    std::vector<hObject>   m_DeleteObjectList; //  次のフレームで削除されるオブジェクトリスト


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
