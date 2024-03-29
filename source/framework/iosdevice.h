﻿/*!
	  @file
	  @brief OS固有の処理のベース
 */

#ifndef maid2_framework_iosdevice_h
#define maid2_framework_iosdevice_h

#include"../config/define.h"

#include<boost/smart_ptr.hpp>


namespace Maid
{
  class IOSDevice
  {
  public:
    virtual ~IOSDevice(){}

    //! アプリケーションの初期化
    /*!
     */
    virtual void Initialize()=0;

    //! アプリケーションの開放
    /*!
     */
		virtual void Finalize()=0;

    //! アプリが生きているか？
    /*!
         @retval	true  生きている
         @retval	false 終了する必要がある
     */
    virtual bool IsValid()=0;

    //! アプリケーションを終了させる
    /*!
	      アプリを終了させるときに呼んでください
     */
    virtual void Exit()=0;


  };

  typedef	boost::shared_ptr<IOSDevice>  SPOSDEVICE;
}


#endif
