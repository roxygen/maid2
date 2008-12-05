#include"igameframework.h"

namespace Maid
{

IGameFrameWork::IGameFrameWork( const SPGAMETHREAD& pGame )
  :m_pGame(pGame)
{

}

bool IGameFrameWork::BootCheck()
{
 #pragma  COMPILERMSG( "TODO:ここに２重起動チェック" )
 return true;
}

void IGameFrameWork::Initialize()
{
  Graphics::IDeviceList& GraphicsDeviceList = GetGraphicsDeviceList();

  { //  各種デバイスリストの初期化
    GraphicsDeviceList.Initialize();
  }

  std::vector<Graphics::IDeviceList::INFO> GraphicsDevice;

  { //  使えるデバイスリストの取得
    GraphicsDeviceList.GetList(GraphicsDevice);
  }


  {
    const bool ret = m_pGame->SelectDevice(GraphicsDevice);

    //  m_pGame->SelectDevice() 内部で終了することもある
    if( !ret )
    {
      OnExit(0);
      return ;
    }
  }
  const IGameThread::DEFAULTCONFIG& Config = m_pGame->GetDefaultConfig();

  //  ＯＳ特有の初期化
  LocalInitialize(Config);

  {
    //  各種デバイスの初期化
    {
      Graphics::IDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphics.IsSoftwareVertexProcessing;
      Graphics::SPDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID,param );
      m_Graphics.Initialize( pGraphics );
    }


  }
  {
    //  ゲームの初期化
		IGameThread::BEGINPARAM	param;
    param.DrawCommand.push_back( m_Graphics.GetDevice()->CreateDrawCommandPlayer() );


    m_pGame->Initialize(param);
  }
}

void IGameFrameWork::OnLoop()
{
  LocalLoop();


/*

	if( m_IsFrameUpdating )
	{
		//	キューにたまっているものを描画
		ThreadMutexLocker Lock(m_MessageMutex);


//		m_GraphicCore.DrawFrame( m_ReturnMessageQue );
		m_IsFrameUpdating = false;
	}else
	{
		::Sleep(1);
		return ;
	}
*/
}

void IGameFrameWork::Finalize()
{
  m_Thread.Close();
  LocalFinalize();
}

}


