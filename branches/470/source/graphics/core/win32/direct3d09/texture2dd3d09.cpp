﻿#include"texture2dd3d09.h"
#include"deviced3d09.h"

namespace Maid { namespace Graphics {

Texture2DD3D09::Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09TEXTURE& pTex )
  :ITexture2D(param), pTexture(pTex),IsSwapChainSurface(false)
{
}


Texture2DD3D09::Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09TEXTURE& pTex, const SPD3D09SURFACE& pSurf )
  :ITexture2D(param), pTexture(pTex), pSurface(pSurf),IsSwapChainSurface(false)
{
}

Texture2DD3D09::Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09SURFACE& pSurf, bool sw )
  :ITexture2D(param), pSurface(pSurf),IsSwapChainSurface(sw)
{
}


void  Texture2DD3D09::Escape()
{
  if( pSurface.get()!=NULL )
  {
    D3DSURFACE_DESC desc;
    pSurface->GetDesc( &desc );

    if( desc.Pool==D3DPOOL_DEFAULT )
    {
      pSurface.reset();
    }
  }

  if( pTexture.get()!=NULL )
  {
    D3DSURFACE_DESC desc;
    pTexture->GetLevelDesc( 0, &desc );

    if( desc.Pool==D3DPOOL_DEFAULT )
    {
      pTexture.reset();
    }
  }
}

void  Texture2DD3D09::Restore( DeviceD3D09& Device )
{
  //  どちらかがある＝＝作り直す必要がない
  if( pTexture.get()!=NULL || pSurface.get()!=NULL ) { return ; }

  if( IsSwapChainSurface )
  {
    SPRENDERTARGET pTarget = Device.GetCurrentRenderTarget();
    Texture2DD3D09* pTex = static_cast<Texture2DD3D09*>(pTarget->GetResource().get());
    pSurface = pTex->pSurface;
    pTexture = pTex->pTexture;
  }else
  {
    SPTEXTURE2D p = Device.CreateTexture2D( GetParam(), NULL );
    Texture2DD3D09* pTex = static_cast<Texture2DD3D09*>(p.get());
    pSurface = pTex->pSurface;
    pTexture = pTex->pTexture;
  }
}


}}


