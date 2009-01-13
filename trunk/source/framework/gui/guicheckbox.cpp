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

  m_IsCheck = IsCheck;

  if( OnStateChange(IsCheck)==MESSAGERETURN_SENDPARENT )
  {
    GUIMESSAGE_CHECKBOX_SETCHECK m;
    m.IsCheck = IsCheck;
    SendMessageParent( m );
  }
}

bool GUICheckBox::IsCheck()const
{
  return m_IsCheck;
}

GUICheckBox::MESSAGERETURN GUICheckBox::OnMouseClick( const POINT2DI& pos )
{
  const bool NewState = !m_IsCheck;

  m_IsCheck = NewState;
  if( OnStateChange(NewState)==MESSAGERETURN_SENDPARENT )
  {
    GUIMESSAGE_CHECKBOX_SETCHECK m;
    m.IsCheck = NewState;
    SendMessageParent( m );
  }

  return MESSAGERETURN_SENDPARENT;
}

}
