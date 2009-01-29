#include"deviced3d09.h"

#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"drawcommandexecuted3d09.h"
#include"id3d09object.h"

namespace Maid { namespace Graphics {

DeviceD3D09::DeviceD3D09( const SPD3D09& pD3D, UINT adapter, bool IsSoftwareVertexProcessing, Window& win )
:m_pDirect3D09(pD3D), m_Adapter(adapter), m_IsSoftwareVertexProcessing(IsSoftwareVertexProcessing), m_Window(win)
{
  ZERO( &m_CreateParams, sizeof(m_CreateParams) );
  ZERO( &m_DeviceCaps, sizeof(m_DeviceCaps) );
  ZERO( &m_CurrentMode, sizeof(m_CurrentMode) );
}



void DeviceD3D09::Initialize()
{
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
}

void DeviceD3D09::Finalize()
{
  m_pDevice.reset();
}

void DeviceD3D09::SerchDisplayMode( std::vector<DISPLAYMODE>& DisplayMode ) const
{
  MAID_ASSERT( !DisplayMode.empty(), "引数はemptyにしてください" );

  static const D3DFORMAT DISPLAYFMT[] =
  {
    //  作成できるバックバッファの列挙。
    //  これはD3D9の仕様で決まっている。
    D3DFMT_R5G6B5,
    D3DFMT_X1R5G5B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_X8R8G8B8,
    D3DFMT_A8R8G8B8,
    D3DFMT_A2R10G10B10,
  };

  SPD3D09 pD3D = GetD3D();
  const UINT ADAPTERID = m_CreateParams.AdapterOrdinal;

  for( int displayno=0; displayno<NUMELEMENTS(DISPLAYFMT); ++displayno )
  {
    const D3DFORMAT AdapterFormat = DISPLAYFMT[displayno];
    const int ModeCount = pD3D->GetAdapterModeCount( ADAPTERID, AdapterFormat );

    for( int ModeNo=0; ModeNo<ModeCount; ++ModeNo )
    {
      D3DDISPLAYMODE mode;
      const HRESULT ret = pD3D->EnumAdapterModes( ADAPTERID, AdapterFormat, ModeNo, &mode );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3D::EnumAdapterModes()")); }

      DISPLAYMODE m;

      m.Size = SIZE2DI(mode.Width,mode.Height);
      m.Format = D3DFORMATtoPIXELFORMAT(mode.Format);
      m.RefreshRate = mode.RefreshRate;

      DisplayMode.push_back( m );
    }
  }
}

void DeviceD3D09::SerchEnableFormat( ENABLEFORMAT& caps ) const
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

 #pragma  COMPILERMSG( "TODO:コメントアウトしてあるフォーマットはそのうち対応する" )

  static const D3DFORMAT s_TEXTURE_FORMAT[] =
  {
    //  作成できるテクスチャフォーマット,RenderSurface,RenderTextueの限界。
//    D3DFMT_A8,
    D3DFMT_P8,
//    D3DFMT_R3G3B2,

    D3DFMT_A8P8,
    D3DFMT_R5G6B5,
    D3DFMT_X1R5G5B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_A4R4G4B4,
    D3DFMT_A8R3G3B2,
    D3DFMT_X4R4G4B4,

    D3DFMT_R8G8B8,

    D3DFMT_A8R8G8B8,
    D3DFMT_X8R8G8B8,
    D3DFMT_A8B8G8R8,
    D3DFMT_X8B8G8R8,
    D3DFMT_A2B10G10R10,
    D3DFMT_A2R10G10B10,
//    D3DFMT_G16R16,

    D3DFMT_A16B16G16R16,

    D3DFMT_DXT1,
    D3DFMT_DXT2,
    D3DFMT_DXT3,
    D3DFMT_DXT4,
    D3DFMT_DXT5,
    D3DFMT_R16F,
    D3DFMT_G16R16F,
    D3DFMT_A16B16G16R16F,
//    D3DFMT_R32F,
//    D3DFMT_G32R32F,
    D3DFMT_A32B32G32R32F,
  };

  static const D3DFORMAT s_DEPTH_STENCIL_FORMAT[] =
  {
    //  作成できる深度バッファの限界。
    D3DFMT_D32,
    D3DFMT_D15S1,
    D3DFMT_D24S8,
    D3DFMT_D24X8,
    D3DFMT_D24X4S4,
    D3DFMT_D24FS8,
    D3DFMT_D16,
  };

  static const D3DFORMAT s_INDEX_FORMAT[] =
  {
    //  作成できるインデックスバッファの限界。
    D3DFMT_INDEX16,
    D3DFMT_INDEX32,
  };

