#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"iguiparts.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    GUIButton();

    bool IsButtonDown() const { return m_IsButtonDown; }
    bool IsMouseIn() const { return m_IsMouseIn; }

  protected:
    virtual void LocalSendMessage( const IGUIParam& Param );

    virtual void OnMouseIn( const POINT2DI& pos ){}
    virtual void OnMouseOut( const POINT2DI& pos ){}

    virtual void OnMouseDown( const POINT2DI& pos, int Button ){}
    virtual void OnMouseUp( const POINT2DI& pos, int Button ){}
    virtual void OnMouseClick( const POINT2DI& pos ){}

  private:
    bool  m_IsMouseIn;
    bool  m_IsButtonDown;
  };

}
#endif