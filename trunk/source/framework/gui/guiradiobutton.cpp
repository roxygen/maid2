#include"guiradiobutton.h"

namespace Maid
{

GUIRadioButton::GUIRadioButton()
:m_IsCheck(false)
{

}

void GUIRadioButton::SetCheck( bool IsCheck )
{
  if( m_IsCheck == IsCheck ) { return ; }

  //  未チェック->チェックのときのみメッセージを送る
  if( !m_IsCheck && IsCheck )
  {
    boost::shared_ptr<GUIMESSAGE_RADIOBUTTON_SETCHECK> m( new GUIMESSAGE_RADIOBUTTON_SETCHECK );
    SendMessage( m );
  }
}

bool GUIRadioButton::IsCheck()const
{
  return m_IsCheck;
}

GUIRadioButton::MESSAGERETURN GUIRadioButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& m = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(*Param);

      if( !m_IsCheck )
      {
        boost::shared_ptr<GUIMESSAGE_RADIOBUTTON_SETCHECK> p( new GUIMESSAGE_RADIOBUTTON_SETCHECK );
        PostMessage( p );
      }
    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& m = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(*Param);

      if( !m_IsCheck )
      {
        m_IsCheck = true;
        OnCheck();
      }
    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
