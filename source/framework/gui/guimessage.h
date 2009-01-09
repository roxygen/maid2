#ifndef maid2_framework_gui_guimessage_h
#define maid2_framework_gui_guimessage_h

#include"../../config/define.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParam
  {
  public:
    enum MESSAGE
    {
      MESSAGE_CREATE,  //  作成された
      MESSAGE_DESTROY, //  削除された

      MESSAGE_MOUSEMOVE, //  マウスが移動した
    };
    IGUIParam( MESSAGE m ):Message(m){}
    virtual ~IGUIParam(){}
    const MESSAGE Message;
  };

  class GUIMESSAGE_CREATE : public IGUIParam
  {
  public:
    GUIMESSAGE_CREATE():IGUIParam(MESSAGE_CREATE){}
  };

  class GUIMESSAGE_DESTROY : public IGUIParam
  {
  public:
    GUIMESSAGE_DESTROY():IGUIParam(MESSAGE_DESTROY){}
  };

  class GUIMESSAGE_MOUSEMOVE : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEMOVE():IGUIParam(MESSAGE_MOUSEMOVE){}
  };

  typedef boost::shared_ptr<IGUIParam> SPGUIPARAM;

}

#endif