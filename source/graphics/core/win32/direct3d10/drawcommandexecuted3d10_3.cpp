#include"drawcommandexecuted3d10.h"
#include"deviced3d10_0.h"

//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D10::Draw( size_t UseVertexCount, size_t StartVertex )
{
/*
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseVertexCount);

  const HRESULT ret = m_Device.GetDevice()->DrawPrimitive( type, StartVertex, count );
  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawPrimitive()")); return; }
*/
}

void DrawCommandExecuteD3D10::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
/*
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseIndexCount);


  const HRESULT ret = m_Device.GetDevice()->DrawIndexedPrimitive(
                        type,
                        StartIndex,
                        0,
                        m_VertexMax-StartIndex,
                        0,
                        count
                        );


  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawIndexedPrimitive()")); return; }
*/
}

}}

