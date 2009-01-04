
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
#include"../graphics/core/ifontdevice.h"
#include"../graphics/core/idrawcommand.h"
#include"../graphics/graphicsobjecttemplate.h"

#include"../input/core/idevicelist.h"
#include"../input/core/imousedevice.h"
#include"../sound/core/idevicelist.h"


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

      struct INPUT
      {
        int MouseDeviceID;    //  使うマウスID
        int KeybordDeviceID;  //  使うキーボードID
      };

      struct SOUND
      {
        int DeviceID;        //  使うデバイスID
        bool IsBackGround;  //  サウンド処理は別スレッドで行うか？
      };

      String WindowCaption;
      GRAPHICS  Graphics;
      INPUT     Input;
      SOUND     Sound;
    };

    struct DEVICELIST
    {
      std::vector<Graphics::IDeviceList::INFO>   Graphics;
      std::vector<Input::IDeviceList::MOUSEINFO> Mouse;
      std::vector<Input::IDeviceList::KEYBORDINFO> Keybord;
      std::vector<Sound::IDeviceList::INFO> Sound;
    };

    bool SelectDevice( const DEVICELIST& DeviceList );

    struct BEGINPARAM
    {
      SIZE2DI ScreenSize;
      std::vector<Graphics::SPDRAWCOMMAND> DrawCommand;
      Graphics::SPFONTDEVICE  pFontDevice;
      Input::SPKEYBORDDEVICE  pKeybordDevice;
      Input::SPMOUSEDEVICE    pMouseDevice;
//      SPCHARCODEDRIVER	pCharCodeDriver;
//      SPINPUTMETHODDRIVER	pIMEDriver;
    };

    void Initialize( const BEGINPARAM& param );
    void UpdateFrame();
    void UpdateDraw();
    void Finalize();

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

    bool IsExitGame();
    void OnExitGame();

    const DEFAULTCONFIG& GetDefaultConfig() const;

  protected:
    virtual bool LocalSelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );

    virtual void LocalInitialize( const BEGINPARAM& param );
    virtual void LocalUpdateFrame();
    virtual void LocalUpdateDraw();
    virtual void LocalFinalize();




    virtual OSMESSAGESTATE LocalOnOSMessage( const OSMessage::Base& Message );

  private:
    DEFAULTCONFIG m_DefaultConfig;
    bool		m_IsExitGame;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
