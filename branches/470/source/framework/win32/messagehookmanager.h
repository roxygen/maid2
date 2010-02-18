﻿/*!
    @file
    @brief Win32 用メッセージプロシージャ
 */


#ifndef maid2_framework_win32_messagehookmanager_h
#define maid2_framework_win32_messagehookmanager_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/globalpointer.h"
#include"messagehook.h"

#include<map>
#include<list>

namespace Maid
{
  class MessageHookManager : public GlobalPointer<MessageHookManager>
  {
  public:
    MessageHookManager();
    ~MessageHookManager();

		void Initialize();
		void Finalize();

    void AddHook( MessageHook* pHook, HWND hWnd );
    void DelHook( MessageHook* pHook, HWND hWnd );

    void SetInterruptHook( MessageHook* pHook );

    LRESULT CALLBACK OnMessage( WindowsMessage& mess );

  private:
    typedef std::list<MessageHook*> HOOKLIST;
    typedef std::map<HWND,HOOKLIST> HWNDMAP;
    HWNDMAP     	m_ProcMap;		//!<  各ハンドルごとのフックリスト

    MessageHook*  m_pInterruptHook;
  };

}

#endif
