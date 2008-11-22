/*!
	@file
	@brief	頂点バッファベースクラス
 */

#ifndef maid2_graphic_core_ivertexbuffer_h
#define maid2_graphic_core_ivertexbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
      @class	IVertexBuffer ivertexbuffer.h
      @brief	頂点バッファの基本クラス
   */
  class IVertexBuffer
  {
  public:
    virtual ~IVertexBuffer(){}	//!<	デストラクタ
  };

  typedef boost::shared_ptr<IVertexBuffer> SPVERTEXBUFFER;
}

#endif

