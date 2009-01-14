#include"guibutton.h"



namespace Maid
{

GUIButton::GUIButton()
  :m_IsButtonDown(false)
{
}


GUIButton::MESSAGERETURN GUIButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*Param);
      m_IsButtonDown = true;
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*Param);

      if( m_IsButtonDown )
      {
        m_IsButtonDown = false;

        const POINT2DI pos = m.Position-CalcTotalOffset();
        if( LocalIsCollision(pos) )
        {
          OnMouseClick( pos );

          GUIMESSAGE_BUTTON_CLICK mess;
          mess.Position = m.Position;
          mess.Button   = m.Button;

          PostMessage( mess );
        }
      }
    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
