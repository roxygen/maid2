#include"gameframeworkbase.h"

namespace Maid
{

GameFrameWorkBase::GameFrameWorkBase( const SPGAMETHREAD& pGame )
  :m_pGame(pGame)
{

}

void GameFrameWorkBase::OnSetup()
{
  m_pGame->OnSetup();

  //  m_pGame->OnSetup() 内部で終了することもある
  if( m_pGame->IsExitGame() )
  {
    OnExit(0);
    return ;
  }

  //  ＯＳ特有の初期化
  LocalSetup();

  {
    //  ゲームスレッドの作成
	  m_Thread.SetFunc( MakeThreadObject(&GameFrameWorkBase::ThreadFunction,this) );
	  m_Thread.Execute();
  }
}

void GameFrameWorkBase::OnLoop()
{
  LocalLoop();

	{
		//	スレッドが終了してたら終了
		if( m_Thread.IsThreadExited() )
		{
			if( m_Thread.IsThreadAssert() )
			{
				//	エラー終了だったら没
			}else
			{
				ThreadMutexLocker Lock(m_MessageMutex);
			//	m_GraphicCore.DrawFrame(m_ReturnMessageQue);
			}

			OnExit(0);
			return ;
		}
	}
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

void GameFrameWorkBase::OnCleanup()
{
  m_Thread.Close();
  LocalCleanup();
}

}


