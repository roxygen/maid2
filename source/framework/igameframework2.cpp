#include"igameframework.h"

namespace Maid
{


void IGameFrameWork::ExecuteOSMessage()
{
	if( m_pGame.get()==NULL ) { return ; }

	OSMESSAGEMESSAGEQUE que;

	{
		ThreadMutexLocker Lock(m_MessageMutex);
		m_OSMessageQue.swap( que );
	}


	for( int i=0; i<(int)que.size(); ++i )
	{
		m_pGame->OnOSMessage( que[i] ); 
	}
}

unt IGameFrameWork::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
	{

		IGameThread::BEGINPARAM	param;
/*		param.pInfo = &m_FrameInfo;
		param.pFontDriver = SPFONTDRIVER( new CFontDriverWindows() );
		param.pKeybordDriver  = m_pKeybordDriver;
		param.pMouseDriver    = m_pMouseDriver;
		param.pCharCodeDriver = m_pCharCodeDriver;
		param.pIMEDriver = m_pIMEDriver;
*/
		m_pGame->Initialize( param );
	}

	while( true )
	{
		if( brige.IsExit ) { break; }
		if( m_pGame->IsExitGame() ) { break; }

		ExecuteOSMessage();
		m_pGame->OnLoop();
	}

	m_pGame->Finalize();

	m_pGame.reset();

//	RETURNMESSAGEQUE ret;

//	OnNextFrame(ret);
  return 0;
}

}


