#ifndef maid2_framework_gui_guimessage_h
#define maid2_framework_gui_guimessage_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParam
  {
  public:
    enum MESSAGE
    {
      MESSAGE_INITIALIZE, //  作成された
      MESSAGE_FINALIZE,   //  削除された


      MESSAGE_SETPOSITION,  //  座標が移動した
      MESSAGE_SETZORDER,    //  Zが変更された
      MESSAGE_ENABLE,       //  稼動状態が変更
      MESSAGE_VISIBLE,      //  表示状態が変更

      MESSAGE_MOUSEMOVE,//  マウスが移動した
      MESSAGE_MOUSEIN,  //  マウスがパーツの範囲内に入った
      MESSAGE_MOUSEOUT, //  マウスがパーツの範囲内から出た
      MESSAGE_MOUSEDOWN,//  マウスボタンが押された
      MESSAGE_MOUSEUP,  //  マウスボタンが離された


      MESSAGE_BUTTON_CLICK  //  ボタンがクリックされた
    };
    IGUIParam( MESSAGE m ):Message(m){}
    virtual ~IGUIParam(){}
    const MESSAGE Message;
  };

  typedef boost::shared_ptr<IGUIParam> SPGUIPARAM;

  class GUIMESSAGE_INITIALIZE : public IGUIParam
  {
  public:
    GUIMESSAGE_INITIALIZE():IGUIParam(MESSAGE_INITIALIZE){}
  };

  class GUIMESSAGE_FINALIZE : public IGUIParam
  {
  public:
    GUIMESSAGE_FINALIZE():IGUIParam(MESSAGE_FINALIZE){}
  };

  class GUIMESSAGE_SETPOSITION : public IGUIParam
  {
  public:
    GUIMESSAGE_SETPOSITION():IGUIParam(MESSAGE_SETPOSITION){}
    POINT2DI Position;
  };

  class GUIMESSAGE_SETZORDER : public IGUIParam
  {
  public:
    GUIMESSAGE_SETZORDER():IGUIParam(MESSAGE_SETZORDER){}
    int Z;
  };

  class GUIMESSAGE_ENABLE : public IGUIParam
  {
  public:
    GUIMESSAGE_ENABLE():IGUIParam(MESSAGE_ENABLE){}
    bool IsEnable;
  };

  class GUIMESSAGE_VISIBLE : public IGUIParam
  {
  public:
    GUIMESSAGE_VISIBLE():IGUIParam(MESSAGE_VISIBLE){}
    bool IsVisible;
  };



  class GUIMESSAGE_MOUSEMOVE : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEMOVE():IGUIParam(MESSAGE_MOUSEMOVE){}
    POINT2DI Position;
  };

  class GUIMESSAGE_MOUSEIN : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEIN():IGUIParam(MESSAGE_MOUSEIN){}
    POINT2DI Position;
  };

  class GUIMESSAGE_MOUSEOUT : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEOUT():IGUIParam(MESSAGE_MOUSEOUT){}
    POINT2DI Position;
  };

  class GUIMESSAGE_MOUSEDOWN : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEDOWN():IGUIParam(MESSAGE_MOUSEDOWN){}
    POINT2DI Position;
    int Button;
  };

  class GUIMESSAGE_MOUSEUP : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEUP():IGUIParam(MESSAGE_MOUSEUP){}
    POINT2DI Position;
    int Button;
  };

  class GUIMESSAGE_BUTTON_CLICK : public IGUIParam
  {
  public:
    GUIMESSAGE_BUTTON_CLICK():IGUIParam(MESSAGE_BUTTON_CLICK){}
    POINT2DI Position;
  };
}

#endif