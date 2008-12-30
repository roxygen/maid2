#ifndef maid2_graphics_depthstencil_h
#define maid2_graphics_depthstencil_h

/*!
    @file
    @brief  DepthStencilState
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"rendertargettexture.h"

namespace Maid
{
  class DepthStencil
      :public IDepthStencil
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    void CreateCompatible( const RenderTargetTexture& tex, PIXELFORMAT format );
    void Create( const SIZE2DI& size, PIXELFORMAT format );
    void Destroy();
  };	
}

#endif