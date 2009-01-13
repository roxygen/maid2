#ifndef maid2_framework_gui_guiradiobutton_h
#define maid2_framework_gui_guiradiobutton_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guibutton.h"

namespace Maid
{
  class GUIRadioButton : public GUIButton
  {
  public:
    GUIRadioButton();

    void SetCheck( bool IsCheck );
    bool IsCheck()const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual void OnCheck() {}

    bool  m_IsCheck;
  };
}
#endif