/*!
    @file
    @brief  DLL 操作のラッパクラス
 */

#ifndef framework_win32_dllwrapper_h
#define framework_win32_dllwrapper_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/string.h"


namespace Maid
{
  class DllWrapper
  {
  public:
    DllWrapper();
    ~DllWrapper();

    void	Load( const String& DllName );
    FARPROC	GetProcAddress( const String& FunctionName ) const;

    void Free();

  private:
    String		m_DllName;    //!<	読み込んだＤＬＬの名前
    HINSTANCE m_hDll;       //!<	読みこんだＤＬＬのハンドル
  };
}

#endif
