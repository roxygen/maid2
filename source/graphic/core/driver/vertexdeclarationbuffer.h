/*!
 	@file
 	@brief	頂点定義ベースクラス
 */

#ifndef maid2_graphic_core_vertexdeclarationbuffer_h
#define maid2_graphic_core_vertexdeclarationbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	VertexDeclarationBuffer vertexdeclarationbuffer.h
      @brief	頂点定義の基本クラス
	 */
	class VertexDeclarationBuffer
	{
	public:
		virtual ~VertexDeclarationBuffer(){}	//!<	デストラクタ
	};

	typedef boost::shared_ptr<VertexDeclarationBuffer> SPVERTEXDECLARATIONBUFFER;
}

#endif

