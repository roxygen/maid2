#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"../../config/define.h"
#include"iguiparts.h"

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    GUIButton();

    bool IsButtonDown() const { return m_IsButtonDown; }

  protected:
    virtual MESSAGERETURN OnSendMessage( const IGUIParam& Param );

    virtual MESSAGERETURN OnMouseClick( const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }

  private:
    bool  m_IsButtonDown;
  };

}
#endif