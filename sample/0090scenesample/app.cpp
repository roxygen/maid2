#include"app.h"

using namespace Maid;


App* GlobalPointer<App>::s_pPointer;


bool App::LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
{
  return true;
}

void App::LocalInitialize( const BEGINPARAM& param )
{
  GlobalPointer<App>::Set();

  m_Render.Initialize( param.ScreenSize, param.DrawCommand[0], param.pFontDevice );

  m_SceneAdmin.Initialize( param.ScreenSize, param.DrawCommand[0], param.pFontDevice );

  m_State = STATE_INITIALIZING;

}

void App::LocalUpdateFrame()
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

void App::LocalUpdateDraw()
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

void App::LocalFinalize()
{

}


