#include"guiradiogroup.h"
#include"guiradiobutton.h"


namespace Maid
{

GUIRadioGroup::GUIRadioGroup()
:m_pSelectted(NULL)
{
}


GUIRadioButton* GUIRadioGroup::GetCheckButton()
{
  return m_pSelectted;
}



}
