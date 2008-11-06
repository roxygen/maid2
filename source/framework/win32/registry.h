/*!
 	@file
 	@brief レジストリキーを取得・作成するクラス
*/

#ifndef framework_win32_registry_h
#define framework_win32_registry_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/exception.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Registry
  {
  public:
    Registry();
    ~Registry();

    void Open( HKEY hKey, const String& SubKey );
    void Create( HKEY hKey, const String& SubKey );

    void SetValue( const String& Name, unt32 Data );
    void SetValue( const String& Name, const String& Data );
    void SetValue( const String& Name, const void* pData, unt32 Length );

    void GetValue( const String& Name, unt32& Data );
    void GetValue( const String& Name, String& Data );
    void GetValue( const String& Name, boost::shared_array<unt08>& pData, unt32& Length );

    void DeleteValue( const String& ValueName );

    static void DeleteKey( HKEY hKey, const String& SubKey );
    static void DeleteValue( HKEY hKey, const String& SubKey, const String& ValueName );
  private:
    void  Close();

    HKEY  m_hKey;
  };
}

#endif
