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
    virtual MESSAGERETURN OnMouseClick( const POINT2DI& pos );
    virtual MESSAGERETURN OnStateChange( bool NewState ) { return MESSAGERETURN_SENDPARENT; }


    bool  m_IsCheck;
  };


}
#endif