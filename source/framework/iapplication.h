
/*!
	  @file
	  @brief アプリケーションの雛形
 */

#ifndef framework_iapplication_h
#define framework_iapplication_h


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
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! アプリケーションが起動していいかチェックする
    /*!
        @return 起動していいなら true
                起動がダメなら false
     */
    virtual bool BootCheck();

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! アプリケーションの初期化
    /*!
     */
    virtual void Initialize();

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 処理の開始
    /*!
     */
    virtual void OnLoop();


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! アプリケーションの開放
    /*!
     */
    virtual void Finalize();

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ＯＳデバイスの取得
    /*!
        @return OSデバイス
     */
    virtual IOSDevice& GetOSDevice()=0;

  private:
    int m_ExitCode;
  };
}

#endif
