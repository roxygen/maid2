#include"graphicscore.h"



namespace Maid {

//! 指定したフォーマットに一番近い作成できるテクスチャフォーマットを探す
/*!
    @param  fmt             [i ]  調べたいフォーマット
    @param  IsRenderTarget  [i ]  RenderTargetとしても使うか？
 */
PIXELFORMAT GraphicsCore::FindFormatTexture2D( PIXELFORMAT fmt, bool IsRenderTarget )const
{
//  const PIXELFORMATLIST& fmtlist = IsRenderTarget? m_TextureFormat : m_RenderTargetTextureFormat;
  const PIXELFORMATLIST& fmtlist = IsRenderTarget? m_RenderTargetTextureFormat : m_TextureFormat;

  return FindFormat( fmt, fmtlist );
}


//! 指定したフォーマットに一番近い作成できるRenderTargetフォーマットを探す
/*!
    @param  fmt  [i ]  調べたいフォーマット
 */
PIXELFORMAT GraphicsCore::FindFormatRenderTarget( PIXELFORMAT fmt )const
{
  return FindFormat( fmt, m_RenderTargetFormat );
}

//! 指定したフォーマットに一番近い作成できるDepthStencilフォーマットを探す
/*!
    @param  fmt  [i ]  調べたいフォーマット
 */
PIXELFORMAT GraphicsCore::FindFormatDepthStencil( PIXELFORMAT fmt )const
{
  const PIXELFORMATLIST& fmtlist = m_DepthStencilFormat;

  return FindFormat( fmt, m_DepthStencilFormat );
}


PIXELFORMAT GraphicsCore::FindFormat( PIXELFORMAT fmt, const PIXELFORMATLIST& fmtlist )const
{
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);

      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_X01R05G05B05I);
    }break;


  case PIXELFORMAT_X01R05G05B05I:
    {
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_R08G08B08I:
    {
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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

  case PIXELFORMAT_B08G08R08I:
    {
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_A08B08G08R08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_A08B08G08R08I:
    {
      cmp(PIXELFORMAT_A08R08G08B08I);
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
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
    }break;

  case PIXELFORMAT_X08R08G08B08I:
    {
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
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

      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);

      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);

    }break;

  case PIXELFORMAT_A02B10G10R10I:
    {
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);

      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);

      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X08R08G08B08I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;

  case PIXELFORMAT_A16B16G16R16I:
    {
      cmp(PIXELFORMAT_A16B16G16R16F);
      cmp(PIXELFORMAT_A32B32G32R32F);

      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A02R10G10B10I);

      cmp(PIXELFORMAT_A08B08G08R08I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);

      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_A04R04G04B04I);
    }break;

  case PIXELFORMAT_A16B16G16R16F:
    {
      cmp(PIXELFORMAT_A32B32G32R32F);
      cmp(PIXELFORMAT_A16B16G16R16I);
      cmp(PIXELFORMAT_A02R10G10B10I);
      cmp(PIXELFORMAT_A02B10G10R10I);
      cmp(PIXELFORMAT_A08R08G08B08I);
      cmp(PIXELFORMAT_A08B08G08R08I);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
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
      cmp(PIXELFORMAT_A08B08G08R08I);
      cmp(PIXELFORMAT_A04R04G04B04I);
      cmp(PIXELFORMAT_A01R05G05B05I);
      cmp(PIXELFORMAT_A01B05G05R05I);

      cmp(PIXELFORMAT_X08R08G08B08I);
      cmp(PIXELFORMAT_R08G08B08I);
      cmp(PIXELFORMAT_B08G08R08I);
      cmp(PIXELFORMAT_X01R05G05B05I);
      cmp(PIXELFORMAT_R05G06B05I);
    }break;

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
#undef cmp

  return PIXELFORMAT_NONE;
}




bool GraphicsCore::IsExist( const PIXELFORMATLIST& list, PIXELFORMAT fmt )const
{
  PIXELFORMATLIST::const_iterator ite = list.find(fmt);

  return ite!=list.end();
}



}

