#include"deviced3d10_0.h"

#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"drawcommandexecuted3d10.h"

#define COMPILE_MULTIMON_STUBS
#include <multimon.h>


#pragma comment(lib,"d3dx10.lib")

namespace Maid { namespace Graphics {

DeviceD3D10_0::DeviceD3D10_0( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, const SPDXGIADAPTER& pAdapter, Window& win )
  :m_DLL(dll)
  ,m_pFactory(pFactory)
  ,m_pAdapter(pAdapter)
  ,m_Window(win)
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

  {
    DXGI_SWAP_CHAIN_DESC desc;

    ZERO( &desc, sizeof(desc) );

    desc.BufferDesc.Width  = m_Window.GetClientSize().w;
    desc.BufferDesc.Height = m_Window.GetClientSize().h;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    desc.BufferCount = 1;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = m_Window.GetHWND();
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain* p=NULL;
    const HRESULT ret = m_pFactory->CreateSwapChain( m_pDevice.get(), &desc, &p  );
    if( FAILED(ret) ) { MAID_WARNING("IDXGIFactory::CreateSwapChain " << ret ); }
    m_pSwapChain.reset(p);
  }

  {
    HMONITOR hMon = MonitorFromWindow( m_Window.GetHWND(), MONITOR_DEFAULTTONEAREST );

    {
      MONITORINFO info;
      info.cbSize = sizeof(info);
      ::GetMonitorInfo(hMon, &info);

      m_AspectRatio.w = info.rcMonitor.bottom - info.rcMonitor.top;
      m_AspectRatio.h = info.rcMonitor.right - info.rcMonitor.left;
    }
  }

  {
    DXGI_SWAP_CHAIN_DESC desc;
    m_pSwapChain->GetDesc( &desc );

    m_ScreenMode.DisplayMode.Size = SIZE2DI( desc.BufferDesc.Width, desc.BufferDesc.Height  );
    m_ScreenMode.DisplayMode.Format = DXGI_FORMATtoPIXELFORMAT(desc.BufferDesc.Format);
    m_ScreenMode.DisplayMode.RefreshRate = 0;
    m_ScreenMode.FullScreenNo = -1;

    m_ScreenMode.IsWaitVSync = true;

  }

  m_WindowModeStyle = m_Window.GetStyle();

  m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D10(*this));
}

void DeviceD3D10_0::Finalize()
{
  m_pDevice.reset();
}

void DeviceD3D10_0::SerchDisplayMode( std::vector<DISPLAYMODE>& DisplayMode ) const
{

}


void DeviceD3D10_0::SerchDisplayInfo( std::vector<DISPLAYINFO>& info ) const
{
  MAID_ASSERT( !info.empty(), "引数はemptyにしてください" );

  for( int no=0; ; ++no )
  {
    SPDXGIOUTPUT pOut;
    {
      IDXGIOutput* p = NULL;
      const HRESULT ret = m_pAdapter->EnumOutputs(no, &p);
      if( ret==DXGI_ERROR_NOT_FOUND) { break; }
      pOut.reset(p);
    }

    DXGI_OUTPUT_DESC desc;

    const HRESULT ret = pOut->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING("IDXGIOutput::GetDesc " << ret ); continue; }

    DISPLAYINFO dat;
    dat.Name = String::ConvertUNICODEtoMAID(desc.DeviceName);
    info.push_back( dat );
  }
}


void DeviceD3D10_0::SerchEnableFormat( ENABLEFORMAT& caps ) const
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

  {
    caps.Texture.push_back(PIXELFORMAT_A08R08G08B08I);
    caps.Texture.push_back(PIXELFORMAT_A02R10G10B10I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.Texture.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.DepthStencil.push_back(PIXELFORMAT_D16I);
    caps.DepthStencil.push_back(PIXELFORMAT_D24IS8);
  }
  {
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A08R08G08B08I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A02R10G10B10I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A08R08G08B08I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A02R10G10B10I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.IndexBit.push_back(16);
  }
}



void DeviceD3D10_0::Present()
{
  const HRESULT ret = m_pSwapChain->Present( 0, 0 );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDXGISwapChain::Present()")); }
}


SCREENMODE DeviceD3D10_0::GetScreenMode() const
{
  return m_ScreenMode;
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
  return m_pDrawCommandExecute;
}

SPDRAWCOMMANDCAPTURE DeviceD3D10_0::CreateDrawCommandCapture()
{
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceD3D10_0::GetCurrentRenderTarget()const
{
  SPD3D10TEXTURE2D pTextureD3D;

  {
    ID3D10Texture2D* p = NULL;
    const HRESULT ret = m_pSwapChain->GetBuffer( 0, __uuidof(ID3D10Texture2D), (void**)&p );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDXGISwapChain::GetBuffer()")); return SPRENDERTARGET(); }
    pTextureD3D.reset(p);
  }

  CREATERETEXTURE2DPARAM texparam;
  {
    DXGI_SWAP_CHAIN_DESC desc;
    const HRESULT ret = m_pSwapChain->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDXGISwapChain::GetDesc()")); }

    texparam.Size.w = desc.BufferDesc.Width;
    texparam.Size.h = desc.BufferDesc.Height;
    texparam.Format = DXGI_FORMATtoPIXELFORMAT(desc.BufferDesc.Format);
    texparam.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
    texparam.BindFlags = Graphics::RESOURCEBINDFLAG_RENDERTARGET;
  }

  SPTEXTURE2D pTexture( new Texture2DD3D10(texparam,pTextureD3D) );

  CREATERENDERTARGETPARAM rt_param;
  rt_param.Format = texparam.Format;
  rt_param.Dimension =  CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;

  return const_cast<DeviceD3D10_0*>(this)->CreateRenderTarget( pTexture, rt_param );
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


