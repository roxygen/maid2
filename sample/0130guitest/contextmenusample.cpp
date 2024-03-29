﻿#include"contextmenusample.h"

using namespace Maid;


void ContextMenuChildSample::Initialize( Maid::Graphics2DRender* r, const Maid::String& text, const SIZE2DI& size )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = size;
  m_Text = text;
}

SIZE2DI ContextMenuChildSample::GetSize() const
{
  return m_Size;
}


void ContextMenuChildSample::LocalUpdateFrame()
{
}

void ContextMenuChildSample::LocalUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = m_Text;

  if( IsSelect() )
  {
    str += MAIDTEXT("on");
  }

  m_pRender->Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), m_Size, POINT2DI(0,0) );
  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}

