/*
  GUIパーツサンプル
*/

#include"app.h"


using namespace Maid;



bool MyApp::SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
{
   return true;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();

  m_Frame = 0;

  m_Render.Initialize();
  m_Manager.SetReviceFunction( MakeReciveMessageFunction(&MyApp::ReciveMessage,this) );


  //  これ、サンプルなので GraphicsRender を渡したりしてるけど
  //  どうせならGUIクラスを GlobalPointer<GraphicsRender> で継承して使うのがいいと思う
  //  GlobalPointer<MyApp> とやるのなら音とかも使えるようになるし、相当ベンリかも

  GraphicsRender* pRender = &m_Render;


  {
    m_Button.Initialize( pRender, SIZE2DI(100,100) );
    m_Button.SetPosition( POINT2DI(250,100) );

    m_Manager.RegisterParts( 1, m_Button );
  }
  {
    m_CheckBox.Initialize( pRender );
    m_CheckBox.SetPosition( POINT2DI(250,250) );

    m_Manager.RegisterParts( 2, m_CheckBox );
  }
  {
    for( int i=0; i<NUMELEMENTS(m_RadioButton); ++i )
    {
      RadioButtonSample& b = m_RadioButton[i];

      b.Initialize( pRender );
      b.SetPosition( POINT2DI(450,100+150*i) );
      b.SetGroup( m_RadioGroup );

      m_Manager.RegisterParts( 3+i, b );
    }
  }


  {
    m_Slider.Initialize( pRender );
    m_Slider.SetPosition( POINT2DI(200,380) );
    m_Slider.SetValueMin(-100);
    m_Slider.SetValueMax(100);
    m_Slider.SetBarLength(200);

    m_Manager.RegisterParts( 10, m_Slider );
  }


  {
    m_Slider20.Initialize( pRender );
    m_Slider20.SetPosition( POINT2DI(200,430) );
    m_Slider20.SetValueMin(-50);
    m_Slider20.SetValueMax(50);
    m_Slider20.SetRotate( DEGtoRAD(20.0f) );  //  ２０度傾く
    m_Slider20.SetBarLength(200);

    m_Manager.RegisterParts( 11, m_Slider20 );
  }
  {
    m_Slider20b.Initialize( pRender );
    m_Slider20b.SetPosition( POINT2DI(200,530) );
    m_Slider20b.SetValueMin(-50);
    m_Slider20b.SetValueMax(50);
    m_Slider20b.SetBarLength(200);
    m_Slider20b.SetButtonLength(20);

    m_Manager.RegisterParts( 12, m_Slider20b );
  } 


  {
    m_WindowButton.Initialize( pRender, SIZE2DI(100,20) );
    m_WindowButton.SetPosition( POINT2DI(0,50) );

    m_Window.Initialize( pRender );
    m_Window.SetPosition( POINT2DI(0,0) );

    m_Window.AddChildParts( 100, m_WindowButton );

    m_Manager.RegisterParts( 13, m_Window );
    m_Window.SetZOrder(IGUIParts::ZORDER_BACK);
  }

  {
    m_TextBox.Initialize( pRender );
    m_TextBox.SetPosition( POINT2DI(0,570) );
    m_TextBox.SetTextLengthByte( 10 );
    m_Manager.RegisterParts( 20, m_TextBox );

  }

  m_hFont.Create( SIZE2DI(8,16), true );
}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  const TextInput& t = GetTextInput();
  GraphicsCore& core = GetGraphicsCore();

  if( m_Render.IsInitializing() ) { return ; }

  ++m_Frame;
  m_Manager.UpdateFrame( &m, &k, &t );
}

void MyApp::UpdateDraw()
{
  //  定期的に描画するとこ

  m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

  if( m_Render.IsInitializing() ) { return ; }

  const RenderTargetBase& rt = GetGraphicsCore().GetRenderTarget();
  const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

  m_Render.Begin();

  m_Manager.UpdateDraw(rt,ds );


  {
    m_Render.BltText( POINT2DI(0,0), m_hFont, MAIDTEXT(" time :id:mess"), COLOR_R32G32B32A32F(1,1,1,1) );

    POINT2DI pos(0,m_hFont.GetSize().h);
    for( std::list<String>::iterator ite=m_MessageLog.begin();
                                    ite!=m_MessageLog.end();
                                    ++ite )
    {
      m_Render.BltText( pos, m_hFont, (*ite), COLOR_R32G32B32A32F(1,1,1,1) );
      pos.y += m_hFont.GetSize().h;
    }
  }
  m_Render.End();
}

void MyApp::Finalize()
{
  // OnExitGame() を呼んだあとの処理

}

void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
{
  String str;
  {
    char buf[256];
    sprintf( buf, "%06d %02d", m_Frame, id );
    str = String::ConvertSJIStoMAID(buf) + MAIDTEXT(" ") + CreateMessageLog(param);
  }

  m_MessageLog.push_back(str);

  if( m_MessageLog.size()>30 ) { m_MessageLog.pop_front(); }
}



