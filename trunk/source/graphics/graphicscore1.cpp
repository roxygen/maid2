#include"graphicscore.h"


//  2の累乗を返すマクロ
inline int ExtendSize( int src )
{
  int now = 1;
  for( int i=0; i<20; ++i ) //  2^20
  {
    if( src<=now ) { return now; }
    now *= 2;
  }

  return now;
}

namespace Maid {

GraphicsCore*  GlobalPointer<GraphicsCore>::s_pPointer;

  /*!
	 	@class Core core.h
	 	@brief  グラフィック周りを管理するクラス
	 */


GraphicsCore::GraphicsCore()
{
}

GraphicsCore::~GraphicsCore()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
	@param	pDriver	[i ]	使うドライバ
 */
void GraphicsCore::Initialize( const Graphics::SPDEVICE& pDevice, const Graphics::SPFONTDEVICE& pFont )
{
  GlobalPointer<GraphicsCore>::Set();

	m_pDevice = pDevice;
	m_pDevice->Initialize();
	m_pDevice->SerchDisplayMode( m_EnableDisplayMode );

  m_pFontDevice = pFont;
  UpdateMember();
}

void GraphicsCore::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

	m_pDevice->Finalize();
	m_pDevice.reset();
  GlobalPointer<GraphicsCore>::Clear();
}

const Graphics::SPDEVICE& GraphicsCore::GetDevice()const
{
  return m_pDevice;
}

const Graphics::SPFONTDEVICE& GraphicsCore::GetFontDevice()const
{
  return m_pFontDevice;
}

const GraphicsCore::SCREENMODE& GraphicsCore::GetScreenMode() const
{
  return m_ScreenMode;
}

void GraphicsCore::SetScreenMode( const SCREENMODE& mode )
{
  const std::vector<Graphics::DISPLAYMODE>& ModeList = m_EnableDisplayMode;

  //  条件が一致するフォーマットで行う
  //GraphicsCore::DISPLAYFORMAT::TYPE GraphicsCore::ConvertType( PIXELFORMAT fmt ) const
  for( size_t i=0; i<ModeList.size(); ++i )
  {
    const Graphics::DISPLAYMODE& src = ModeList[i];
    const DISPLAYFORMAT& dst = mode.Format;

    if( dst.Type!=ConvertType(src.Format) ) { continue; }
    if( dst.Size!=src.Size ) { continue; }

    //  リフレッシュレートを０指定した場合は、勝手に行う
    if( dst.RefreshRate!=0 )
    {
      if( dst.RefreshRate!=src.RefreshRate ) { continue; }
    }
    
    Graphics::SCREENMODE new_mode;
    new_mode.DisplayMode  = src;
    new_mode.IsFullScreen = mode.IsFullScreen;
    new_mode.IsWaitVSync  = mode.IsWaitVSync;

	  m_pDevice->SetScreenMode(new_mode);
    break;
  }
}

void GraphicsCore::EnumDisplayFormat( std::vector<DISPLAYFORMAT>& dstlist ) const
{
  dstlist.clear();

  const std::vector<Graphics::DISPLAYMODE>& ModeList = m_EnableDisplayMode;

  for( size_t i=0; i<ModeList.size(); ++i )
  {
    const Graphics::DISPLAYMODE& src = ModeList[i];
    DISPLAYFORMAT dst;

    dst.Type = ConvertType(src.Format);
    dst.Size = src.Size;
    dst.RefreshRate = src.RefreshRate;

    dstlist.push_back( dst );
  }
}


const SIZE2DI& GraphicsCore::GetMaxTextureSize() const
{
  return m_EnableFormat.MaxTextureSize;
}

SIZE2DI GraphicsCore::CalcTextureSize( const SIZE2DI& size ) const
{
  SIZE2DI ret(0,0);

  const SIZE2DI& SRCSIZE = size;
  const SIZE2DI  MAXSIZE = GetMaxTextureSize();
  ret.w = std::min( ExtendSize(SRCSIZE.w), MAXSIZE.w );
  ret.h = std::min( ExtendSize(SRCSIZE.h), MAXSIZE.h );

  return ret;
}



