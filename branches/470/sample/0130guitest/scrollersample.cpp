﻿#include"scrollersample.h"

using namespace Maid;



void ScrollerSample::Initialize( Maid::GraphicsRender* r )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
}

void ScrollerSample::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void ScrollerSample::OnFinalize()
{
}


const int BARSIZE_W = 200;
const int BARSIZE_H = 10;

bool ScrollerSample::IsBarCollision( const POINT2DI& pos, int len, float rad ) const
{
  const RECT2DI rc( POINT2DI(-BARSIZE_W/2,-BARSIZE_H/2), SIZE2DI(BARSIZE_W,BARSIZE_H) );

  return Collision<float>::PointRect( pos, rc, rad );
}

const int BUTTONSIZE_W = 20;
const int BUTTONSIZE_H = 20;


bool ScrollerSample::IsButtonCollision( const POINT2DI& pos, int len, float rad ) const
{
  const RECT2DI rc( POINT2DI(-BUTTONSIZE_W/2,-BUTTONSIZE_H/2), SIZE2DI(BUTTONSIZE_W,BUTTONSIZE_H) );
  return Collision<float>::PointRect( pos, rc, rad );
}

int ScrollerSample::GetBarLength() const
{
  return BARSIZE_W;
}


void ScrollerSample::OnUpdateFrame()
{
}

void ScrollerSample::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = MAIDTEXT("slider:");
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();
  const int vale = GetValue();

  if( in )
  {
    if( down ) { str += MAIDTEXT("押し中"); }
    else { str += MAIDTEXT("入った"); }
  }else
  {
    if( down ) { str += MAIDTEXT("押し中外"); }
    else { str += MAIDTEXT("通常"); }
  }

  {
    char buf[256];
    sprintf( buf, "value:%03d", vale );
    str += String::ConvertSJIStoMAID( buf );
  }

  const float rot = GetRotate();
  {
    const SIZE2DI barsize(BARSIZE_W,BARSIZE_H);
    const POINT2DI baroff(BARSIZE_W/2,BARSIZE_H/2);

    m_pRender->FillR( pos, COLOR_R32G32B32A32F(1,0,0,1), barsize, baroff, rot, VECTOR3DF(0,0,1) );
  }
  {
    const SIZE2DI barsize(BUTTONSIZE_W,BUTTONSIZE_H);
    const POINT2DI baroff(BUTTONSIZE_H/2,BUTTONSIZE_H/2);

    const VECTOR2DI vec = CalcButtonOffset();

    m_pRender->FillR( pos+vec, COLOR_R32G32B32A32F(0,1,0,1), barsize, baroff, rot, VECTOR3DF(0,0,1) );
  }

  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}



void ScrollerSample::OnMouseMove( const POINT2DI& pos )
{
}

void ScrollerSample::OnMouseIn( const POINT2DI& pos )
{
}
void ScrollerSample::OnMouseOut( const POINT2DI& pos )
{
}
void ScrollerSample::OnButtonDown( const POINT2DI& pos )
{

}

void ScrollerSample::OnButtonUp( const POINT2DI& pos )
{

}

