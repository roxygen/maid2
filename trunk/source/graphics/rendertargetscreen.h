#ifndef maid2_graphics_rendertargetscreen_h
#define maid2_graphics_rendertargetscreen_h

/*!
    @file
    @brief  スクリーンが持っている RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"rendertargetbase.h"

namespace Maid
{
  class GraphicsCore;
  class RenderTargetScreen
    : public RenderTargetBase
    , public GlobalPointer<GraphicsCore>
  {
  public:
    RenderTargetScreen();

    void Initialize();
    void Finalize();
    void Present();
    void SetAspectLock( bool IsLock );

  private:
    void Update();


  private:
    SIZE2DI  m_AspectSize;
  };	
	
}

#endif