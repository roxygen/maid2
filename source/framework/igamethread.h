
/*!
    @file
    @brief ゲーム実装用フレームワーク
 */

#ifndef framework_igamethread_h
#define framework_igamethread_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"../auxiliary/mathematics.h"
#include"osmessage.h"
#include"../graphics/core/idevice.h"
#include"../graphics/core/idevicelist.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGameThread 
  {
  public:
    virtual ~IGameThread(){}

    struct DEFAULTCONFIG
    {
      struct GRAPHICS
      {
        int   DeviceID;            //!<  使うデバイスＩＤ
        bool  IsSoftwareVertexProcessing; //!<  頂点変換をソフトウェアで行うか？
        SIZE2DI ScreenSize;

        int DrawCommandCount; //  DeferredRendering に使うコンテキストの数

        //  GraphicCore::SCREENMODE
      };

      String WindowCaption;
      GRAPHICS Graphics;
    };

    bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice );

    struct BEGINPARAM
    {
      std::vector<Graphics::SPDRAWCOMMAND> DrawCommand;
//      SPFONTDRIVER		pFontDriver;
//      SPKEYBORDDRIVER		pKeybordDriver;
//      SPMOUSEDRIVER		pMouseDriver;
//      SPCHARCODEDRIVER	pCharCodeDriver;
//      SPINPUTMETHODDRIVER	pIMEDriver;
    };

    void Initialize( const BEGINPARAM& param );
    void OnLoop();
    void Finalize();

    void OnOSMessage( const SPOSMESSAGE& pMess );

//    void OnNextFrame( CGraphic& gra );

    bool IsExitGame();
    void OnExitGame();

    const DEFAULTCONFIG& GetDefaultConfig() const;
  protected:
    virtual bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& VideoDevice, DEFAULTCONFIG& conf );
    virtual void LocalInitialize( const BEGINPARAM& param );
    virtual void LocalLoop();
    virtual void LocalFinalize();



    enum OSMESSAGECALLBACK
    {
      OSMESSAGECALLBACK_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGECALLBACK_DEFAULT,  //!<	デフォルトの動作を呼び出す

    };
    virtual OSMESSAGECALLBACK LocalOnOSMessage( const SPOSMESSAGE& pMess );

  private:
    DEFAULTCONFIG m_DefaultConfig;
    bool		m_IsExitGame;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
