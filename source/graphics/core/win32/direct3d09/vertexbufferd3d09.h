/*!
 	@file
 	@brief	頂点バッファクラス D3D9 Ver.
 */

#ifndef graphic_core_driver_win32_direct3d09_vertexbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_vertexbufferd3d09_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"d3d09include.h"

#include"../../ivertexbuffer.h"

namespace Maid
{
  /*!
    @class	VertexBufferD3D09 vertexbufferd3d09.h
    @brief	頂点バッファクラス
   */
  class VertexBufferD3D09 : public IVertexBuffer
  {
  public:
    VertexBufferD3D09( IDirect3DVertexBuffer9* p, int Stride );

    SPD3D09VERTEXBUFFER	pBuffer;
    D3DVERTEXBUFFER_DESC Desc;
    int	Stride;
  };
}

#endif

