#ifndef maid2_graphics_rasterizerstate_h
#define maid2_graphics_rasterizerstate_h

/*!
    @file
    @brief  RasterizerState
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class RasterizerState
      :public IRasterizerState
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    RasterizerState();
    virtual ~RasterizerState();

    void Create( const Graphics::RASTERIZERSTATEPARAM& state );
    void Delete();
  };	
}

#endif