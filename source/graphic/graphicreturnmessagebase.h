/*!	
	@file
	@brief	返信メッセージ 定義
*/

#ifndef maid2_graphic_graphicreturnmessagebase_h
#define maid2_graphic_graphicreturnmessagebase_h

#include"../config/define.h"

#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  //	描画スレッドから帰ってくるメッセージ

  namespace GraphicReturnMessage
  {
    class Base
    {
      typedef uintptr_t OBJECTID;
      enum MESSAGE
      {
        COPYRENDERTARGET,		//	サーフェスの取得
        DRIVERCAPS,		        //	ドライバ情報の取得
      };
      Base( MESSAGE mess ) : Message(mess){}
      const MESSAGE	Message;
    };
  }

  typedef	boost::shared_ptr<GraphicReturnMessage::Base>	SPRETURNMESSAGE;
  typedef	std::vector<SPRETURNMESSAGE>			            RETURNMESSAGEQUE;

}

#endif