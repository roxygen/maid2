/*!
    @file
    @brief	ピクセルシェーダーバッファ
 */

#ifndef maid2_graphic_core_vertexshaderbuffer_h
#define maid2_graphic_core_vertexshaderbuffer_h


#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class PixelShaderBuffer
  {
  public:
    virtual ~PixelShaderBuffer(){}

  };

  /*!
      @brief		PixelShaderBuffer クラスの共有ポインタ
   */
  typedef boost::shared_ptr<PixelShaderBuffer> SPPIXELSHADERBUFFER;	

}

#endif