#include"dllwrapper.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/exception.h"

namespace Maid
{
/*!
 	@class	DllWrapper dllwrapper.h
 	@brief	DLL 操作のラッパクラス
 */


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
DllWrapper::DllWrapper()
:m_hDll(NULL)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
DllWrapper::~DllWrapper()
{
	Free();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! DLL の読み込み
/*!
    @param	DllName [i ]	読み込むＤＬＬ名

    @exception CExceptionNotDLL オープンに失敗した場合
 */
void DllWrapper::Load( const String& DllName )
{
	Free();

	const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(DllName);

	HINSTANCE hInst  = ::LoadLibrary( unicode_name.c_str() );

	if( hInst==NULL ) 
	{
		MAID_ASSERT( true, DllName << "が見つかりません" );
		MAID_THROWEXCEPTION( DllName + MAIDTEXT("が見つかりません") );
	}

	m_DllName = DllName;
	m_hDll    = hInst;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 関数アドレスの取得
/*!
    @param	FunctionName [i ]	取得したい関数名

    @return 関数がみつかったらそのアドレス
\n          みつからなかったらＮＵＬＬ
 */
FARPROC	DllWrapper::GetProcAddress( const String& FunctionName ) const
{
	MAID_ASSERT( m_hDll==NULL, "ＤＬＬを読み込んでいません" );
	if( m_hDll==NULL ) { return NULL; }

	const std::string sjis_name = String::ConvertMAIDtoSJIS(FunctionName);

	return ::GetProcAddress( m_hDll, sjis_name.c_str() );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 読み込んだＤＬＬの開放
/*!
 */
void DllWrapper::Free()
{
	if( m_hDll!=NULL )
	{
		::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

}