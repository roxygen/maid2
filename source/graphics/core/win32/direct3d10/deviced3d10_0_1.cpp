﻿#include"deviced3d10_0.h"

#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"drawcommandexecuted3d10.h"



namespace Maid { namespace Graphics {

DeviceD3D10_0::DeviceD3D10_0( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, Window& win )
  :m_DLL(dll)
  ,m_pAdapter(pAdapter)
  ,m_Window(win)
  ,m_ShaderCompilerLast(NULL)
  ,m_ShaderCompilerDefault(NULL)
{

}

FUNCTIONRESULT DeviceD3D10_0::CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,UINT,DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D10Device**);
  FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDeviceAndSwapChain"));

  IDXGISwapChain* p=NULL;
  ID3D10Device* pDev = NULL;

  const HRESULT ret = createdevice(
    pAdapter.get(),
    D3D10_DRIVER_TYPE_HARDWARE,
    NULL,
    0,
    D3D10_SDK_VERSION,
    &desc,
    &p,
    &pDev
    );

  if( FAILED(ret) ) { MAID_WARNING("D3D10CreateDeviceAndSwapChain()"); return FUNCTIONRESULT_ERROR; }

  pDevice.reset(pDev);
  pSwapChain.reset(p);

  return FUNCTIONRESULT_OK;
}


void DeviceD3D10_0::Initialize()
{
  {
    const char* filename[] =
    {
      "d3dx10_41.dll",
      "d3dx10_42.dll",
      "d3dx10_40.dll",
      "d3dx10_39.dll",
      "d3dx10_38.dll",
      "d3dx10_37.dll",
      "d3dx10_36.dll",
      "d3dx10_35.dll",
      "d3dx10_34.dll",
      "d3dx10_33.dll",
    };

    for( int i=0; i<NUMELEMENTS(filename); ++i )
    {
      if( m_D3DX10_LastDLL.Load( String::ConvertSJIStoMAID(filename[i]) ) == DllWrapper::LOADRETURN_ERROR ) { continue; }

      m_ShaderCompilerDefault = (SHADERCOMPILE)m_D3DX10_LastDLL.GetProcAddress( MAIDTEXT("D3DX10CompileFromMemory") );
      break;
    }

    if( m_ShaderCompilerDefault==NULL )
    {
      ::MessageBox( NULL, L"D3DX10.dllが見つかりませんでした。\nDirectXランタイムをインストールしてください。", L"エラー", MB_OK );
      MAID_WARNING( "d3dx10_XX.dllがありません" );
    }

    m_ShaderCompilerLast = m_ShaderCompilerDefault;
  }

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
    desc.BufferCount = 2;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = m_Window.GetHWND();
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    const FUNCTIONRESULT ret = CreateDeviceAndSwapChain( m_DLL, m_pAdapter, desc, m_pDevice, m_pSwapChain );

    if( FUNCTIONRESULT_FAILE(ret) ) { return ; }

    {
      IDXGIFactory* p = NULL;
      m_pSwapChain->GetParent(__uuidof(IDXGIFactory), (void**)&p );
      m_pFactory.reset(p);
    }
  }

  {
    //  D3D10はビューポートの設定を自動的にやってくれない
    D3D10_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width    = m_Window.GetClientSize().w;
    vp.Height   = m_Window.GetClientSize().h;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    m_pDevice->RSSetViewports( 1, &vp );
  }


  m_SyncInterval = 1;

  UpdateDisplayAspect();
  m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D10(*this));
}



void DeviceD3D10_0::Finalize()
{
  if( IsFullScreen() ) { m_pSwapChain->SetFullscreenState( FALSE, NULL ); }
  m_pSwapChain.reset();

  m_pDevice.reset();
}



void DeviceD3D10_0::UpdateDisplayAspect()
{
  SPDXGIOUTPUT  pOutput;
  {
    IDXGIOutput* p;
    const HRESULT ret = m_pSwapChain->GetContainingOutput( &p );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetContainingOutput() " ); return ; }

    pOutput.reset(p);
  }

  DXGI_OUTPUT_DESC desc;
  const HRESULT ret = pOutput->GetDesc( &desc );
  if( FAILED(ret) ) { MAID_WARNING("IDXGIOutput::GetDesc() " ); return ; }
  
  MONITORINFO info;
  info.cbSize = sizeof(info);
  ::GetMonitorInfo(desc.Monitor, &info);

  const int w = info.rcMonitor.right - info.rcMonitor.left;
  const int h = info.rcMonitor.bottom - info.rcMonitor.top;

  m_DisplayAspect = SIZE2DI(w,h);
}



void DeviceD3D10_0::SetFullScreenState( bool IsFull )
{
  const bool NowFull = IsFullScreen();


  if( NowFull==IsFull ) { return ; }

  if( IsFull )
  {
    //  ウィンドウ -> フルスクリーンの時は、対象先の解像度を覚えておく
    UpdateDisplayAspect();
  }
  {
    const BOOL full = IsFull? TRUE:FALSE;
    const HRESULT ret = m_pSwapChain->SetFullscreenState( full, NULL );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::SetFullscreenState() " ); return ; }
  }
}

float DeviceD3D10_0::GetTexelMapValue() const
{
  return 0.0f;
}


