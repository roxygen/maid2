#include"deviced3d10_0.h"

/*
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"drawcommandexecuted3d09.h"
*/

namespace Maid { namespace Graphics {

DeviceD3D10_0::DeviceD3D10_0( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, const SPDXGIADAPTER& pAdapter, Window& win )
:m_DLL(dll), m_pFactory(pFactory), m_pAdapter(pAdapter), m_Window(win)
{

}

SPD3D10DEVICE DeviceD3D10_0::CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,UINT,ID3D10Device**);
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDevice"));

  if( createdevice==NULL ) { MAID_WARNING("load失敗"); return SPD3D10DEVICE(); }

  ID3D10Device* pDev = NULL;

  const HRESULT ret = createdevice(
    pAdapter.get(),
    D3D10_DRIVER_TYPE_HARDWARE,
    NULL,
    0,
    D3D10_SDK_VERSION,
    &pDev
    );

  if( FAILED(ret) ) { MAID_WARNING("D3D10CreateDevice()"); return SPD3D10DEVICE(); }

  return SPD3D10DEVICE(pDev);
}


void DeviceD3D10_0::Initialize()
{
  m_pDevice = CreateDevice( m_DLL, m_pAdapter );
/*
  {
    const UINT adapter = m_Adapter;
    const D3DDEVTYPE type = D3DDEVTYPE_HAL;
    const HWND hWnd = m_Window.GetHWND();
    const DWORD Flags = D3DCREATE_MULTITHREADED | (m_IsSoftwareVertexProcessing? D3DCREATE_SOFTWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING); 
    D3DPRESENT_PARAMETERS param={0};
    param.BackBufferWidth = m_Window.GetClientSize().w;
    param.BackBufferHeight = m_Window.GetClientSize().h;
    param.BackBufferFormat = D3DFMT_UNKNOWN;
    param.BackBufferCount = 1;
    param.MultiSampleType = D3DMULTISAMPLE_NONE;  //  ウィンドウモードではマルチサンプルできない
    param.MultiSampleQuality = 0;
    param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    param.hDeviceWindow = hWnd;
    param.Windowed = TRUE;
    param.EnableAutoDepthStencil = FALSE;
    param.Flags = 0;
    param.FullScreen_RefreshRateInHz = 0;
    param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    IDirect3DDevice9* p=NULL;
    HRESULT ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );
    if( FAILED(ret) ) 
    {
      //  失敗すると、param に適切な値が入っているので、もう一回作る
      ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );

      if( FAILED(ret) ) 
      {
        //  それでもダメな場合は頂点処理をソフトウェアでやってみる
        ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &p );
        if( FAILED(ret) ) 
        {
          //  これでもダメならアウト
          MAID_WARNING(MAIDTEXT("IDirect3D::CreateDevice()"));
          return ;
        }
      }
    }

    m_pDevice.reset(p);
  }

  {
    //　ウィンドウモードで初期化されているので、
    //  ここでスクリーンサイズを調べればアスペクト比がわかる
    //  ここで画面アスペクト比を調べてしまう

    D3DDISPLAYMODE mode;
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &mode );

    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }

    m_AspectRatio.w = mode.Width;
    m_AspectRatio.h = mode.Height;

  }


  m_pDevice->GetDeviceCaps( &m_DeviceCaps );

  {
    SPD3D09SWAPCHAIN pSwap;
    {
      IDirect3DSwapChain9* p;
      const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
    	if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); }
      pSwap.reset( p );
    }
    pSwap->GetPresentParameters( &m_CurrentMode );
  }
  D3DDISPLAYMODE CurrentDiplayMode;
  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  {
    const HRESULT ret = m_pDevice->GetCreationParameters( &m_CreateParams );
   	if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetCreationParameters()")); }
  }

  const D3DPRESENT_PARAMETERS& param = m_CurrentMode;

  m_ScreenMode.DisplayMode.Size = SIZE2DI(param.BackBufferWidth,param.BackBufferHeight );
  m_ScreenMode.DisplayMode.Format = D3DFORMATtoPIXELFORMAT(CurrentDiplayMode.Format);
  m_ScreenMode.DisplayMode.RefreshRate = 0;
  m_ScreenMode.IsFullScreen = false;

  m_ScreenMode.IsWaitVSync = param.PresentationInterval != D3DPRESENT_INTERVAL_IMMEDIATE;

  m_WindowModeStyle = m_Window.GetStyle();

  m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D09(*this));
*/
}

void DeviceD3D10_0::Finalize()
{
  m_pDevice.reset();
}

void DeviceD3D10_0::SerchDisplayMode( std::vector<DISPLAYMODE>& DisplayMode ) const
{

}

void DeviceD3D10_0::SerchEnableFormat( ENABLEFORMAT& caps ) const
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );


}



void DeviceD3D10_0::Present()
{

}


SCREENMODE DeviceD3D10_0::GetScreenMode() const
{
  SCREENMODE ret;
/*

  ret.DisplayMode.Size = SIZE2DI(m_CurrentMode.BackBufferWidth, m_CurrentMode.BackBufferHeight);
  ret.DisplayMode.RefreshRate = mode.RefreshRate;
  ret.DisplayMode.Format = D3DFORMATtoPIXELFORMAT(m_CurrentMode.BackBufferFormat);
  ret.IsFullScreen = m_CurrentMode.Windowed==FALSE;
  ret.IsWaitVSync  = m_CurrentMode.PresentationInterval!=D3DPRESENT_INTERVAL_IMMEDIATE;
*/
  return ret;
}

SIZE2DI DeviceD3D10_0::GetAspectRatio() const
{
  return m_AspectRatio;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::Reset() 参照
void	DeviceD3D10_0::SetScreenMode( const SCREENMODE& Mode )
{
//  m_CurrentMode = NewPresentParam;
//  m_ScreenMode  = Mode;
}


SPDRAWCOMMANDEXECUTE DeviceD3D10_0::GetDrawCommandExecute()const
{
  return SPDRAWCOMMANDEXECUTE();
}

SPDRAWCOMMANDCAPTURE DeviceD3D10_0::CreateDrawCommandCapture()
{
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceD3D10_0::GetCurrentRenderTarget()const
{

  return SPRENDERTARGET();
}



BOOL CALLBACK DeviceD3D10_0::EnumWindowsProc(HWND hWnd , LPARAM lp) 
{
	OTHERWINDOWINFO	info;
	RECT			rc;

	::GetWindowRect( hWnd, &rc );

	info.hWnd = hWnd;
	info.Rect.x = rc.left;
	info.Rect.y = rc.top;
	info.Rect.w = rc.right - rc.left;
	info.Rect.h = rc.bottom- rc.top;

	std::vector<OTHERWINDOWINFO>* pList = (std::vector<OTHERWINDOWINFO>*)lp;

	pList->push_back(info);

	return TRUE;
}

}}


