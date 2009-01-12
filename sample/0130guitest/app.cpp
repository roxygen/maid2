#include"app.h"

#include"buttonsample.h"
#include"checkboxsample.h"
#include"radiobuttonsample.h"



using namespace Maid;



bool MyApp::SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
{
   return true;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();

  m_Render.Initialize();
  m_Manager.Initialize( MakeReciveMessageFunction(&MyApp::ReciveMessage,this), &m, &k );

  {
    boost::shared_ptr<ButtonSample> pButton( new ButtonSample(m_Render) );

    pButton->SetOffset( VECTOR2DI(250,100) );

    m_Manager.RegisterParts( 1, pButton );
  }
  {
    boost::shared_ptr<CheckBoxSample> pCheck( new CheckBoxSample(m_Render) );

    pCheck->SetOffset( VECTOR2DI(250,250) );

    m_Manager.RegisterParts( 2, pCheck );
  }


  {
    std::vector<SPGUIRADIOBUTTON> Button;

    for( int i=0; i<3; ++i )
    {
      boost::shared_ptr<RadioButtonSample> pRadio( new RadioButtonSample(m_Render) );
      pRadio->SetOffset( VECTOR2DI(450,100+150*i) );

      Button.push_back( pRadio );
    }

    boost::shared_ptr<GUIRadioGroup> pGroup( new GUIRadioGroup );

    pGroup->SetRadioButton( Button );
    m_Manager.RegisterParts( 3, pGroup );
  }

  m_hFont.Create( SIZE2DI(8,16), true );
  m_Frame = 0;

}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  GraphicsCore& core = GetGraphicsCore();

  if( m_Render.IsInitializing() ) { return ; }

  ++m_Frame;
  m_Manager.UpdateFrame();
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
  if( param.Message==IGUIParam::MESSAGE_RADIDGROUP_CHANGESELECT )
  {
    int k=0;

  }


  String str;
  {
    char buf[256];
    sprintf( buf, "%06d %02d", m_Frame, id );
    str = String::ConvertSJIStoMAID(buf) + MAIDTEXT(" ") + CreateMessageLog(param);
  }

  m_MessageLog.push_back(str);

  if( m_MessageLog.size()>30 ) { m_MessageLog.pop_front(); }
}



