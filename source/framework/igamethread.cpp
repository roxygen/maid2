#include"igamethread.h"


namespace Maid
{

using namespace OSMessage;

const IGameThread::DEFAULTCONFIG& IGameThread::GetDefaultConfig() const
{
  return m_DefaultConfig;
}


void	IGameThread::OnSetup( const std::vector<IVideoDeviceList::INFO>& VideoDevice )
{
  DEFAULTCONFIG& config = m_DefaultConfig;

	config.WindowCaption = MAIDTEXT("GameFrameWork");
	config.Graphic.DeviceID = 0;
	config.Graphic.IsSoftwareVertexProcessing = true;
	config.Graphic.ScreenSize = SIZE2DI(800,600);
/*
	m_Config.Graphic.RefreshRate = 60;
	m_Config.Graphic.IsSoftTnL = false;
	m_Config.Graphic.IsWaitVSync = true;
	m_Config.Graphic.IsAspectLock = true;

	m_Config.Sound.SamplesPerSec = 44100;
	m_Config.Sound.BitsPerSample = 16;
	m_Config.Sound.Channels = 2;
*/
	m_IsExitGame = false;
	LocalSetup( VideoDevice, config );
}

void	IGameThread::OnBegin( const BEGINPARAM& param )
{

	LocalBegin(param);
}

void	IGameThread::OnLoop()
{
	LocalLoop();
}

void	IGameThread::OnEnd()
{
	LocalEnd();
}

/*
void GameThread::OnNextFrame( CGraphic& gra )
{
	RETURNMESSAGEQUE ret;

	gra.NextFrame();
	GetFrameWork()->OnNextFrame(ret);
	gra.OnReturnMessage(ret);
}
*/
bool IGameThread::IsExitGame()
{
	return m_IsExitGame;
}

void IGameThread::OnExitGame()
{
	m_IsExitGame = true;
}


void IGameThread::OnOSMessage( const SPOSMESSAGE& pMess )
{
  const OSMESSAGECALLBACK ret = LocalOnOSMessage( pMess );

  if( ret==OSMESSAGECALLBACK_RETURN ) { return ; }

  switch( pMess->Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  }
}



void IGameThread::LocalSetup( const std::vector<IVideoDeviceList::INFO>& VideoDevice, DEFAULTCONFIG& conf ){}
void IGameThread::LocalBegin( const BEGINPARAM& param ){}
void IGameThread::LocalLoop(){}
void IGameThread::LocalEnd(){}
IGameThread::OSMESSAGECALLBACK IGameThread::LocalOnOSMessage( const SPOSMESSAGE& pMess )
{
  return OSMESSAGECALLBACK_DEFAULT;
}

}