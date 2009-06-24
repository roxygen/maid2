﻿#include"registry.h"
#include"../../auxiliary/debug/assert.h"



namespace Maid
{

/*!
    @class Registry registry.h
    @brief レジストリキーを取得・作成するクラス

    すでにキーが存在しているときは上書きします
 */


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!	
 */
Registry::Registry()
{
  m_hKey = NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!	
 */
Registry::~Registry()
{
  Close();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! レジストリキーを開く
/*!	
  @param  hKey    [i ]  大元のキー
  @param  SubKey  [i ]  サブキーの名前

  @exception Exception オープンに失敗した場合
 */
void Registry::Open( HKEY hKey, const String& SubKey )
{
  Close();

  const std::wstring unicode_sub = String::ConvertMAIDtoUNICODE(SubKey);

  if( ::RegOpenKeyEx( hKey, unicode_sub.c_str(), 0, KEY_ALL_ACCESS, &m_hKey )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegOpenKeyEx()"));
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! レジストリキーを作成、開く
/*!	
  @param  hKey    [i ]  大元のキー
  @param  SubKey  [i ]  サブキーの名前

  @exception Exception  キーを作成して、オープンに失敗した場合
                        キーが存在していて、オープンに失敗した場合
 */
void Registry::Create( HKEY hKey, const String& SubKey )
{
  Close();
  const std::wstring unicode_sub = String::ConvertMAIDtoUNICODE(SubKey);

  DWORD ret=0;

  const LONG code = ::RegCreateKeyEx( hKey, unicode_sub.c_str(), 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &ret );
  if( code!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegCreateKeyEx()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! DWORD 値をセットする
/*!	
    @param  Name  [i ]  作成する名前
    @param  Data  [i ]  セットする値

    @exception Exception 関数の失敗時
 */
void Registry::SetValue( const String& Name, unt32 Data )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);
  if( ::RegSetValueEx( m_hKey, unicode_name.c_str(), 0, REG_DWORD, (BYTE*)&Data, sizeof(Data) )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegSetValueEx()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 値をセットする
/*!	
    @param  Name  [i ]  作成する名前
    @param  Data  [i ]  セットする文字列

    @exception exception 関数の失敗時
 */
void Registry::SetValue( const String& Name, const String& Data )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);
  const std::wstring unicode_data = String::ConvertMAIDtoUNICODE(Data);
  if( ::RegSetValueEx( m_hKey, unicode_name.c_str(), 0, REG_SZ, (BYTE*)unicode_data.c_str(), (DWORD)unicode_data.length()+1 )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegSetValueEx()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 値をセットする
/*!	
    @param  Name    [i ]  作成する名前
    @param  pData   [i ]  セットするバイナリデータ
    @param  Length  [i ]  _pData の長さ

    @exception CException 関数の失敗時
 */
void Registry::SetValue( const String& Name, const void* pData, unt32 Length )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);
  if( ::RegSetValueEx( m_hKey, unicode_name.c_str(), 0, REG_BINARY, (BYTE*)pData, Length+1 )!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegSetValueEx()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! DWORD 値を取得する
/*!	
    @param  Name  [i ]  取得する名前
    @param  Data  [ o]  取得するDWORD値

    @exception Exception 関数の失敗時
 */
void Registry::GetValue( const String& Name, unt32& Data )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  DWORD	dwKeyType;
  DWORD	dwKeyData;
  DWORD	dwKeyBuffLen = 4;
  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);

  if( ::RegQueryValueEx( m_hKey, unicode_name.c_str(), NULL, &dwKeyType, (BYTE*)&dwKeyData, &dwKeyBuffLen )!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegQueryValueEx()"));
  }

  if( dwKeyType!=REG_DWORD ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("dwKeyType!=REG_DWORD"));
  }
  Data = dwKeyData;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 文字列 値を取得する
/*!	
    @param  Name  [i ]  取得する名前
    @param  Data  [ o]  取得する文字列

    @exception Exception 関数の失敗時
 */
void Registry::GetValue( const String& Name, String& Data )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);

  DWORD dwKeyType;
  DWORD dwKeyBuffLen=0;

  if( ::RegQueryValueEx( m_hKey, unicode_name.c_str(), NULL, &dwKeyType, NULL, &dwKeyBuffLen )!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegQueryValueEx()"));
  }
  if( dwKeyType!=REG_SZ )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("dwKeyType!=REG_SZ"));
  }

  std::wstring s;
  s.resize(dwKeyBuffLen-1);

  if( ::RegQueryValueEx( m_hKey, unicode_name.c_str(), NULL, &dwKeyType, (BYTE*)s.data(), &dwKeyBuffLen )!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegQueryValueEx()"));
  }

  Data = String::ConvertUNICODEtoMAID(s);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! バイナリデータ 値を取得する
/*!	
    @param  Name    [i ]  取得する名前
    @param  pData   [ o]  取得するバイナリデータ
    @param  Length  [ o]  バイナリデータの長さ

    @exception Exception 関数の失敗時
 */
void Registry::GetValue( const String& Name, boost::shared_array<unt08>& pData, unt32& Length )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  DWORD dwKeyType;
  DWORD dwKeyBuffLen=0;

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(Name);

  if( ::RegQueryValueEx( m_hKey, unicode_name.c_str(), NULL, &dwKeyType, NULL, &dwKeyBuffLen )!=ERROR_SUCCESS )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegQueryValueEx()"));
  }
  if( dwKeyType!=REG_BINARY ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("dwKeyType!=REG_BINARY"));
  }

  pData.reset( new unt08[dwKeyBuffLen] );

  if( ::RegQueryValueEx( m_hKey, unicode_name.c_str(), NULL, &dwKeyType, pData.get(), &dwKeyBuffLen )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegQueryValueEx()"));
  }
  Length = dwKeyBuffLen;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 値を削除する
/*!
  @param  ValueName [i ]  削除する値の名前
 
  @exception Exception 関数の失敗時
 */
void Registry::DeleteValue( const String& ValueName )
{
  MAID_ASSERT( m_hKey==NULL, "キーを開いていません" );
  if( m_hKey==NULL ) { return ; }

  const std::wstring unicode_name = String::ConvertMAIDtoUNICODE(ValueName);
  if( ::RegDeleteValue( m_hKey, unicode_name.c_str() )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegDeleteValue()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! レジストリの操作を終了する
/*!
 */
void Registry::Close()
{
  if( m_hKey!=NULL )
  {
    ::RegCloseKey( m_hKey );
    m_hKey = NULL;
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! レジストリキーの削除
/*!	
    @param  hKey    [i ]  削除したいキー
    @param  SubKey  [i ]  削除したいサブキー

    @exception Exception 削除の失敗時
 */
void Registry::DeleteKey( HKEY hKey, const String& SubKey )
{
  const std::wstring unicode_key = String::ConvertMAIDtoUNICODE(SubKey);
  if( ::RegDeleteKey( hKey, unicode_key.c_str() )!=ERROR_SUCCESS ) 
  {
    MAID_THROWEXCEPTION(MAIDTEXT("RegDeleteKey()"));
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! レジストリの値の削除
/*!	
    @param    hKey      [i ]  削除したいキー
    @param    SubKey    [i ]  探すサブキー
    @param    ValueName [i ]  削除したい値

    @exception Exception 削除の失敗時
*/
void Registry::DeleteValue( HKEY hKey, const String& SubKey, const String& ValueName )
{
  Registry hReg;
  hReg.Open( hKey, SubKey );
  hReg.DeleteValue( ValueName );
}


}