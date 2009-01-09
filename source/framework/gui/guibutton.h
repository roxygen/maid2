#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"iguiparts.h"

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    virtual void SendMessage( const IGUIParam& Param );
    virtual void UpdateFrame();
    virtual void UpdateDraw( RenderTargetBase& Target );
  };

}
#endif