  SPD3D09 pD3D = GetD3D();
  const UINT AdapterID = m_CreateParams.AdapterOrdinal;
  const D3DDEVTYPE DeviceType = m_CreateParams.DeviceType;
  D3DDISPLAYMODE CurrentDiplayMode;
  D3DCAPS9 d3dCaps;

  {
    const HRESULT ret = m_pDevice->GetDeviceCaps( &d3dCaps );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDeviceCaps()")); }
  }

  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  {
    //  作成できるテクスチャのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      0, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.Texture.push_back(f);  }
      }
    }

    caps.MaxTextureSize = SIZE2DI(d3dCaps.MaxTextureWidth,d3dCaps.MaxTextureHeight);
  }

  {
    //  作成できるレンダーテクスチャのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );
      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetTexture.push_back(f);  }
      }
    }
  }
  {
    //  作成できるレンダーサーフェスのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetSurface.push_back(f);  }
      }
    }
  }

  {
    //  作成できるDepthStencilフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_DEPTH_STENCIL_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_DEPTH_STENCIL_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.DepthStencil.push_back(f);  }
      }
    }
  }

  {
    //  作成できるindexフォーマットを調べる
    if( d3dCaps.MaxVertexIndex <= 0x0000FFFF )
    {
      caps.IndexBit.push_back(16); 
    }else
    {
      caps.IndexBit.push_back(16); 
      caps.IndexBit.push_back(32); 
    }
  }
}


const SPD3D09& DeviceD3D09::GetD3D()const
{
  return m_pDirect3D09;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! GraphicsDeviceD3D09::Flip() を参照
void DeviceD3D09::Present()
{
	const HRESULT ret = m_pDevice->Present( NULL, NULL, NULL, NULL );

  switch( ret )
  {
  case D3DERR_DRIVERINTERNALERROR:
  case D3DERR_DEVICELOST:
    {
      switch( m_pDevice->TestCooperativeLevel() )
      {
      case D3DERR_DEVICELOST:     { }break;
      case D3DERR_DEVICENOTRESET: 
        {
          SetScreenMode(m_ScreenMode);
        }break;

        /* そのほかは正常 */
      }

    }break;

  case D3DERR_INVALIDCALL:
    {
      MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()"));
    }break;

  }
}


SCREENMODE DeviceD3D09::GetScreenMode() const
{
  D3DDISPLAYMODE mode={0};

  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &mode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  SCREENMODE ret;

  ret.DisplayMode.Size = SIZE2DI(m_CurrentMode.BackBufferWidth, m_CurrentMode.BackBufferHeight);
  ret.DisplayMode.RefreshRate = mode.RefreshRate;
  ret.DisplayMode.Format = D3DFORMATtoPIXELFORMAT(m_CurrentMode.BackBufferFormat);
  ret.IsFullScreen = m_CurrentMode.Windowed==FALSE;
  ret.IsWaitVSync  = m_CurrentMode.PresentationInterval!=D3DPRESENT_INTERVAL_IMMEDIATE;

  return ret;
}

SIZE2DI DeviceD3D09::GetAspectRatio() const
{
  return m_AspectRatio;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::Reset() 参照
void	DeviceD3D09::SetScreenMode( const SCREENMODE& Mode )
{
	for( int i=0; i<8; ++i ) { m_pDevice->SetTexture( i, NULL ); }
	for( int i=0; i<8; ++i ) { m_pDevice->SetStreamSource( i, NULL, 0, 0 ); }

	m_pDevice->SetIndices( NULL );
	m_pDevice->SetPixelShader( NULL );
	m_pDevice->SetVertexShader( NULL );
	m_pDevice->SetVertexDeclaration( NULL );

  {
    //  vramにしかないやつを全部退避する
    ID3D09Object::EscapeAll();
  }

  //  ウィンドウ -> フルスクリーンのときは
  //  その他のウィンドウの配置座標を保存しておく
  if( !m_ScreenMode.IsFullScreen && Mode.IsFullScreen )
  {
    {
      m_WindowInfoList.clear();
      EnumWindows( EnumWindowsProc, (LPARAM)&m_WindowInfoList );

      //  このままだと、 m_Window も含まれてしまうので、探し出して削除する
      for( std::vector<OTHERWINDOWINFO>::iterator ite=m_WindowInfoList.begin();
                                                  ite!=m_WindowInfoList.end();
                                                  ++ite )
      {
        if( ite->hWnd==m_Window.GetHWND() )
        {
          m_WindowInfoList.erase( ite );
          break;
        }
      }
    }

    { //  ウィンドウの変更も忘れずに
      m_WindowModeStyle = m_Window.GetStyle();
      m_Window.SetStyle( WS_POPUP );
      m_Window.SetZOrder( HWND_TOPMOST );
    }
  }

  D3DPRESENT_PARAMETERS NewPresentParam;
  { //  これでやっと解像度変更
    ConvertPresentParam( Mode, NewPresentParam );

	  const HRESULT ret = m_pDevice->Reset( &NewPresentParam );
    if( ret==D3DERR_DRIVERINTERNALERROR ) { MAID_WARNING("ドライバエラーです。復帰できません"); return; }
    ef( FAILED(ret) ) { MAID_WARNING("IDirect3DDevice9::Reset()"); return; }
  }
  m_Window.SetClientSize( Mode.DisplayMode.Size );

  //  フルスクリーン -> ウィンドウのときは
  //  各種ウィンドウを再配置＆自分のスタイルの変更
  if( m_ScreenMode.IsFullScreen && !Mode.IsFullScreen )
  {
		for( std::vector<OTHERWINDOWINFO>::iterator ite=m_WindowInfoList.begin();
					ite!=m_WindowInfoList.end(); ++ite )
		{
			OTHERWINDOWINFO& info = *ite;
			::MoveWindow( info.hWnd, info.Rect.x,  info.Rect.y, info.Rect.w, info.Rect.h, TRUE );
		}

    {
      m_Window.SetStyle( m_WindowModeStyle );
      m_Window.SetZOrder( HWND_NOTOPMOST );
    }
  }

  {
    //  で、戻ってきてもらうー
    ID3D09Object::RestoreAll( *this );
  }

	m_pDevice->GetDeviceCaps(&m_DeviceCaps);
  m_CurrentMode = NewPresentParam;
  m_ScreenMode  = Mode;
}


SPDRAWCOMMANDEXECUTE DeviceD3D09::GetDrawCommandExecute()const
{
  return m_pDrawCommandExecute;
}

SPDRAWCOMMANDCAPTURE DeviceD3D09::CreateDrawCommandCapture()
{
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceD3D09::GetCurrentRenderTarget()const
{
  SPD3D09SURFACE pTarget;
  {
    SPD3D09SWAPCHAIN pSwap;
    {
      IDirect3DSwapChain9* p=NULL;
      const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
    	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); goto MAID_ERROR; }
      pSwap.reset( p );
    }

    IDirect3DSurface9* p=NULL;
    const HRESULT ret = pSwap->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &p );
   	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetBackBuffer()")); goto MAID_ERROR; }
    pTarget.reset( p );
  }
  {
    D3DSURFACE_DESC desc;
    pTarget->GetDesc( &desc );

    SPTEXTURE2D pTex;
    {
      CREATERETEXTURE2DPARAM param;

      {
        param.Size = SIZE2DI(desc.Width,desc.Height);
        param.ArraySize = 1;
        param.MipLevels = 0;
        param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
        param.Sample.Count   = desc.MultiSampleType;
        param.Sample.Quality = desc.MultiSampleQuality;
        param.BindFlags = D3DUSAGEtoRESOURCEBINDFLAG(desc.Usage);
        param.Usage = RESOURCEUSAGE_GPUWRITE;
        param.CPUAccessFlags = 0;
        param.MiscFlags = 0;
      }

      pTex.reset( new Texture2DD3D09(param,pTarget,true) );
    }

    SPRENDERTARGET pRet;
    {
      CREATERENDERTARGETPARAM param;
      {
        param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
        param.Dimension = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;
      }
      pRet.reset( new RenderTargetD3D09(pTex,param) );
    }

    return pRet;
  }

MAID_ERROR:
  return SPRENDERTARGET();
}


