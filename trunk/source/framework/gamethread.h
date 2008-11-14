
/*!
    @file
    @brief ゲーム実装用フレームワーク
 */

#ifndef framework_gamethread_h
#define framework_gamethread_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"../auxiliary/mathematics.h"
#include"osmessage.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  struct APPCONFIG
  {
    struct GRAPHIC
    {
      bool IsFullScreen;
      SIZE2DI Size;
    };
/*

    struct SOUND
    {
      int SamplesPerSec;
      int BitsPerSample;
      int Channels;
    };
*/

    String  WindowCaption;
    GRAPHIC Graphic;
 //   SOUND Sound;
  };



  class GameThread 
  {
  public:
    virtual ~GameThread(){}

    void OnSetup();

    struct BEGINPARAM
    {
//      CGraphic::FRAMEINFO* pInfo;
//      SPFONTDRIVER		pFontDriver;
//      SPKEYBORDDRIVER		pKeybordDriver;
//      SPMOUSEDRIVER		pMouseDriver;
//      SPCHARCODEDRIVER	pCharCodeDriver;
//      SPINPUTMETHODDRIVER	pIMEDriver;
      int dummy;
    };

    void OnBegin( const BEGINPARAM& param );
    void OnLoop();
    void OnEnd();

    void OnOSMessage( const SPOSMESSAGE& pMess );

//    void OnNextFrame( CGraphic& gra );

    bool IsExitGame();
    void OnExitGame();

    const APPCONFIG& GetAppConfig();

  protected:
    virtual void LocalSetup( APPCONFIG& conf );
    virtual void LocalBegin( const BEGINPARAM& param );
    virtual void LocalLoop();
    virtual void LocalEnd();



    enum OSMESSAGECALLBACK
    {
      OSMESSAGECALLBACK_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGECALLBACK_DEFAULT,  //!<	デフォルトの動作を呼び出す

    };
    virtual OSMESSAGECALLBACK LocalOnOSMessage( const SPOSMESSAGE& pMess );

  private:
    APPCONFIG	m_Config;
    bool		m_IsExitGame;
  };

  typedef	boost::shared_ptr<GameThread>	SPGAMETHREAD;
}
#endif
