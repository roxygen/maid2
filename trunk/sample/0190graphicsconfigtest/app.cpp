#include"app.h"
#include"startupdialog.h"

using namespace Maid;

  enum SAMPLEID
  {
    ID_RELOADBUTTON,


    ID_CONFIGWINDOW,
  };


bool MyApp::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
  if( ret==APPEXIT ) { return false; }
  return true;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  Graphics2DRender* pRender = &m_Render;

  m_Manager.SetReviceFunction( MakeReciveMessageFunction(&MyApp::ReciveMessage,this) );

  m_Render.Initialize();

  m_FontM.Create( SIZE2DI(16,32), true );

  m_FrameCount = 0;
  m_DrawCount = 0;
  m_FPS = 0.0f;


  m_Window.Initialize( pRender );
  m_Window.SetPosition( POINT2DI(0,0) );

  {
    m_ReloadButton.Initialize( pRender, SIZE2DI(80,25), MAIDTEXT("リロード") );
    m_ReloadButton.SetPosition( POINT2DI(4,35) );

    m_Window.AddChildParts( ID_RELOADBUTTON, m_ReloadButton );
  }

  m_Manager.RegisterParts( ID_CONFIGWINDOW, m_Window );

}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  const TextInput& t = GetTextInput();
  GraphicsCore& core = GetGraphicsCore();

  ++m_FrameCount;

  if( m_Render.IsInitializing() ) { return ; }

  m_Manager.UpdateFrame( &m, &k, &t );
}

void MyApp::UpdateDraw()
{
  ++m_DrawCount;
  if( 60<=m_FrameCount )
  {
    m_FPS = float(m_DrawCount) / float(m_FrameCount) * 60.0f;
    m_FrameCount = 0;
    m_DrawCount = 0;

  }

  if( m_Render.IsInitializing() ) { return ; }

  const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
  const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

  m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(0,0,0,0) );

  m_Render.Begin();


  m_Manager.UpdateDraw(rt,ds );

  {
    //  FPS表示 
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Render.BltText( POINT2DI(0,  0), m_FontM, String::PrintFormat( "fps:%f",m_FPS), COLOR_R32G32B32A32F(1,1,1,1) );
  }

  m_Render.End();

}

void MyApp::Finalize()
{

}



void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
{

}
