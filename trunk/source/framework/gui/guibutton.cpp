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

          boost::shared_ptr<GUIMESSAGE_BUTTON_CLICK> p( new GUIMESSAGE_BUTTON_CLICK );
          p->Position = m.Position;
          p->Button   = m.Button;

          PostMessage( p );
        }
      }
    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
