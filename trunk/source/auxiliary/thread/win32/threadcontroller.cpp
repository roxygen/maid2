#include"threadcontroller.h"
#include"../../../auxiliary/debug/assert.h"
#include"../../../auxiliary/debug/warning.h"
#include"../../../auxiliary/debug/trace.h"
#include"../../../auxiliary/exception.h"
#include"../../../config/Win32.h"

namespace Maid
{

	/*!
	 	@class	ThreadController threadcontroller.h
	 	@brief	スレッド作成クラス
	\n			スレッドで実行している関数側は以下のルールを守ること
	\n			BRIGEDATA::IsExit が true の時は即座に return すること
	\n
	\n			複数のスレッドで Cランタイムを使用する場合の注意
	\n
	\n				http://msdn2.microsoft.com/ja-jp/library/7t9ha0zh(VS.80).aspx
	\n
	\n				Libcmt.lib を使用してビルドしたプログラムから C ランタイム ルーチンを呼び出す場合は、
	\n				_beginthread 関数または _beginthreadex 関数でスレッドを起動する必要があります。
	\n				Win32 の ExitThread 関数および CreateThread 関数は使用しないでください。
	\n				また、C ランタイム ライブラリのデータ構造体へアクセス中のスレッドがあって、
	\n				その完了を待っている複数のスレッドが存在する場合に SuspendThread を使うと、
	\n				デッドロック状態になります。
	\n
	\n
	\n			分かりやすくいうと　２つのスレッドがそれぞれ printf() してるときに
	\n			片方のスレッドを SuspendThread() すると、デッドロックすることがあります。
	\n
	\n			うかつにとめないほうがいいみたいです。

	 */


void ThreadController::Sleep( unt time )
{
	::Sleep( time );
}

unt ThreadController::GetCurrentID()
{
  return GetCurrentThreadId();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
ThreadController::ThreadController()
{
	m_hThread = NULL;
	m_BrigeData.IsExit    = false;
	m_BrigeData.IsExecute = false;
  m_ThreadID = 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
ThreadController::~ThreadController()
{
	Close();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドで行う関数の設定
/*!
    @param	Function	[i ]	設定する関数
 */
void ThreadController::SetFunc( const THREADFUNCTION& Function )
{
	m_Func = Function;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドの起動
/*!
 */
void ThreadController::Execute( PRIORITY priority )
{
	Close();
	unt32 id;
	m_BrigeData.IsExecute = true;

	if( priority!=PRIORITY_NORMAL )
	{
		m_hThread = ::_beginthreadex( NULL, 0, Thread, this, CREATE_SUSPENDED, &id );
		SetPriority( priority );
		Resume();
	}else
	{
		m_hThread = ::_beginthreadex( NULL, 0, Thread, this, 0, &id );
	}

  m_ThreadID = id;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドの終了
/*!
 */
void ThreadController::Close()
{
	if( m_hThread!=NULL )
	{
		m_BrigeData.IsExit = true;
		while( m_BrigeData.IsExecute ) { ::WaitForSingleObject((HANDLE)m_hThread,100); }
		::CloseHandle( (HANDLE)m_hThread );
		m_hThread = NULL;
    m_ThreadID= 0;
	}

	m_BrigeData.IsExit    = false;
	m_BrigeData.IsExecute = false;
	m_BrigeData.IsAssert  = false;

	const String m = m_ExceptionText;
	m_ExceptionText.clear();

	if( !m.empty() )
	{
		MAID_THROWEXCEPTION(m);
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 優先度の変更
/*!
    @param	priority	[i ]	新しい優先度
 */
void ThreadController::SetPriority( PRIORITY priority )
{
	int p;
	switch( priority )
	{
	case PRIORITY_HIGH2:	{ p = THREAD_PRIORITY_HIGHEST; }break;
	case PRIORITY_HIGH1:	{ p = THREAD_PRIORITY_ABOVE_NORMAL; }break;
	case PRIORITY_NORMAL:	{ p = THREAD_PRIORITY_NORMAL; }break;
	case PRIORITY_LOW1:		{ p = THREAD_PRIORITY_BELOW_NORMAL; }break;
	case PRIORITY_LOW2:		{ p = THREAD_PRIORITY_LOWEST; }break;
	}
	::SetThreadPriority( (HANDLE)m_hThread, p );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドを一時止める
/*!
 */
void ThreadController::Pause()
{
	::SuspendThread( (HANDLE)m_hThread );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 停止していたスレッドを起動する
/*!
 */
void ThreadController::Resume()
{
	::ResumeThread( (HANDLE)m_hThread );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドの処理が終わったか？
/*!
 */
bool ThreadController::IsExited() const
{
	return !m_BrigeData.IsExecute;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドの処理が強制終了
/*!
 */
bool ThreadController::IsAssert() const
{
	return m_BrigeData.IsAssert;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドの戻り値の取得
/*!
    IsThreadExited()==true のときだけ呼び出すこと
 */
unt ThreadController::GetReturnCode() const
{
	MAID_ASSERT( !IsExited(), "スレッドは実行中です" );
	if( !IsExited() ) { return 0; }

	DWORD ret;
	::GetExitCodeThread( (HANDLE)m_hThread, &ret );
	return ret;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドIDの取得
/*!
    ハンドルじゃなくてＩＤね
 */
unt ThreadController::GetID() const
{
  return m_ThreadID;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドをメンバ関数で起動させるための踏み台関数
/*!
    @param	pVoid	[i ]	常に _beginthreadex を呼び出した CThread* であること
 */
unsigned __stdcall ThreadController::Thread( void* pVoid )
{
	unt RetCode = 0;
	ThreadController* pThis = (ThreadController*)pVoid;

  try
	{
		RetCode = pThis->m_Func( pThis->m_BrigeData );
	}
	catch( Exception& e )
	{
		//	別スレッドの例外はメインスレッドでキャッチできないので
		//	ログだけだして終了
		e.WriteLog();

    pThis->m_ExceptionText = String::ConvertSJIStoMAID(e.GetLog());
		pThis->m_BrigeData.IsAssert = true;
	}

	pThis->m_BrigeData.IsExecute = false;
	_endthreadex(RetCode);

	return RetCode;
}


}
