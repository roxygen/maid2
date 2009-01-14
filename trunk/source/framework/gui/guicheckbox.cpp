#include"guicheckbox.h"

namespace Maid
{

GUICheckBox::GUICheckBox()
:m_IsCheck(false)
{

}

void GUICheckBox::SetCheck( bool IsCheck )
{
  if( m_IsCheck == IsCheck ) { return ; }

  {
    GUIMESSAGE_CHECKBOX_SETCHECK mess;
    mess.NewState = IsCheck;
    SendMessage( mess );
  }
}

bool GUICheckBox::IsCheck()const
{
  return m_IsCheck;
}

GUICheckBox::MESSAGERETURN GUICheckBox::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& m = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(*Param);
      {
        GUIMESSAGE_CHECKBOX_SETCHECK mess;
        mess.NewState = !m_IsCheck;
        PostMessage( mess );
      }

    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& m = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(*Param);
      m_IsCheck = m.NewState;
      OnStateChange(m.NewState);
    }break;
  }

  return GUIButton::MessageExecuting( Param );
}

}
