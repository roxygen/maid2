#include"graphicsrender.h"




namespace Maid
{
void GraphicsRender::Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand )
{
  {
    m_FillVertex.Create( sizeof(CUSTOMVERTEX_COLOR)*4 );

    const char* vs =
      "vs.1.1\n"
      "dcl_position v0\n"
      "dcl_color v1\n"
      "mov oPos, v0\n"
      "mov oD0, v1\n"
      ;

    m_FillVertexShader.Create( String::ConvertSJIStoMAID(vs) );

    const char* ps =
      "1"
      ;

    m_FillPixelShader.Create( String::ConvertSJIStoMAID(ps) );
  }

  m_pDrawCommand = pDrawCommand;
}

void GraphicsRender::ClearRenderTarget( const COLOR_A32B32G32R32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}


bool GraphicsRender::IsLoading() const
{
  return m_FillVertexShader.IsCompiling()
    || m_FillPixelShader.IsCompiling()
}

}