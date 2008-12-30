#ifndef maid2_graphics_rendertargettexture_h
#define maid2_graphics_rendertargettexture_h

/*!
    @file
    @brief  テクスチャに使える RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"texture2dbase.h"

namespace Maid
{
  class RenderTargetTexture
    :public Texture2DBase
    ,public IRenderTarget
    ,public GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const SIZE2DI& size, PIXELFORMAT format );
    void Create( const SIZE2DI& create, const SIZE2DI& size, PIXELFORMAT format );
    void Destroy();

  };
	
}

#endif