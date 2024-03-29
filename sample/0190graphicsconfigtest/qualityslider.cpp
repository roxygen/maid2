﻿#include"qualityslider.h"

using namespace Maid;



void QualitySlider::Initialize( Maid::Graphics2DRender* r, const Maid::String& Text, const Maid::SIZE2DI& bar, const Maid::SIZE2DI& button )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Text = Text;
  m_BarSize = bar;
  m_ButtonSize = button;
}

void QualitySlider::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void QualitySlider::OnFinalize()
{
}


//const int BARSIZE_W = 200;
//const int BARSIZE_H = 10;

bool QualitySlider::IsBarCollision( const POINT2DI& pos ) const
{
  const int barlen = GetBarLength();
  const float rad = GetRotate();

  const RECT2DI rc( POINT2DI(0,-m_BarSize.h/2), m_BarSize );

  return Collision<float>::PointRect( pos, rc, rad );
}

//const int BUTTONSIZE_W = 20;
//const int BUTTONSIZE_H = 20;


bool QualitySlider::IsButtonCollision( const POINT2DI& pos ) const
{
  const int butlen = GetButtonLength();
  const float rad = GetRotate();

  const RECT2DI rc( POINT2DI(-m_ButtonSize.w/2,-m_ButtonSize.h/2), m_ButtonSize );
  return Collision<float>::PointRect( pos, rc, rad );
}

void QualitySlider::OnUpdateFrame()
{
}

void QualitySlider::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = m_Text;
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();
  const int vale = GetValue();
  const int barlen = GetBarLength();
  const int butlen = GetButtonLength();

  {
    char buf[256];
    sprintf( buf, "%03d", vale );
    str += String::ConvertSJIStoMAID( buf );
  }

  const float rot = GetRotate();
  {
    const SIZE2DI barsize = m_BarSize;
    const POINT2DI baroff(0,m_BarSize.h/2);

    m_pRender->FillR( pos, COLOR_R32G32B32A32F(1,0,0,1), barsize, baroff, rot, VECTOR3DF(0,0,1) );
  }
  {
    const SIZE2DI barsize = m_ButtonSize;
    const POINT2DI baroff(m_ButtonSize.w/2,m_ButtonSize.h/2);

    const VECTOR2DI vec = CalcButtonOffset();

    m_pRender->FillR( pos+vec, COLOR_R32G32B32A32F(0.5f,0.5f,0.5f,1), barsize, baroff, rot, VECTOR3DF(0,0,1) );
  }

  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}



void QualitySlider::OnMouseMove( const POINT2DI& pos )
{
}

void QualitySlider::OnMouseIn( const POINT2DI& pos )
{
}
void QualitySlider::OnMouseOut( const POINT2DI& pos )
{
}
void QualitySlider::OnButtonDown( const POINT2DI& pos )
{

}

void QualitySlider::OnButtonUp( const POINT2DI& pos )
{

}

