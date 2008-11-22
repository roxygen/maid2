/*!
 	@file
 	@brief	インデックスバッファクラス D3D9 Ver.
 */

#ifndef graphic_core_driver_win32_direct3d09_indexbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_indexbufferd3d09_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../iindexbuffer.h"

#include"d3d09include.h"

namespace Maid
{
	/*!
	 	@class	IndexBufferD3D09 iindexbufferd3d09.h
	 	@brief	インデックスバッファクラス
	 */
	class IndexBufferD3D09 : public IIndexBuffer
	{
	public:
		IndexBufferD3D09( IDirect3DIndexBuffer9* p );

		SPD3D09INDEXBUFFER	pBuffer;
		D3DINDEXBUFFER_DESC Desc;
	};
}

#endif

