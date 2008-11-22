#include"igameframework.h"

namespace Maid
{
/*
void GameFrameWorkBase::OnNextFrame( RETURNMESSAGEQUE& ret )
{
	while( true )
	{
		CThreadMutexLocker Lock(m_MessageMutex);

		CGraphic::FRAMEINFO* pInfo = &m_FrameInfo;

		if( !m_IsFrameUpdating )
		{
			//	描画が終わっていたら、キューを交換する
			m_GraphicCore.SwapRenderMessage( pInfo->que );
		}else
		{
			//	終わっていなくても、強制実行命令が出ている場合は待つ
			if( pInfo->IsForce ) { ::Sleep(0); continue; }
		}

		ret.swap(m_ReturnMessageQue);
		pInfo->que.resize(0);
		pInfo->IsForce = false;

		m_IsFrameUpdating = true;
		break;
	}
}
*/

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


