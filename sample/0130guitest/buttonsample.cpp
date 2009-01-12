#include"buttonsample.h"

using namespace Maid;



ButtonSample::ButtonSample( GraphicsRender& r )
    :m_Render( r )
{

}


void ButtonSample::OnInitialize()
{
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = SIZE2DI(100,100);
}

void ButtonSample::OnFinalize()
{

}

bool ButtonSample::LocalIsMouseIn( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void ButtonSample::OnUpdateFrame()
{
}

void ButtonSample::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = MAIDTEXT("ボタン:");
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();

  if( in )
  {
    if( down ) { str += MAIDTEXT("押し中"); }
    else { str += MAIDTEXT("入った"); }
  }else
  {
    if( down ) { str += MAIDTEXT("押し中外"); }
    else { str += MAIDTEXT("通常"); }
  }

  m_Render.Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), m_Size, POINT2DI(0,0) );
  m_Render.BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}

void ButtonSample::OnMouseIn( const POINT2DI& pos )
{
}

void ButtonSample::OnMouseOut( const POINT2DI& pos )
{
}

void ButtonSample::OnMouseDown( const POINT2DI& pos, int Button )
{
}

void ButtonSample::OnMouseUp( const POINT2DI& pos, int Button )
{
}
