﻿#ifndef maid2_auxiliary_thread_win32_threadcontroller_h
#define maid2_auxiliary_thread_win32_threadcontroller_h

/*!
  @file
  @brief	スレッド制御クラス
 */

#include"../../../config/define.h"
#include"../../string.h"
#include<process.h>
#include<boost/function.hpp>
#include<boost/bind.hpp>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class ThreadController
  {
  public:

    /*!
      @class  BRIGEDATA ThreadController.h
      @brief  スレッドを作成したクラスとスレッドを呼ばれたクラスが
    \n        受け渡すデータ
     */
    struct BRIGEDATA
    {
      volatile bool IsExit;     //!<  強制終了命令がでているか？
      volatile bool IsExecute;  //!<  スレッドが起動中か？
      volatile bool IsAssert;   //!<  スレッドが強制終了しちゃったか？
    };

  public:
    ThreadController();
    ~ThreadController();

    //!	スレッドの優先度
    enum PRIORITY
    {
      PRIORITY_HIGH2,   //!<  一番高い
      PRIORITY_HIGH1,
      PRIORITY_NORMAL,  //!<  デフォルト
      PRIORITY_LOW1,
      PRIORITY_LOW2,    //!<  一番低い
    };

    typedef boost::function1<unt, volatile BRIGEDATA&>	THREADFUNCTION;
    void  SetFunc( const THREADFUNCTION& Function );
    void  Execute( PRIORITY priority = PRIORITY_NORMAL );
    void  Close();

    void SetPriority( PRIORITY priority );
    void SetProcesserMask( unt32 mask );

    void Pause();
    void Resume();

    bool IsExited() const;
    bool IsAssert() const;
    unt GetReturnCode() const;
    unt GetID() const;

    static void Sleep( unt time );
    static unt GetCurrentID();
    static void SetCurrentThreadProcesserMask( unt32 mask );

  private:
    class Impl;
    boost::shared_ptr<Impl>	m_pImpl;
 };

  #define MakeThreadObject(pFunc,pObject) boost::bind( pFunc, pObject, _1 )

}

#endif


