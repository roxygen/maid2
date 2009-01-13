#ifndef maid2_framework_gui_guicheckbox_h
#define maid2_framework_gui_guicheckbox_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guibutton.h"

namespace Maid
{
  class GUICheckBox : public GUIButton
  {
  public:
    GUICheckBox();

    void SetCheck( bool IsCheck );
    bool IsCheck()const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual void OnStateChange( bool NewState ) {}


    bool  m_IsCheck;
  };


}
#endif