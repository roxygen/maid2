#include"guiradiogroup.h"


namespace Maid
{

void GUIRadioGroup::SetRadioButton( const std::vector<SPGUIRADIOBUTTON>& RadioButtonList )
{
  m_Button = RadioButtonList;

  //  私が親ですよーとメッセージを送る
  for( int i=0; i<(int)m_Button.size(); ++i )
  {
    GUIMESSAGE_INITIALIZE m;
    m.pParent = this;
    m.ID = (IGUIParts::ID)i;
    m_Button[i]->SendMessage( m );
  }

  m_SelectNo = SELECT_NONE;
}

SPGUIPARTS GUIRadioGroup::LocalFindChild( const POINT2DI& pos ) const
{
  for( int i=0; i<(int)m_Button.size(); ++i )
  {
    const SPGUIPARTS& pParts = m_Button[i];

    if( pParts->IsMouseIn(pos) ) 
    {
      return pParts;
    }
  }

  return SPGUIPARTS();
}

bool GUIRadioGroup::LocalIsMouseIn( const POINT2DI& pos ) const
{
  for( int i=0; i<(int)m_Button.size(); ++i )
  {
    const SPGUIPARTS& pParts = m_Button[i];

    if( pParts->IsMouseIn(pos) ) { return true; }
  }

  return false;
}


int GUIRadioGroup::GetCheckNo() const
{
  return m_SelectNo;
}

void GUIRadioGroup::SetCheck( int pos )
{
  MAID_ASSERT( pos<=SELECT_NONE, "値が小ささすぎます " << pos );
  MAID_ASSERT( !(pos<(int)m_Button.size()), "範囲が大きすぎます " << pos );

  if( m_SelectNo==pos ) { return ; }

  GUIMESSAGE_RADIDGROUP_CHANGESELECT m;
  m.OldNo = m_SelectNo;
  m.NewNo = pos;

  SendMessage( m );
}

void GUIRadioGroup::OnUpdateFrame()
{
  for( int i=0; i<(int)m_Button.size(); ++i )
  {
    m_Button[i]->UpdateFrame();
  }
}

void GUIRadioGroup::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  for( int i=0; i<(int)m_Button.size(); ++i )
  {
    m_Button[i]->UpdateDraw(Target,Depth,pos);
  }
}


void GUIRadioGroup::OnSendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_RADIDGROUP_CHANGESELECT:
    {
      GUIMESSAGE_RADIDGROUP_CHANGESELECT p = static_cast<const GUIMESSAGE_RADIDGROUP_CHANGESELECT&>(Param);

      m_SelectNo = p.NewNo;

      if( p.OldNo!=SELECT_NONE )
      {
        m_Button[p.OldNo]->SetCheck(false);
      }

      if( p.NewNo!=SELECT_NONE )
      {
        m_Button[p.NewNo]->SetCheck(true);
      }

    //  SendMessageParent( p );
    }break;
  }
}

void GUIRadioGroup::ReciveMessage( ID src, const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      {
        GUIMESSAGE_RADIDGROUP_MOUSEMOVE m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
      OnGroupMouseMove( src, pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      {
        GUIMESSAGE_RADIDGROUP_MOUSEDOWN m;
        m.ButtonNo = src;
        m.Position = p.Position;
        m.ButtonNo = p.Button;
        SendMessageParent( m );
      }
      OnGroupMouseDown( src, pos, p.Button );
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      {
        GUIMESSAGE_RADIDGROUP_MOUSEUP m;
        m.ButtonNo = src;
        m.Position = p.Position;
        m.ButtonNo = p.Button;
        SendMessageParent( m );
      }
      OnGroupMouseUp( src, pos, p.Button );
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      {
        GUIMESSAGE_RADIDGROUP_MOUSEIN m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
      OnGroupMouseIn( src, pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      {
        GUIMESSAGE_RADIDGROUP_MOUSEOUT m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
      OnGroupMouseOut( src, pos );
    }break; 

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& p = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(Param);
      if( p.IsCheck )
      {
        SetCheck(src);
      }
    }break;
  }
}

}
