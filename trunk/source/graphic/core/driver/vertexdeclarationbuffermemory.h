/*!
 	@file
 	@brief	頂点定義ベースクラス
 */

#ifndef maid2_graphic_core_vertexdeclarationbuffermemory_h
#define maid2_graphic_core_vertexdeclarationbuffermemory_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	VertexDeclarationBufferMemory vertexdeclarationbuffermemory.h
      @brief	頂点定義の基本クラス
	 */
  class VertexDeclarationBufferMemory : public VertexDeclarationBuffer
	{
	public:
		virtual ~VertexDeclarationBufferMemory(){}	//!<	デストラクタ
	};
}

#endif

