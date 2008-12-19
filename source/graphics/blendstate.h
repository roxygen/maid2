#ifndef maid2_graphics_blendstate_h
#define maid2_graphics_blendstate_h

/*!
    @file
    @brief  BlendState
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class BlendState
      :public IBlendState
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    BlendState();
    virtual ~BlendState();

    void Create( const Graphics::BLENDSTATEPARAM& state );
    void Delete();

  private:
    BlendState( const BlendState& rha ){} //  そのうち書く。
    BlendState& operator = ( const BlendState& rha ){} //  そのうち書く。

  };	
}

#endif