void DeviceD3D09::ConvertPresentParam( const SCREENMODE& ModeLib, D3DPRESENT_PARAMETERS& ModeD3D )
{
  ModeD3D.BackBufferWidth  = ModeLib.DisplayMode.Size.w;
  ModeD3D.BackBufferHeight = ModeLib.DisplayMode.Size.h;
  ModeD3D.BackBufferFormat = PIXELFORMATtoD3DFORMAT(ModeLib.DisplayMode.Format);
  ModeD3D.BackBufferCount = 1;
  ModeD3D.MultiSampleType = D3DMULTISAMPLE_NONE;
  ModeD3D.MultiSampleQuality = 0;
  ModeD3D.SwapEffect = D3DSWAPEFFECT_DISCARD;
  ModeD3D.hDeviceWindow = m_Window.GetHWND();
  ModeD3D.Windowed = ModeLib.IsFullScreen? FALSE: TRUE;
  ModeD3D.EnableAutoDepthStencil = FALSE;
  ModeD3D.Flags = 0;
//  param.FullScreen_RefreshRateInHz = ModeLib.DisplayMode.RefreshRate;
  ModeD3D.FullScreen_RefreshRateInHz = 0;
  ModeD3D.PresentationInterval = ModeLib.IsWaitVSync? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
}


BOOL CALLBACK DeviceD3D09::EnumWindowsProc(HWND hWnd , LPARAM lp) 
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


