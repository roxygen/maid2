#include"guibutton.h"



namespace Maid
{


void GUIButton::UpdateFrame()
{

}

void GUIButton::UpdateDraw( RenderTargetBase& Target )
{

}

void GUIButton::SendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_CREATE:
    {

    }break;

  case IGUIParam::MESSAGE_DESTROY:
    {

    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {

    }break;
  }
}

}
