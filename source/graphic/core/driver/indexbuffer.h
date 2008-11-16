/*!
    @file
    @brief	インデックスバッファベースクラス
 */

#ifndef maid2_graphic_core_indexbuffer_h
#define maid2_graphic_core_indexbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	IndexBuffer indexbuffer.h
      @brief	頂点バッファの基本クラス
	 */
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer(){}	//!<	デストラクタ
	};

	typedef boost::shared_ptr<IndexBuffer> SPINDEXBUFFER;
}

#endif

