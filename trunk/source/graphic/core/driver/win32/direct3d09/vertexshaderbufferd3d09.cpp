#include"vertexshaderbufferd3d09.h"


namespace Maid
{

VertexShaderBufferD3D09HARD::VertexShaderBufferD3D09HARD( IDirect3DVertexShader9* p )
{
  pBuffer.reset( p );
}

void VertexShaderBufferD3D09HARD::Setup( const SPD3D09DEVICE& pDevice )
{
  HRESULT ret = pDevice->SetVertexShader( pBuffer.get() );
 	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetVertexShader()")); }
}
	





//  以下固定パイプライン向けシェーダー
void VertexShaderBufferD3D09_NORMAL::Setup( const SPD3D09DEVICE& pDevice )
{

}


}

