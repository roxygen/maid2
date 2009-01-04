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

  m_Render.Initialize( GetGraphicsCore() );

  m_SceneAdmin.Initialize( GetGraphicsCore() );

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
  m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

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


