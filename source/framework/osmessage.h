#ifndef framework_osmessage_h
#define framework_osmessage_h

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  namespace OSMessage
  {
    //  ＯＳから飛んでくるメッセージ
    struct Base
    {
      enum MESSAGE
      {
        NONE,               //!<	何もしない
        PUSHCLOSEBUTTON,    //!<	クローズボタンが押された
        PUSHMAXIMIZEBUTTON,	//!<	最大化ボタンが押された
        REDRAW,             //!<	再描画の必要がある

        CHARACODE,          //!<	WM_CHAR
        IMERESULT,          //!<	WM_CHAR
        ACTIVATE,           //!<	WM_ACTIVATE
      };

      Base( MESSAGE mess ) : Message(mess){}

      const MESSAGE	Message;
    };
  }

  typedef	boost::shared_ptr<OSMessage::Base>  SPOSMESSAGE;
  typedef	std::vector<SPOSMESSAGE>          OSMESSAGEMESSAGEQUE;
}


#endif
