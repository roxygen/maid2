#include"guiradiogroup.h"
#include"iguiradiobutton.h"


namespace Maid
{

GUIRadioGroup::GUIRadioGroup()
:m_pSelectted(NULL)
{
}


IGUIRadioButton* GUIRadioGroup::GetCheckButton()
{
  return m_pSelectted;
}



}
