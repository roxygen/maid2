#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"iguiparts.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    class Listner
    {
    public:
      virtual ~Listner(){}

      virtual void Initialize(){}
      virtual void Finalize(){}
      virtual void UpdateFrame(){}
      virtual void UpdateDraw( RenderTargetBase& Target, const POINT2DI& pos ){}

      virtual void SetEnable ( bool IsEnable  ){}
      virtual void SetVisible( bool IsVisible ){}
    };

    GUIButton();


    void SetListner( const boost::shared_ptr<Listner>& pListner );

    void SendMessage( const IGUIParam& Param );
  protected:
    void LocalUpdateFrame();
    void LocalUpdateDraw( RenderTargetBase& Target, const POINT2DI& pos );

  private:

    boost::shared_ptr<Listner>  m_pListner;
  };

}
#endif