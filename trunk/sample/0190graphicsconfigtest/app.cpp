#include"app.h"
#include"startupdialog.h"

using namespace Maid;

  enum SAMPLEID
  {
    ID_RELOADBUTTON,

    ID_SCREEN640x480,
    ID_SCREEN800x600,
    ID_SCREEN1024x768,

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

  m_FontM.Create( SIZE2DI(8,16), true );

  m_FrameCount = 0;
  m_DrawCount = 0;
  m_FPS = 0.0f;


  m_Window.Initialize( pRender );
  m_Window.SetPosition( POINT2DI(0,0) );

  {
    m_ReloadButton.Initialize( pRender, SIZE2DI(80,25), MAIDTEXT("リロード") );
    m_ReloadButton.SetPosition( POINT2DI(5,35) );

    m_Window.AddChildParts( ID_RELOADBUTTON, m_ReloadButton );
  }

  {
    m_Screen640x480.Initialize( pRender, SIZE2DI(55,25), MAIDTEXT("VGA") );
    m_Screen640x480.SetPosition( POINT2DI(5,65) );

    m_Window.AddChildParts( ID_SCREEN640x480, m_Screen640x480 );

    m_Screen800x600.Initialize( pRender, SIZE2DI(55,25), MAIDTEXT("SVGA") );
    m_Screen800x600.SetPosition( POINT2DI(65,65) );

    m_Window.AddChildParts( ID_SCREEN800x600, m_Screen800x600 );

    m_Screen1024x768.Initialize( pRender, SIZE2DI(55,25), MAIDTEXT("XGA") );
    m_Screen1024x768.SetPosition( POINT2DI(125,65) );

    m_Window.AddChildParts( ID_SCREEN1024x768, m_Screen1024x768 );
  }
  m_Manager.RegisterParts( ID_CONFIGWINDOW, m_Window );

  ReloadImage();
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

  ++m_PixivTanAngle;
  m_PixivTanAngle %= 360;
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



  if( !IsImageLoading() )
  {

    {
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_NONE );
      const RECT2DI  rc( POINT2DI(0,0), m_BG.GetImageSize() );
      const POINT2DI center(rc.w/2,rc.h/2);
		  m_Render.Blt( POINT2DI(400,300), m_BG, rc, center, 1.0f );
    }
    {
      const RECT2DI  rc( POINT2DI(0,0), m_PixivTan.GetImageSize() );
      const POINT2DI center(rc.w/2,760);

      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_SCREEN );
      m_Render.BltSR( POINT2DI(600,600), m_PixivTan, rc, center, 1.0f, SIZE2DF(0.8f,0.8f), DEGtoRAD(-m_PixivTanAngle), Maid::VECTOR3DF(0,1,0) );
    }
    {
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
      const RECT2DI  rc( POINT2DI(0,0), m_ITChan.GetImageSize() );
      const POINT2DI center(rc.w/2,413);
	    m_Render.Blt( POINT2DI(200,600), m_ITChan, rc, center, 1.0f );
	    m_Render.BltS( POINT2DI(50,600), m_ITChan, rc, center, 1.0f, SIZE2DF(0.3f,0.3f) );
    }


  }else
  {
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Render.BltText( POINT2DI(0,  16), m_FontM, MAIDTEXT("Loading"), COLOR_R32G32B32A32F(1,1,1,1) );
  }

  m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
  m_Manager.UpdateDraw(rt,ds);
  {
    //  FPS表示 
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Render.BltText( POINT2DI(0,  0), m_FontM, String::PrintFormat( "fps:%f",m_FPS), COLOR_R32G32B32A32F(0,0,1,1) );
  }

  m_Render.End();

}

void MyApp::Finalize()
{

}



void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
{
  switch( param.Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      switch( id )
      {
      case ID_RELOADBUTTON:
        {
          ReloadImage();
        }break;

      case ID_SCREEN640x480:
        {
          SetScreenSize( SIZE2DI(640,480) );
        }break;

      case ID_SCREEN800x600:
        {
          SetScreenSize( SIZE2DI(800,600) );
        }break;

      case ID_SCREEN1024x768:
        {
          SetScreenSize( SIZE2DI(1024,768) );
        }break;

      }
    }break;
  }
}


void MyApp::ReloadImage()
{
  m_BG.LoadFile( MAIDTEXT("nc4758.png") );

  m_PixivTan.LoadFile( MAIDTEXT("nc1429.png") );
  m_ITChan.LoadFile( MAIDTEXT("nc3258.png") );

  m_PixivTanAngle = 0;

  m_State = STATE_LOADING;
}

bool MyApp::IsImageLoading()
{
  if( m_State==STATE_EXECUTING ) { return false; }

  return  m_BG.IsLoading()
    ||    m_ITChan.IsLoading()
    ||    m_PixivTan.IsLoading()
    ;
}
