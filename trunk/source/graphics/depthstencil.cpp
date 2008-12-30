#include"depthstencil.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

void DepthStencil::CreateCompatible( const RenderTargetTexture& tex, PIXELFORMAT format )
{
  const SIZE2DI size = tex.GetTextureSize();
  Create( size, format );

}

void DepthStencil::Create( const SIZE2DI& size, PIXELFORMAT format )
{
  Destroy();

  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const PIXELFORMAT DstFormat = pCore->FindFormatDepthStencil( format );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << format );
    return ; 
  }

  const SIZE2DI TexSize = pCore->CalcTextureSize( size );

  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = TexSize;
    param.MipLevels = 0;
    param.ArraySize = 0;
    param.Format = DstFormat;
    param.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
    param.BindFlags = Graphics::RESOURCEBINDFLAG_DEPTHSTENCIL;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("DepthStencilの作成に失敗 ") );
        return ; 
    }
  }

  Graphics::SPDEPTHSTENCIL pTarget;
  {
    Graphics::CREATEDEPTHSTENCILPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D;

    pTarget = pCore->GetDevice()->CreateDepthStencil( pTexture, param );
    if( pTarget.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("DepthStencilの作成に失敗 ") );
        return ; 
    }
  }
  
  IDepthStencil::Set( pTarget );
}


void DepthStencil::Destroy()
{
  IDepthStencil::Clear();
}

}