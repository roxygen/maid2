#include"rendertargettexture.h"
#include"../auxiliary/debug/warning.h"

namespace Maid
{

void RenderTargetTexture::Destroy()
{
  Texture2DBase::Clear();
}


void RenderTargetTexture::Create( const SIZE2DI& size, PIXELFORMAT format )
{
  Create( size, size, format );
}

void RenderTargetTexture::Create( const SIZE2DI& create, const SIZE2DI& size, PIXELFORMAT format )
{
  Destroy();

  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const PIXELFORMAT DstFormat = pCore->FindFormatTexture2D( format, true );
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
    param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL | Graphics::RESOURCEBINDFLAG_RENDERTARGET;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
        return ; 
    }
  }


  Graphics::SPMATERIAL pMaterial;
  {
    Graphics::CREATEMATERIALPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;

    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, param );
    if( pMaterial.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
        return ; 
    }
  }

  Graphics::SPRENDERTARGET pTarget;
  {
    Graphics::CREATERENDERTARGETPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;

    pTarget = pCore->GetDevice()->CreateRenderTarget( pTexture, param );
    if( pTarget.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
        return ; 
    }
  }
  
  IRenderTarget::Set( pTarget );

  Setup( pTexture, pMaterial, size, create );
}


}

