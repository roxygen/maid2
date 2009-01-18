
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

#include"../graphics/core/idevicelist.h"
#include"../input/core/idevicelist.h"
#include"../sound/core/idevicelist.h"

#include"../graphics/graphicscore.h"
#include"../sound/soundcore.h"
#include"../input/mouse.h"
#include"../input/keybord.h"
#include"../input/textinput.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGameFrameWork;
  class IGameThread 
  {
    friend IGameFrameWork;
  public:
    IGameThread();
    virtual ~IGameThread();

    bool IsExitGame();
    void OnExitGame();


    GraphicsCore& GetGraphicsCore();
    SoundCore& GetSoundCore();
    Mouse& GetMouse();
    Keybord& GetKeybord();
    TextInput& GetTextInput();


  protected:

    struct DEFAULTCONFIG
    {
      struct GRAPHICS
      {
        int   DeviceID;            //!<  使うデバイスＩＤ
        bool  IsSoftwareVertexProcessing; //!<  頂点変換をソフトウェアで行うか？
        SIZE2DI ScreenSize;
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
      bool SystemCursorDraw;
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
    virtual bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );

    virtual void Initialize();
    virtual void UpdateFrame()=0;
    virtual void UpdateDraw()=0;
    virtual void Finalize();

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    virtual OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

    IGameFrameWork* GetFrameWork();
  private:
    bool		      m_IsExitGame;
    IGameFrameWork* m_pFrameWork;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
