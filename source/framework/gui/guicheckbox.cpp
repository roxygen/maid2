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
    boost::shared_ptr<GUIMESSAGE_CHECKBOX_SETCHECK> m( new GUIMESSAGE_CHECKBOX_SETCHECK );
    m->NewState = IsCheck;
    SendMessage( m );
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
        boost::shared_ptr<GUIMESSAGE_CHECKBOX_SETCHECK> p( new GUIMESSAGE_CHECKBOX_SETCHECK );
        p->NewState = !m_IsCheck;
        PostMessage( p );
      }

    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& m = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(*Param);
      m_IsCheck = m.NewState;
      OnStateChange(m.NewState);
    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
