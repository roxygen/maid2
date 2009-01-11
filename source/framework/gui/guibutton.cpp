#include"guibutton.h"



namespace Maid
{

GUIButton::GUIButton()
  :m_IsMouseIn(false)
  ,m_IsButtonDown(false)
{

}


void GUIButton::OnSendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      const bool in = IGUIParts::IsMouseIn( p.Position );

      if( !m_IsMouseIn && in ) 
      {
        OnMouseIn( pos );
        GUIMESSAGE_MOUSEIN m;
        m.Position = p.Position;
        SendMessageParent( m );
      }
      ef( m_IsMouseIn && !in )
      {
        OnMouseOut( pos );
        GUIMESSAGE_MOUSEOUT m;
        m.Position = p.Position;
        SendMessageParent( m );
      }

      m_IsMouseIn = in;
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      {
        m_IsButtonDown = true;
        const POINT2DI pos = p.Position-GetOffset();
        OnMouseDown(pos, p.Button );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      {
        const POINT2DI pos = p.Position-GetOffset();
        OnMouseUp(pos, p.Button);

        if( m_IsButtonDown && IGUIParts::IsMouseIn(p.Position) )
        {
          const POINT2DI pos = p.Position - GetOffset();
          OnMouseClick( pos );

          GUIMESSAGE_BUTTON_CLICK m;
          m.Position = p.Position;
          SendMessageParent( m );
        }
      }
      m_IsButtonDown = false;
    }break;
  }
}

}
