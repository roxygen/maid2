/*!
 	@file
 	@brief	頂点定義クラス
 */

#ifndef graphic_core_driver_win32_direct3d09_vertexdeclarationbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_vertexdeclarationbufferd3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include"d3d09include.h"

#include"../../vertexdeclarationbuffer.h"


namespace Maid
{
	/*!
	 	@class	VertexDeclarationBufferD3D09 vertexdeclarationbufferd3d09.h
	 	@brief	頂点定義の基本クラス
	 */
	class VertexDeclarationBufferD3D09 : public VertexDeclarationBuffer
	{
	public:
		VertexDeclarationBufferD3D09( IDirect3DVertexDeclaration9* pBuf );
		SPD3D09VERTEXDECLARATION	pBuffer;
	};
}

#endif

