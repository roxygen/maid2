#ifndef maid2_storage_storagemessage_h
#define maid2_storage_storagemessage_h

#include"../config/define.h"
#include"../config/typedef.h"

#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class StorageObject
  {
  public:
    StorageObject() : MessageCount(0){}
    virtual ~StorageObject(){}
    unt MessageCount;
  };

	typedef	boost::shared_ptr<StorageObject>	SPSTORAGEOBJECT;

	//	描画スレッドに投げる　メッセージクラス
  namespace StorageMessage
  {
    class Base
    {
    public:
      enum MESSAGE
      {
        NONE,				    //!<	何もしない

        FILE_W_CREATE,  //!<  ファイルの作成＆オープン
        FILE_W_OPEN,    //!<  ファイルのオープン
        FILE_W_ADD,     //!<  ファイルの追記オープン
        FILE_W_WRITE,   //!<  ファイルの書き込み
        FILE_W_SEEK,    //!<  ファイルポジションの移動
        FILE_W_CLOSE,   //!<  ファイルを閉じる

        FILE_R_OPEN,    //!<  ファイルのオープン
        FILE_R_READ,    //!<  ファイルの読み込み
        FILE_R_SEEK,    //!<  ファイルポジションの移動
        FILE_R_CLOSE,   //!<  ファイルを閉じる
      };

      Base( MESSAGE mess ) : Message(mess){}
      const MESSAGE	Message;
      SPSTORAGEOBJECT pObject;
    };
  }

	typedef	boost::shared_ptr<StorageMessage::Base>	SPSTORAGEMESSAGE;
	typedef	std::vector<SPSTORAGEMESSAGE>			      STORAGEMESSAGEQUE;
}



#endif
