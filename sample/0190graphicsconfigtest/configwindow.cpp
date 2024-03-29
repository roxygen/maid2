﻿#include"configwindow.h"

using namespace Maid;



void ConfigWindow::Initialize( Maid::Graphics2DRender* r )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
}

void ConfigWindow::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void ConfigWindow::OnFinalize()
{
}


const RECT2DI BARRECT(0,0,200,30);
const RECT2DI CLIENTRECT(0,BARRECT.GetBottom(),200,200);

const RECT2DI MINIRECT( 0,0, 30, 30);
const RECT2DI HIDERECT(30,0, 30, 30);

bool ConfigWindow::IsBarCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, BARRECT );
}

bool ConfigWindow::IsClientCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, CLIENTRECT );
}


bool ConfigWindow::IsMinimumButtonCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, MINIRECT );
}

bool ConfigWindow::IsHideButtonCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, HIDERECT );
}


void ConfigWindow::OnUpdateFrame()
{
}

void ConfigWindow::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = MAIDTEXT("window:");
  const bool in = IsMouseIn();
  const ConfigWindow::STATE state = GetState();
  const bool mini = IsMinimum();

  switch( state )
  {
  case ConfigWindow::STATE_NORMAL:
    {
      if( in ) { str += MAIDTEXT("入った"); }
      else { str += MAIDTEXT("通常"); }
    }break;

  case ConfigWindow::STATE_MOVING:
    {
      str += MAIDTEXT("移動中");
    }break;

  case ConfigWindow::STATE_MINIMUMBUTTONDOWN:
    {
      str += MAIDTEXT("最小化ボタン");
    }break;

  case ConfigWindow::STATE_HIDEBUTTONDOWN:
    {
      str += MAIDTEXT("消えるボタン");
    }break;
  }

  {
    POINT2DI p = pos;
    p.x += BARRECT.x;
    p.y += BARRECT.y;

    m_pRender->Fill( p, COLOR_R32G32B32A32F(1,0,0,1), BARRECT.GetSize(), POINT2DI(0,0) );
  }
  {
    POINT2DI p = pos;
    p.x += MINIRECT.x;
    p.y += MINIRECT.y;

    m_pRender->Fill( p, COLOR_R32G32B32A32F(0,1,1,1), MINIRECT.GetSize(), POINT2DI(0,0) );
  }
  {
    POINT2DI p = pos;
    p.x += HIDERECT.x;
    p.y += HIDERECT.y;

    m_pRender->Fill( p, COLOR_R32G32B32A32F(0,0,1,1), HIDERECT.GetSize(), POINT2DI(0,0) );
  }

  if( !mini )
  {
    POINT2DI p = pos;
    p.x += CLIENTRECT.x;
    p.y += CLIENTRECT.y;

    m_pRender->Fill( p, COLOR_R32G32B32A32F(0,1,0,1), CLIENTRECT.GetSize(), POINT2DI(0,0) );
  }


  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}



void ConfigWindow::OnMouseIn( const POINT2DI& pos )
{
}

void ConfigWindow::OnMouseOut( const POINT2DI& pos )
{
}

void ConfigWindow::OnMoveBegin( const POINT2DI& pos )
{
}
void ConfigWindow::OnMoveEnd( const POINT2DI& pos )
{
}
void ConfigWindow::OnMinimum( bool IsMin )
{

}

void ConfigWindow::OnChangeState( const POINT2DI& pos, STATE state )
{

}

