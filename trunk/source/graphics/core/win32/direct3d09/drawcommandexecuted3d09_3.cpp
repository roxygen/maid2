#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::Draw( int UseVertexCount, int StartVertex )
{
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseVertexCount);

  const HRESULT ret = m_Device.GetDevice()->DrawPrimitive( type, StartVertex, count );
  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawPrimitive()")); return; }
}


}}

