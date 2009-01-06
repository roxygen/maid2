#include"app.h"

using namespace Maid;


App* GlobalPointer<App>::s_pPointer;


bool App::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  return true;
}

void App::Initialize()
{
  GlobalPointer<App>::Set();

  m_Render.Initialize();

  m_SceneAdmin.Initialize();

  m_State = STATE_INITIALIZING;

}

void App::UpdateFrame()
{
  switch( m_State )
  {
  case STATE_INITIALIZING:
    {
      if( m_Render.IsInitializing() ) { return ; }
      if( m_SceneAdmin.IsInitializing() ) { return ; }

      m_State = STATE_PLAY;


    }break;

  case STATE_PLAY:
    {
      m_SceneAdmin.UpdateFrame();
    }break;
  }

}

void App::UpdateDraw()
{

  switch( m_State )
  {
  case STATE_INITIALIZING:
    {

    }break;

  case STATE_PLAY:
    {
      m_SceneAdmin.UpdateDraw();
    }break;
  }


}

void App::Finalize()
{

}


