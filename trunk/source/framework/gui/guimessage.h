#ifndef maid2_framework_gui_guimessage_h
#define maid2_framework_gui_guimessage_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/string.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParam
  {
  public:
    enum MESSAGE
    {
      MESSAGE_ADDCHILED, //  子になった
      MESSAGE_DELCHILED, //  子から解放された

      MESSAGE_SETOFFSET,    //  オフセットの変更
      MESSAGE_ENABLE,       //  稼動状態が変更
      MESSAGE_VISIBLE,      //  表示状態が変更

      MESSAGE_MOUSEMOVE,//  マウスが移動した
      MESSAGE_MOUSEIN,  //  マウスがパーツの範囲内に入った
      MESSAGE_MOUSEOUT, //  マウスがパーツの範囲内から出た
      MESSAGE_MOUSEDOWN,//  マウスボタンが押された
      MESSAGE_MOUSEUP,  //  マウスボタンが離された


      MESSAGE_BUTTON_CLICK,  //  ボタンがクリックされた

      MESSAGE_CHECKBOX_SETCHECK,  //  チェックボックスの状態が変更された

      MESSAGE_RADIOBUTTON_SETCHECK,  //  ラジオボタンがチェックされた

      MESSAGE_RADIDGROUP_MOUSEMOVE,   //  マウスが移動した
      MESSAGE_RADIDGROUP_MOUSEIN,     //  マウスがパーツの範囲内に入った
      MESSAGE_RADIDGROUP_MOUSEOUT,    //  マウスがパーツの範囲内から出た
      MESSAGE_RADIDGROUP_MOUSEDOWN,   //  マウスボタンが押された
      MESSAGE_RADIDGROUP_MOUSEUP,     //  マウスボタンが離された
      MESSAGE_RADIDGROUP_CHANGESELECT,//  マウスボタンが離された

    };
    IGUIParam( MESSAGE m ):Message(m){}
    virtual ~IGUIParam(){}
    const MESSAGE Message;

    enum BUTTONTYPE
    {
      BUTTON_LEFT=0,
      BUTTON_RIGHT,
      BUTTON_MIDDLE,
    };
  };

  String CreateMessageLog( const IGUIParam& param );

  typedef boost::shared_ptr<IGUIParam> SPGUIPARAM;


  class IGUIParts;
  class GUIMessageQue;
  class GUIMESSAGE_ADDCHILED : public IGUIParam
  {
  public:
    GUIMESSAGE_ADDCHILED():IGUIParam(MESSAGE_ADDCHILED){}
    GUIMessageQue*  pPostTarget;
    IGUIParts*  pParent;
    unt         ID;
  };

  class GUIMESSAGE_DELCHILED : public IGUIParam
  {
  public:
    GUIMESSAGE_DELCHILED():IGUIParam(MESSAGE_DELCHILED){}
  };


  class GUIMESSAGE_SETOFFSET : public IGUIParam
  {
  public:
    GUIMESSAGE_SETOFFSET():IGUIParam(MESSAGE_SETOFFSET){}
    VECTOR2DI Offset;
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
    BUTTONTYPE Button;
  };

  class GUIMESSAGE_MOUSEUP : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEUP():IGUIParam(MESSAGE_MOUSEUP){}
    POINT2DI Position;
    BUTTONTYPE Button;
  };

  class GUIMESSAGE_BUTTON_CLICK : public IGUIParam
  {
  public:
    GUIMESSAGE_BUTTON_CLICK():IGUIParam(MESSAGE_BUTTON_CLICK){}
    POINT2DI Position;
    BUTTONTYPE Button;
  };

  class GUIMESSAGE_CHECKBOX_SETCHECK : public IGUIParam
  {
  public:
    GUIMESSAGE_CHECKBOX_SETCHECK():IGUIParam(MESSAGE_CHECKBOX_SETCHECK){}
    bool NewState;
  };


  class GUIMESSAGE_RADIOBUTTON_SETCHECK : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIOBUTTON_SETCHECK():IGUIParam(MESSAGE_RADIOBUTTON_SETCHECK){}
  };


  class GUIMESSAGE_RADIDGROUP_MOUSEMOVE : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_MOUSEMOVE():IGUIParam(MESSAGE_RADIDGROUP_MOUSEMOVE){}
    POINT2DI Position;
    int ButtonNo;
  };

  class GUIMESSAGE_RADIDGROUP_MOUSEIN : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_MOUSEIN():IGUIParam(MESSAGE_RADIDGROUP_MOUSEIN){}
    POINT2DI Position;
    int ButtonNo;
  };

  class GUIMESSAGE_RADIDGROUP_MOUSEOUT : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_MOUSEOUT():IGUIParam(MESSAGE_RADIDGROUP_MOUSEOUT){}
    POINT2DI Position;
    int ButtonNo;
  };

  class GUIMESSAGE_RADIDGROUP_MOUSEDOWN : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_MOUSEDOWN():IGUIParam(MESSAGE_RADIDGROUP_MOUSEDOWN){}
    POINT2DI Position;
    int ButtonNo;
  };

  class GUIMESSAGE_RADIDGROUP_MOUSEUP : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_MOUSEUP():IGUIParam(MESSAGE_RADIDGROUP_MOUSEUP){}
    POINT2DI Position;
    int ButtonNo;
  };

  class GUIMESSAGE_RADIDGROUP_CHANGESELECT : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIDGROUP_CHANGESELECT():IGUIParam(MESSAGE_RADIDGROUP_CHANGESELECT){}
    int OldNo;  //  以前選択されていた番号
    int NewNo;  //  新しく選択された番号
  };

}

#endif