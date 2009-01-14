#include"guiradiobutton.h"
#include"guiradiogroup.h"

namespace Maid
{

GUIRadioButton::GUIRadioButton()
:m_IsCheck(false),m_pGroup(NULL)
{

}

void GUIRadioButton::SetCheck()
{
  GUIMESSAGE_RADIOBUTTON_SETCHECK m;
  SendMessage( m );
}

bool GUIRadioButton::IsCheck()const
{
  return m_IsCheck;
}

void GUIRadioButton::SetGroup( GUIRadioGroup& Group )
{
  m_pGroup = &Group;

  if( m_pGroup->m_pSelectted==NULL )
  { //  まだどこもチェックされていないなら自分をチェックしておく
    SetCheck();
  }
}


GUIRadioButton::MESSAGERETURN GUIRadioButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& m = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(*Param);

      {
        GUIMESSAGE_RADIOBUTTON_SETCHECK post;
        PostMessage( post );
      }

    }break;

  case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK:
    {
      const GUIMESSAGE_RADIOBUTTON_SETCHECK& m = static_cast<const GUIMESSAGE_RADIOBUTTON_SETCHECK&>(*Param);

      //  すでにチェックされていたら届けない
      if( m_IsCheck ) { return GUIRadioButton::MESSAGERETURN_NONE; }

      if( m_pGroup->m_pSelectted!=NULL )
      {
        m_pGroup->m_pSelectted->m_IsCheck = false;
      }
      m_pGroup->m_pSelectted = this;

      m_IsCheck = true;
      OnCheck();
    }break;
  }

  return GUIButton::MessageExecuting( Param );
}

}
