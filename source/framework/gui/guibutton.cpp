#include"guibutton.h"



namespace Maid
{

GUIButton::GUIButton()
  :m_pListner(new Listner)
{

}

void GUIButton::LocalUpdateFrame()
{
  m_pListner->UpdateFrame();
}

void GUIButton::LocalUpdateDraw( RenderTargetBase& Target, const POINT2DI& pos )
{
  m_pListner->LocalUpdateDraw(Target,pos);
}

void GUIButton::SendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {

    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {

    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {

    }break;
  }
}

}
