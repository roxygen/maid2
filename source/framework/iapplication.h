
/*!
	  @file
	  @brief アプリケーションの雛形
 */

#ifndef framework_iapplication_h
#define framework_iapplication_h


#include"../config/define.h"
#include"iosdriver.h"

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
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 処理開始の準備
    /*!
     */
    virtual void OnSetup()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 処理の開始
    /*!
     */
    virtual void OnLoop()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 処理の開放
    /*!
     */
    virtual void OnCleanup()=0;

    void SetOSDriver( const SPOSDRIVER& pDriver );

  private:
    SPOSDRIVER  m_pOSDriver;
    int m_ExitCode;
  };
}

#endif