PIXELFORMAT GraphicsCore::FindFormatTexture2D( PIXELFORMAT fmt, bool IsRenderTarget )const
{
  const PIXELFORMATLIST& fmtlist = IsRenderTarget? m_TextureFormat : m_RenderTargetFormat;

  //  同じフォーマットがあるならそれ
  if( IsExist( fmtlist, fmt ) ) { return fmt; }

  //  ないなら似たようなフォーマットを返す
  //  アルファ無しフォーマットはアルファありフォーマットで代用できるけど
  //  アルファありは無しで代用できないので注意

#define cmp( fmt ) { if( IsExist( fmtlist, fmt ) ) { return fmt; } }
  switch( fmt )
  {
  case PIXELFORMAT_P08X08R08G08B08I:
    {
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);

      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);

    }break;

  case PIXELFORMAT_P08A08R08G08B08I:
    {
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_X01R05G05B05I);
    }break;


  case PIXELFORMAT_X01R05G05B05I:
    {
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_R05G06B05I:
    {
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);
      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_A01R05G05B05I:
    {
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;

  case PIXELFORMAT_A01B05G05R05I:
    {
      cmp(PIXELFORMAT_A01R05G05B05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;

  case PIXELFORMAT_A04R04G04B04I:
    {
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_R08G08B08I:
    {
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_A08R08G08B08I:
    {
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_X08R08G08B08I:
    {
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_A02R10G10B10I:
    {
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_A02B10G10R10I:
    {
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_A16B16G16R16I:
    {
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);

      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_A16B16G16R16F:
    {
      cmp(PIXELFORMAT_A32B32G32R32F);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;

  case PIXELFORMAT_A32B32G32R32F:
    {
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;
  }
#undef cmp

  return PIXELFORMAT_NONE;
}


PIXELFORMAT GraphicsCore::FindFormatDepthStencil( PIXELFORMAT fmt )const
{
  const PIXELFORMATLIST& fmtlist = m_DepthStencilFormat;

  //  同じフォーマットがあるならそれ
  if( IsExist( fmtlist, fmt ) ) { return fmt; }

  //  ないなら似たようなフォーマットを返す
  //  アルファ無しフォーマットはアルファありフォーマットで代用できるけど
  //  アルファありは無しで代用できないので注意

#define cmp( fmt ) { if( IsExist( fmtlist, fmt ) ) { return fmt; } }
  switch( fmt )
  {
  case PIXELFORMAT_D15IS1:
    {
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24FS8);

      cmp(PIXELFORMAT_D16I);
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D24IX8);
    }break;

  case PIXELFORMAT_D16I:
    {
      cmp(PIXELFORMAT_D24IX8);
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24FS8);

      cmp(PIXELFORMAT_D15IS1);
    }break;

  case PIXELFORMAT_D32I:
    {
      cmp(PIXELFORMAT_D24IX8);
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24FS8);

      cmp(PIXELFORMAT_D16I);

      cmp(PIXELFORMAT_D15IS1);
    }break;

  case PIXELFORMAT_D24IX8:
    {
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24FS8);
      cmp(PIXELFORMAT_D16I);

      cmp(PIXELFORMAT_D15IS1);
    }break;

  case PIXELFORMAT_D24IS8:
    {
      cmp(PIXELFORMAT_D24FS8);
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D15IS1);
      cmp(PIXELFORMAT_D24IX8);
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D16I);

    }break;

  case PIXELFORMAT_D24IX4S4:
    {
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24FS8);
      cmp(PIXELFORMAT_D15IS1);
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D24IX8);
      cmp(PIXELFORMAT_D16I);

    }break;

  case PIXELFORMAT_D24FS8:
    {
      cmp(PIXELFORMAT_D24IS8);
      cmp(PIXELFORMAT_D24IX4S4);
      cmp(PIXELFORMAT_D15IS1);
      cmp(PIXELFORMAT_D24IX8);
      cmp(PIXELFORMAT_D32I);
      cmp(PIXELFORMAT_D16I);

    }break;
  }

  return PIXELFORMAT_NONE;
}



bool GraphicsCore::IsExist( const PIXELFORMATLIST& list, PIXELFORMAT fmt )const
{
  PIXELFORMATLIST::const_iterator ite = list.find(fmt);

  return ite!=list.end();
}


void GraphicsCore::UpdateMember()
{
  MAID_ASSERT( m_pDevice.get()==NULL, "デバイスが作成されていません"  );

  {
    const Graphics::SCREENMODE& mode = m_pDevice->GetScreenMode();
    m_ScreenMode.Format.Type = ConvertType(mode.DisplayMode.Format); 
    m_ScreenMode.Format.Size = mode.DisplayMode.Size;
    m_ScreenMode.Format.RefreshRate = mode.DisplayMode.RefreshRate;
    m_ScreenMode.IsWaitVSync = mode.IsWaitVSync;
    m_ScreenMode.IsFullScreen= mode.IsFullScreen;

  }

  m_TextureFormat.clear();

  m_pDevice->SerchEnableFormat( m_EnableFormat );
  const Graphics::ENABLEFORMAT& fmt = m_EnableFormat;

  for( size_t i=0; i<fmt.Texture.size();             ++i ) { m_TextureFormat.insert(fmt.Texture[i]); }
  for( size_t i=0; i<fmt.RenderTargetTexture.size(); ++i ) { m_RenderTargetTextureFormat.insert(fmt.RenderTargetTexture[i]); }
  for( size_t i=0; i<fmt.RenderTargetSurface.size(); ++i ) { m_RenderTargetFormat.insert(fmt.RenderTargetSurface[i]); }
  for( size_t i=0; i<fmt.DepthStencil.size();        ++i ) { m_DepthStencilFormat.insert(fmt.DepthStencil[i]); }
  for( size_t i=0; i<fmt.IndexBit.size();            ++i ) { m_IndexBitFormat.insert(fmt.IndexBit[i]); }
}

GraphicsCore::DISPLAYFORMAT::TYPE GraphicsCore::ConvertType( PIXELFORMAT fmt ) const
{
  DISPLAYFORMAT::TYPE ret = DISPLAYFORMAT::BPP16;
  switch( GetPixelBPP(fmt) )
  {
  case 16: { ret = DISPLAYFORMAT::BPP16; }break;
  case 32: 
    {
      if( fmt==PIXELFORMAT_A02R10G10B10I || fmt==PIXELFORMAT_A02B10G10R10I )
      {
        ret = DISPLAYFORMAT::BPP32EX; 
      }else
      {
        ret = DISPLAYFORMAT::BPP32; 
      }
    }break;
  default:
    {
      MAID_ASSERT( true, "ここにはこないはず" );
    }break;
  }

  return ret;
}


}

