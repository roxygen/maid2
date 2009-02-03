#include"rendertargettexture.h"
#include"../auxiliary/debug/warning.h"
#include"graphicscore.h"

namespace Maid
{
  /*!
      @class  RenderTargetTexture rendertargettexture.h
      @brief  RenderTargetでもありTextureでもあるときに使うクラス
  */


void RenderTargetTexture::Destroy()
{
  Texture2DBase::Clear();
  RenderTargetBase::Clear();
}


void RenderTargetTexture::Create( const SIZE2DI& size, PIXELFORMAT format )
{
  Destroy();

  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const PIXELFORMAT DstFormat = pCore->FindFormatTexture2D( format, true );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << format );
    return ; 
  }

  const SIZE2DI VirtualSize = size;
  SIZE2DI RealSize;
  SIZE2DI TextureSize;

  pCore->CalcTextureSize( VirtualSize, RealSize, TextureSize );

  if( VirtualSize.w <= RealSize.w && VirtualSize.h <= RealSize.h )
  {
    RealSize = VirtualSize;
  }


  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = TextureSize;
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

  Texture2DBase::Setup( pTexture, pMaterial, VirtualSize, RealSize, format );
  RenderTargetBase::Setup( pTarget, VirtualSize, RealSize, DRAWAREA_LEFTTOP );
}


}

