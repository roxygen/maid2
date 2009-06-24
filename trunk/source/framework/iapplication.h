﻿
/*!
	  @file
	  @brief アプリケーションの雛形
 */

#ifndef maid2_framework_iapplication_h
#define maid2_framework_iapplication_h


#include"../config/define.h"
#include"iosdevice.h"

namespace Maid
{
  class IApplication
  {
  public:
    IApplication();
    virtual ~IApplication();

    int Run();

    void OnExit( int ExitCode );

  protected:
    virtual bool BootCheck();
    virtual void Initialize();
    virtual void OnLoop();
    virtual void Finalize();
    virtual IOSDevice& GetOSDevice()=0;

  private:
    int m_ExitCode;
  };
}

#endif
