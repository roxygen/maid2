﻿#include"processmutex.h"
#include"../../debug/warning.h"

namespace Maid
{


    #pragma COMPILERMSG("TODO:map<ProcessMutex> とかやるとコンストラクタの共有とかでダメポになりそう")	


/*!
 	@class ProcessMutex processmutex.h
 	@brief プロセスレベルの同期オブジェクト
 */


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
ProcessMutex::ProcessMutex()
{
	m_hMutex = NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
ProcessMutex::~ProcessMutex()
{
	Close();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ミューテクスを作成する
/*!
  @param  MutexName   [i ]	作成するミューテクス名
  @param  IsOriginal	[i ]	唯一のミューテックスにするか？
 */
void ProcessMutex::Open( const String& MutexName, bool IsOriginal )
{
	Close();

	const std::wstring sjis_name = String::ConvertMAIDtoUNICODE(MutexName);

	m_hMutex = ::CreateMutex( NULL, TRUE, sjis_name.c_str() );
	if( m_hMutex==NULL ) 
	{
		MAID_WARNING(MutexName + MAIDTEXT("ミューテックスの作成に失敗") );
    return ;
	}

	if( IsOriginal && (::GetLastError()==ERROR_ALREADY_EXISTS) ) 
	{
		::CloseHandle(m_hMutex); 
		MAID_WARNING(MutexName + MAIDTEXT("ミューテックスはすでに存在しています") );
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ミューテクスを閉じる
/*!
 */
void ProcessMutex::Close()
{
	if( m_hMutex!=NULL )
	{
		::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したミューテックスがすでに存在しているか調べる
/*!
    @param  MutexName		[i ]	調べるミューテクス名

    @return	存在していたら true
\n			    存在していなかったら false
 */
bool ProcessMutex::IsExist(  const String& MutexName )
{
	const std::wstring sjis_name = String::ConvertMAIDtoUNICODE(MutexName);

	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, sjis_name.c_str());

	if( hMutex != NULL ) { ::CloseHandle(hMutex); return true; }

	return false;
}


}