void DeviceD3D10_0::SerchSwapChainSurfaceFormat( std::vector<PIXELFORMAT>& mode )  const
{
  static const DXGI_FORMAT TestFMT[] =
  {
    DXGI_FORMAT_R32G32B32A32_FLOAT,
    DXGI_FORMAT_R16G16B16A16_FLOAT,
    DXGI_FORMAT_R16G16B16A16_UNORM,
    DXGI_FORMAT_R10G10B10A2_UNORM,
    DXGI_FORMAT_R8G8B8A8_UNORM,
  };

  for( int i=0; i<NUMELEMENTS(TestFMT); ++i )
  {
    const DXGI_FORMAT fmt = TestFMT[i];
    UINT flag = 0;

    const HRESULT ret = m_pDevice->CheckFormatSupport( fmt, &flag );
    if( FAILED(ret) ) { continue; }

    const bool IsTextre2D = IsFlag( flag, D3D10_FORMAT_SUPPORT_TEXTURE2D );
    const bool IsRenderTarget = IsFlag( flag, D3D10_FORMAT_SUPPORT_RENDER_TARGET );

    if( IsTextre2D && IsRenderTarget )
    {
      mode.push_back( DXGI_FORMATtoPIXELFORMAT(fmt) );
    }
  }
}

void DeviceD3D10_0::SetSwapChainFormat( const SWAPCHAINFORMAT& mode )
{
  DXGI_SWAP_CHAIN_DESC desc;
  const bool NowFull = IsFullScreen();

  {
    const HRESULT ret = m_pSwapChain->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return ; }

    //  一回ウィンドウモードに戻してから開放しないとダメらしい
    if( NowFull ) { m_pSwapChain->SetFullscreenState( FALSE, NULL ); }
    m_pSwapChain.reset();
  }

  m_Window.SetClientSize( mode.Size );

  desc.BufferDesc.Width  = mode.Size.w;
  desc.BufferDesc.Height = mode.Size.h;
  desc.BufferDesc.RefreshRate.Numerator = 60;
  desc.BufferDesc.RefreshRate.Denominator = 1;
  desc.BufferDesc.Format = PIXELFORMATtoDXGI_FORMAT(mode.Format);
  desc.BufferDesc.ScanlineOrdering = SWAPCHAINFORMATSCANLINEORDERtoDXGI_MODE_SCANLINE_ORDER(mode.ScanlineOrder);
  desc.BufferDesc.Scaling = SWAPCHAINFORMATSCALINGtoDXGI_MODE_SCALING(mode.Scaling);
  desc.BufferCount = 2;

  desc.SampleDesc.Count   = mode.MultiSample.Count;
  desc.SampleDesc.Quality = mode.MultiSample.Quality;


  {
    IDXGISwapChain* p=NULL;
    const HRESULT ret = m_pFactory->CreateSwapChain( m_pDevice.get(), &desc, &p  );
    if( FAILED(ret) ) { MAID_WARNING("IDXGIFactory::CreateSwapChain " << ret ); }
    m_pSwapChain.reset(p);
  }
  
  if( NowFull ) { m_pSwapChain->SetFullscreenState( TRUE, NULL ); }

  m_SyncInterval = mode.SyncInterval;
}


SWAPCHAINFORMAT DeviceD3D10_0::GetSwapChainFormat() const
{
  SWAPCHAINFORMAT ret;

  DXGI_SWAP_CHAIN_DESC desc;

  {
    const HRESULT ret = m_pSwapChain->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return SWAPCHAINFORMAT(); }
  }

  ret.Size   = SIZE2DI( desc.BufferDesc.Width, desc.BufferDesc.Height );
  ret.Format = DXGI_FORMATtoPIXELFORMAT(desc.BufferDesc.Format);
  ret.MultiSample.Quality = desc.SampleDesc.Quality;
  ret.MultiSample.Count   = desc.SampleDesc.Count;
  ret.RefreshRate.Numerator   = desc.BufferDesc.RefreshRate.Numerator;
  ret.RefreshRate.Denominator = desc.BufferDesc.RefreshRate.Denominator;

  ret.Scaling       = DXGI_MODE_SCALINGtoDISPLAYMODESCALING( desc.BufferDesc.Scaling );
  ret.ScanlineOrder = DXGI_MODE_SCANLINE_ORDERtoSWAPCHAINFORMATSCANLINEORDER( desc.BufferDesc.ScanlineOrdering );

  ret.SyncInterval = m_SyncInterval;

  return ret;
}

SIZE2DI DeviceD3D10_0::GetDisplayAspect() const
{
  return m_DisplayAspect;
}


void DeviceD3D10_0::SerchEnableFormat( ENABLEFORMAT& caps ) const
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

  {
    caps.Texture.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.Texture.push_back(PIXELFORMAT_A02B10G10R10I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.Texture.push_back(PIXELFORMAT_A32B32G32R32F);

    caps.MaxTextureSize = SIZE2DI(4096,4096);
  }
  {
    caps.DepthStencil.push_back(PIXELFORMAT_D16I);
    caps.DepthStencil.push_back(PIXELFORMAT_D24IS8);
  }
  {
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A02B10G10R10I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A02B10G10R10I);
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
  const HRESULT ret = m_pSwapChain->Present( m_SyncInterval, 0 );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDXGISwapChain::Present()")); }
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
    texparam.ArraySize = 1;
    texparam.MipLevels = 1;
  }

  SPTEXTURE2D pTexture( new Texture2DD3D10(texparam,pTextureD3D) );

  CREATERENDERTARGETPARAM rt_param;
  rt_param.Format = texparam.Format;
  rt_param.Dimension =  CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;

//  return const_cast<DeviceD3D10_0*>(this)->CreateRenderTarget( pTexture, rt_param );
  return const_cast<DeviceD3D10_0*>(this)->CreateRenderTarget( pTexture, NULL );
}




bool DeviceD3D10_0::IsFullScreen() const
{
  DXGI_SWAP_CHAIN_DESC desc;
  const HRESULT ret = m_pSwapChain->GetDesc( &desc );
  if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return false; }

  return desc.Windowed == FALSE;
}

}}


