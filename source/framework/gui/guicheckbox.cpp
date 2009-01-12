#include"guicheckbox.h"

/*
  GUIButtonクラスを継承してチェックボタンを作ると
  MESSAGE_BUTTON_CLICK メッセージが親に飛んでしまう。

  別に飛んでもいい気がするんだけど、なんとなくいやなので
  飛ばないようにラップする
*/


namespace Maid
{

bool GUICheckBox::GUIButtonEx::LocalIsMouseIn( const POINT2DI& pos ) const
{
  return GetParent()->IsMouseIn(pos);
}

GUICheckBox::GUICheckBox()
:m_IsCheck(false)
{

}

void GUICheckBox::SetCheck( bool IsCheck )
{
  GUIMESSAGE_CHECKBOX_SETCHECK m;
  m.IsCheck = IsCheck;
  SendMessage( m );
}

bool GUICheckBox::IsCheck()const
{
  return m_IsCheck;
}

void GUICheckBox::OnSendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      GUIMESSAGE_INITIALIZE p = static_cast<const GUIMESSAGE_INITIALIZE&>(Param);
      p.pParent = this;
      p.ID = 0;

      m_Button.SendMessage( p );
    }break;

  case IGUIParam::MESSAGE_SETOFFSET:
    {
      //  このメッセージは送らない
    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& p = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(Param);

      if( m_IsCheck != p.IsCheck )
      {
        m_IsCheck = p.IsCheck;
        OnStateChange( m_IsCheck );
      }
    }break;

  default:
    {
      m_Button.SendMessage( Param );
    }break;
  }
}

void GUICheckBox::ReciveMessage( ID src, const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      OnMouseMove( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      OnMouseDown(pos, p.Button );
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      OnMouseUp(pos, p.Button);
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      OnMouseIn(pos);
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      OnMouseOut(pos);
    }break; 

  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& p = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(Param);
      m_IsCheck = !m_IsCheck;

      {
        GUIMESSAGE_CHECKBOX_SETCHECK m;
        m.IsCheck = m_IsCheck;
        SendMessageParent( m );
      }

      OnStateChange( m_IsCheck );
    }break;
  }
}

}
