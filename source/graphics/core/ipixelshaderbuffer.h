/*!
    @file
    @brief	ピクセルシェーダーバッファ
 */

#ifndef maid2_graphic_core_pixelshaderbuffer_h
#define maid2_graphic_core_pixelshaderbuffer_h


#include"../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IPixelShaderBuffer
  {
  public:
    virtual ~IPixelShaderBuffer(){}

  };

  /*!
      @brief		PixelShaderBuffer クラスの共有ポインタ
   */
  typedef boost::shared_ptr<IPixelShaderBuffer> SPPIXELSHADERBUFFER;	

}

#endif